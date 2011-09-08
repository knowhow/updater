/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "loadmetasql.h"

#include <QDomDocument>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()

#include "metasql.h"
#include "xsqlquery.h"

#define DEBUG false

LoadMetasql::LoadMetasql(const QString &name, const QString &group,
                         const bool system,
                         const QString &comment, const QString &filename)
  : Loadable("loadmetasql", name, 0, system, comment, filename)
{
  _pkgitemtype = "M";
  _group = group;
}

LoadMetasql::LoadMetasql(const QDomElement &elem, const bool system,
                         QStringList &msg, QList<bool> &fatal)
  : Loadable(elem, system, msg, fatal)
{
  if (DEBUG)
    qDebug("LoadMetasql::LoadMetasql(QDomElement) entered");

  _pkgitemtype = "M";

  if (elem.nodeName() != "loadmetasql")
  {
    msg.append(TR("Creating a LoadMetasql element from a %1 node.")
              .arg(elem.nodeName()));
    fatal.append(false);
  }

  if (elem.hasAttribute("group"))
    _group = elem.attribute("group");

  if (elem.hasAttribute("enabled"))
  {
    msg.append(TR("Node %1 '%2' has an 'enabled' "
                           "attribute which is ignored for MetaSQL statements.")
                       .arg(elem.nodeName()).arg(elem.attribute("name")));
    fatal.append(false);
  }

}

int LoadMetasql::writeToDB(const QByteArray &pdata, const QString pkgname, QString &errMsg)
{
  if (pdata.isEmpty())
  {
    errMsg = TR("<font color=orange>The MetaSQL statement %1 is empty.</font>")
                         .arg(_name);
    return -2;
  }

  QString metasqlStr = QString(pdata);
  QStringList lines  = metasqlStr.split("\n");
  QRegExp groupRE    = QRegExp("(^\\s*--\\s*GROUP:\\s*)(.*)",Qt::CaseInsensitive);
  QRegExp nameRE     = QRegExp("(^\\s*--\\s*NAME:\\s*)(.*)", Qt::CaseInsensitive);
  QRegExp notesRE    = QRegExp("(^\\s*--\\s*NOTES:\\s*)(.*)",Qt::CaseInsensitive);
  QRegExp dashdashRE = QRegExp("(^\\s*--\\s*)(.*)");

  for (int i = 0; i < lines.size(); i++)
  {
    if (DEBUG)
      qDebug("LoadMetasql::writeToDB looking at %s", qPrintable(lines.at(i)));

    if (groupRE.indexIn(lines.at(i)) >= 0)
    {
      _group = groupRE.cap(2).trimmed();
      if (DEBUG)
        qDebug("LoadMetasql::writeToDB() found group %s", qPrintable(_group));
    }
    else if (nameRE.indexIn(lines.at(i)) >= 0)
    {
      _name = nameRE.cap(2).trimmed();
      if (DEBUG)
        qDebug("LoadMetasql::writeToDB() found name %s", qPrintable(_name));
    }
    else if (notesRE.indexIn(lines.at(i)) >= 0)
    {
      _comment = notesRE.cap(2).trimmed();
      while (dashdashRE.indexIn(lines.at(++i)) >= 0)
        _comment += " " + dashdashRE.cap(2).trimmed();
      if (DEBUG)
        qDebug("LoadMetasql::writeToDB() found notes %s", qPrintable(_comment));
    }
  }

  if (DEBUG)
    qDebug("LoadMetasql::writeToDB(): name %s group %s notes %s\n%s",
           qPrintable(_name), qPrintable(_group), qPrintable(_comment),
           qPrintable(metasqlStr));

  QString destschema = "public";
  if (_schema.isEmpty()        &&   pkgname.isEmpty())
    ;   // leave it alone
  else if (_schema.isEmpty()   && ! pkgname.isEmpty())
    destschema = pkgname;
  else if ("public" == _schema)
    ;   // leave it alone
  else if (! _schema.isEmpty())
    destschema = _schema;

  XSqlQuery gradedsavepoint("SAVEPOINT savemetasql_graded;");
  MetaSQLQuery upsertm("SELECT saveMetasql(<? value('group') ?>,"
                       "       <? value('name') ?>,   <? value('notes') ?>,"
                       "       <? value('query') ?>,"
                       "       CAST(<? value('system') ?> AS BOOLEAN),"
                       "       <? value('schema') ?>"
                       "<? if not exists('skipgrade') ?>"
                       "        , <? value('grade') ?>"
                       "<? endif ?>"
                       ") AS result;");
  ParameterList upsertp;
  upsertp.append("group", _group);
  upsertp.append("name",  _name);
  upsertp.append("notes", _comment);
  upsertp.append("query", metasqlStr);
  upsertp.append("system",_system);
  upsertp.append("schema",destschema);
  upsertp.append("grade", _grade);

  int metasqlid = -1;

  XSqlQuery upsert = upsertm.toQuery(upsertp);
  if (upsert.first())
    metasqlid = upsert.value(0).toInt();
  else if (upsert.lastError().type() != QSqlError::NoError)
  {
    XSqlQuery gradedrollback("ROLLBACK TO SAVEPOINT savemetasql_graded;");
    upsertp.append("skipgrade");
    upsert = upsertm.toQuery(upsertp);
    if (upsert.first())
      metasqlid = upsert.value(0).toInt();
    if (upsert.lastError().type() != QSqlError::NoError)
    {
      QSqlError err = upsert.lastError();
      errMsg = _sqlerrtxt.arg(_filename).arg(err.driverText()).arg(err.databaseText());
      return -6;
    }
  }
  else
  {
    errMsg = TR("Saving the MetaSQL statement returned 0 rows. This should "
                "not be possible.");
    return -6;
  }

  if (metasqlid < 0)
  {
    errMsg = TR("The %1 stored procedure failed, returning %2.")
              .arg("saveMetasql").arg(metasqlid);
    return -5;
  }
  else
    XSqlQuery gradedrelease("RELEASE SAVEPOINT savemetasql_graded;");

  if (DEBUG)
    qDebug("LoadMetasql::writeToDB() executed %s and got %d in return",
           qPrintable(upsert.executedQuery()), metasqlid);

  return metasqlid;
}
