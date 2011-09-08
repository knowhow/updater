/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "loadappscript.h"

#include <QDomElement>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()
#include <limits.h>

#include "xsqlquery.h"

LoadAppScript::LoadAppScript(const QString &name, const int order,
                             const bool system, const bool enabled,
                             const QString &comment, const QString &filename)
  : Loadable("loadappscript", name, order, system, comment, filename)
{
  _enabled = enabled;
  _pkgitemtype = "C";
}

LoadAppScript::LoadAppScript(const QDomElement &elem, const bool system,
                             QStringList &msg, QList<bool> &fatal)
  : Loadable(elem, system, msg, fatal)
{
  _pkgitemtype = "C";

  if (_name.isEmpty())
  {
    msg.append(TR("The script in %1 does not have a name.")
                         .arg(_filename));
    fatal.append(true);
  }

  if (elem.nodeName() != "loadappscript")
  {
    msg.append(TR("Creating a LoadAppScript element from a %1 node.")
           .arg(elem.nodeName()));
    fatal.append(false);
  }

  if (elem.hasAttribute("grade"))
  {
    msg.append(TR("Node %1 '%2' has a 'grade' attribute but should use "
                      "'order' instead.")
                   .arg(elem.nodeName()).arg(elem.attribute("name")));
    fatal.append(false);
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

int LoadAppScript::writeToDB(const QByteArray &pdata, const QString pkgname, QString &errMsg)
{
  if (_name.isEmpty())
  {
    errMsg = TR("The script does not have a name.");
    return -1;
  }

  if (pdata.isEmpty())
  {
    errMsg = TR("The script %1 is empty.").arg(_filename);
    return -2;
  }

  _minMql = new MetaSQLQuery("SELECT MIN(script_order) AS min "
                   "FROM script "
                   "WHERE (script_name=<? value(\"name\") ?>);");

  _maxMql = new MetaSQLQuery("SELECT MAX(script_order) AS max "
                   "FROM script "
                   "WHERE (script_name=<? value(\"name\") ?>);");

  _selectMql = new MetaSQLQuery("SELECT script_id, -1, -1"
                      "  FROM <? literal(\"tablename\") ?> "
                      " WHERE ((script_name=<? value(\"name\") ?>)"
                      "    AND (script_order=<? value(\"grade\") ?>));");

  _updateMql = new MetaSQLQuery("UPDATE <? literal(\"tablename\") ?> "
                      "   SET script_order=<? value(\"grade\") ?>, "
                      "       script_enabled=<? value(\"enabled\") ?>,"
                      "       script_source=<? value(\"source\") ?>,"
                      "       script_notes=<? value(\"notes\") ?> "
                      " WHERE (script_id=<? value(\"id\") ?>) "
                      "RETURNING script_id AS id; ");

  _insertMql = new MetaSQLQuery("INSERT INTO <? literal(\"tablename\") ?> ("
                      "    script_id, script_name,"
                      "    script_order, script_enabled,"
                      "    script_source, script_notes"
                      ") VALUES (DEFAULT, <? value(\"name\") ?>, "
                      "    <? value(\"grade\") ?>,  <? value(\"enabled\") ?>,"
                      "    <? value(\"source\") ?>, <? value(\"notes\") ?>) "
                      "RETURNING script_id AS id;");

  ParameterList params;
  params.append("enabled",   QVariant(_enabled));
  params.append("tablename", "script");

  return Loadable::writeToDB(pdata, pkgname, errMsg, params);
}
