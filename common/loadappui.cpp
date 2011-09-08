/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "loadappui.h"

#include <QDomDocument>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()
#include <limits.h>

#include "xsqlquery.h"

#define DEBUG false

LoadAppUI::LoadAppUI(const QString &name, const int order,
                     const bool system, const bool enabled,
                     const QString & comment, const QString &filename)
  : Loadable("loadappui", name, order, system, comment, filename)
{
  _enabled = enabled;
  _pkgitemtype = "U";
}

LoadAppUI::LoadAppUI(const QDomElement &elem, const bool system,
                     QStringList &msg, QList<bool> &fatal)
  : Loadable(elem, system, msg, fatal)
{
  _pkgitemtype = "U";

  if (elem.nodeName() != "loadappui")
  {
    msg.append(TR("Creating a LoadAppUI element from a %1 node.")
                         .arg(elem.nodeName()));
    fatal.append(false);
  }

  if (elem.hasAttribute("grade"))
  {
    msg.append(TR("Node %1 '%2' has a 'grade' attribute but "
                      "should use 'order' instead.")
                     .arg(elem.nodeName()).arg(elem.attribute("name")));
    fatal.append(false);
  }

  if (elem.hasAttribute("order"))
  {
    if (elem.attribute("order").contains("highest", Qt::CaseInsensitive))
      _grade = INT_MAX;
    else if (elem.attribute("order").contains("lowest", Qt::CaseInsensitive))
      _grade = INT_MIN;
    else
      _grade = elem.attribute("order").toInt();
  }

  _enabled = true;
  if (elem.hasAttribute("enabled"))
  {
    if (elem.attribute("enabled").contains(trueRegExp))
      _enabled = true;
    else if (elem.attribute("enabled").contains(falseRegExp))
      _enabled = false;
    else
    {
      msg.append(TR("Node %1 '%2' has an 'enabled' attribute that is "
                        "neither 'true' nor 'false'. Using '%3'.")
                         .arg(elem.nodeName()).arg(elem.attribute("name"))
                         .arg(_enabled ? "true" : "false"));
      fatal.append(false);
    }
  }
}

int LoadAppUI::writeToDB(const QByteArray &pdata, const QString pkgname, QString &errMsg)
{
  int errLine = 0;
  int errCol = 0;
  QDomDocument doc;
  if (! doc.setContent(pdata, &errMsg, &errLine, &errCol))
  {
    errMsg = TR("Error parsing file %1: %2 on line %3 column %4")
                          .arg(_filename).arg(errMsg).arg(errLine).arg(errCol);
    return -1;
  }

  QDomElement root = doc.documentElement();
  if (root.tagName() != "ui")
  {
    errMsg = TR("XML Document %1 does not have root node of 'ui'")
              .arg(_filename);
    return -2;
  }

  if (DEBUG)
    qDebug("LoadAppUI::writeToDB() name before looking for class node: %s",
           qPrintable(_name));
  QDomElement n = root.firstChildElement("class");
  if (n.isNull())
  {
    errMsg = TR("XML Document %1 does not name its class and is not a valid "
                "UI Form.")
                .arg(_filename);
    return -3;
  }
  _name = n.text();
  if (DEBUG)
    qDebug("LoadAppUI::writeToDB() name after looking for class node: %s",
           qPrintable(_name));

  _minMql = new MetaSQLQuery("SELECT MIN(uiform_order) AS min "
                   "FROM uiform "
                   "WHERE (uiform_name=<? value(\"name\") ?>);");

  _maxMql = new MetaSQLQuery("SELECT MAX(uiform_order) AS max "
                   "FROM uiform "
                   "WHERE (uiform_name=<? value(\"name\") ?>);");

  _selectMql = new MetaSQLQuery("SELECT uiform_id, -1, -1"
                      "  FROM <? literal(\"tablename\") ?> "
                      " WHERE ((uiform_name=<? value(\"name\") ?>)"
                      "   AND  (uiform_order=<? value(\"grade\") ?>));");

  _updateMql = new MetaSQLQuery("UPDATE <? literal(\"tablename\") ?> "
                      "   SET uiform_order=<? value(\"grade\") ?>, "
                      "       uiform_enabled=<? value(\"enabled\") ?>,"
                      "       uiform_source=<? value(\"source\") ?>,"
                      "       uiform_notes=<? value(\"notes\") ?> "
                      " WHERE (uiform_id=<? value(\"id\") ?>) "
                      "RETURNING uiform_id AS id;");

  _insertMql = new MetaSQLQuery("INSERT INTO <? literal(\"tablename\") ?> ("
                      "    uiform_id, uiform_name,"
                      "    uiform_order, uiform_enabled, "
                      "    uiform_source, uiform_notes"
                      ") VALUES ("
                      "    DEFAULT, <? value(\"name\") ?>,"
                      "    <? value(\"grade\") ?>, <? value(\"enabled\") ?>,"
                      "    <? value(\"source\") ?>, <? value(\"notes\") ?>) "
                      "RETURNING uiform_id AS id;");

  ParameterList params;
  params.append("enabled",   QVariant(_enabled));
  params.append("tablename", "uiform");

  return Loadable::writeToDB(pdata, pkgname, errMsg, params);
}
