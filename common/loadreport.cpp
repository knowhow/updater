/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "loadreport.h"

#include <QDomDocument>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()
#include <limits.h>

#include "xsqlquery.h"

LoadReport::LoadReport(const QString &name, const int grade, const bool system,
                       const QString &comment, const QString &filename)
  : Loadable("loadreport", name, grade, system, comment, filename)
{
  _pkgitemtype = "R";
}

LoadReport::LoadReport(const QDomElement & elem, const bool system,
                       QStringList &msg, QList<bool> &fatal)
  : Loadable(elem, system, msg, fatal)
{
  _pkgitemtype = "R";

  if (elem.nodeName() != "loadreport")
  {
    msg.append(TR("Creating a LoadAppReport element from a %1 node.")
                       .arg(elem.nodeName()));
    fatal.append(false);
  }
}

int LoadReport::writeToDB(const QByteArray &pdata, const QString pkgname, QString &errMsg)
{
  int errLine = 0;
  int errCol  = 0;
  QDomDocument doc;
  if (! doc.setContent(pdata, &errMsg, &errLine, &errCol))
  {
    errMsg = (TR("<font color=red>Error parsing file %1: %2 on "
                          "line %3 column %4</font>")
                          .arg(_filename).arg(errMsg).arg(errLine).arg(errCol));
    return -1;
  }

  QDomElement root = doc.documentElement();
  if(root.tagName() != "report")
  {
    errMsg = TR("<font color=red>XML Document %1 does not have root"
                         " node of report</font>")
                         .arg(_filename);
    return -2;
  }

  for(QDomNode n = root.firstChild(); !n.isNull(); n = n.nextSibling())
  {
    if(n.nodeName() == "name")
      _name = n.firstChild().nodeValue();
    else if(n.nodeName() == "description")
      _comment = n.firstChild().nodeValue();
  }
  QString report_src = doc.toString();

  if(_filename.isEmpty())
  {
    errMsg = TR("<font color=orange>The document %1 does not have"
                         " a report name defined</font>")
                         .arg(_filename);
    return -3;
  }

  /* the following block avoids
      ERROR:  duplicate key violates unique constraint "report_name_grade_idx"
   */
  if (! pkgname.isEmpty())
  {
    // if there's a version of the report that's not part of this pkg
    XSqlQuery select;
    select.prepare("SELECT report_id "
                   "FROM report r JOIN pg_class c ON (r.tableoid=c.oid)"
                   "              JOIN pg_namespace n ON (relnamespace=n.oid) "
                   "WHERE ((report_name=:name)"
                   "  AND  (report_grade=:grade)"
                   "  AND  (nspname<>:pkgname));");
    select.bindValue(":name",    _name);
    select.bindValue(":grade",   _grade);
    select.bindValue(":pkgname", pkgname);
    select.exec();
    if(select.first())
    {
      // then find the next open higher grade
      XSqlQuery next;
      next.prepare("SELECT MIN(sequence_value) AS next "
                       "FROM sequence "
                       "WHERE ((sequence_value NOT IN ("
                       "      SELECT report_grade"
                       "      FROM report"
                       "      WHERE (report_name=:name)))"
                       "  AND (sequence_value>=:grade));");
      next.bindValue(":name", _name);
      next.bindValue(":grade",   _grade);
      next.exec();
      if (next.first())
        _grade = next.value(0).toInt();
      else if (next.lastError().type() != QSqlError::NoError)
      {
        QSqlError err = next.lastError();
        errMsg = _sqlerrtxt.arg(_filename).arg(err.driverText()).arg(err.databaseText());
        return -8;
      }
    }
    else if (select.lastError().type() != QSqlError::NoError)
    {
      QSqlError err = select.lastError();
      errMsg = _sqlerrtxt.arg(_filename).arg(err.driverText()).arg(err.databaseText());
      return -9;
    }
  }

  _minMql = new MetaSQLQuery("SELECT MIN(report_grade) AS min "
                   "FROM report "
                   "WHERE (report_name=<? value(\"name\") ?>);");

  _maxMql = new MetaSQLQuery("SELECT MAX(report_grade) AS max "
                   "FROM report "
                   "WHERE (report_name=<? value(\"name\") ?>);");

  _selectMql = new MetaSQLQuery("SELECT report_id, -1, -1"
                      "  FROM report "
                      " WHERE ((report_name=<? value(\"name\") ?>) "
                      "    AND (report_grade=<? value(\"grade\") ?>) );");

  _updateMql = new MetaSQLQuery("UPDATE <? literal(\"tablename\") ?> "
                      "   SET report_descrip=<? value(\"notes\") ?>, "
                      "       report_source=<? value(\"source\") ?> "
                      " WHERE (report_id=<? value(\"id\") ?>) "
                      "RETURNING report_id AS id;");

  _insertMql = new MetaSQLQuery("INSERT INTO <? literal(\"tablename\") ?> ("
                      "    report_id, report_name,"
                      "    report_grade, report_source, report_descrip"
                      ") VALUES ("
                      "    DEFAULT, <? value(\"name\") ?>,"
                      "    <? value(\"grade\") ?>, <? value(\"source\") ?>,"
                      "    <? value(\"notes\") ?>) "
                      "RETURNING report_id AS id;");

  ParameterList params;
  params.append("tablename", "report");

  return Loadable::writeToDB(pdata, pkgname, errMsg, params);
}
