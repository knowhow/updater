/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "loadpriv.h"

#include <QDomDocument>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()

#include "xsqlquery.h"

#include "loadable.h"

LoadPriv::LoadPriv(const QString &nodename,
                   const QString &name, const QString &module,
                   const bool system, const QString &comment)
  : Loadable(nodename, name, 0, system, comment)
{
  _module = module;
  if (_module == "Custom" && ! _name.startsWith("Custom"))
    _name = "Custom" + _name;
  _pkgitemtype = "P";
}

LoadPriv::LoadPriv(const QDomElement &elem, const bool system,
                   QStringList &msg, QList<bool> &fatal)
  : Loadable(elem, system, msg, fatal)
{
  _pkgitemtype = "P";

  if (_name.isEmpty())
  {
    msg.append(TR("A Privilege %1 does not have a name."));
    fatal.append(true);
  }

  if (elem.hasAttribute("module"))
    _module = elem.attribute("module");
  else
  {
    _module = "Custom";
    msg.append(TR("The Privilege %1 has not been assigned to a "
                           "module. It will default to '%2'.")
                .arg(_name).arg(_module));
    fatal.append(false);
  }
}

QDomElement LoadPriv::createElement(QDomDocument &doc)
{
  QDomElement elem = doc.createElement("loadpriv");
  elem.setAttribute("name", _name);
  elem.setAttribute("module", _module);

  if(!_comment.isEmpty())
    elem.appendChild(doc.createTextNode(_comment));

  return elem;
}

int LoadPriv::writeToDB(const QString pkgname, QString &errMsg)
{
  if (_name.isEmpty())
  {
    errMsg = TR("<font color=orange>The Privilege does not have a name.</font>")
               .arg(_name);
    return -1;
  }

  if (_module.isEmpty())
  {
    errMsg = TR("<font color=orange>The Privilege %1 has not been "
                 "assigned to a module and so may not be assignable.</font>")
               .arg(_name);
  }

  _selectMql = new MetaSQLQuery("SELECT priv_id AS id, -1, -1"
                      "  FROM <? literal(\"tablename\") ?> "
                      " WHERE (priv_name=<? value(\"name\") ?>);");

  _updateMql = new MetaSQLQuery("UPDATE <? literal(\"tablename\") ?> "
                      "   SET priv_module=<? value(\"module\") ?>, "
                      "       priv_descrip=<? value(\"comment\") ?> "
                      " WHERE (priv_id=<? value(\"id\") ?>) "
                      "RETURNING priv_id AS id;");

  _insertMql = new MetaSQLQuery("INSERT INTO <? literal(\"tablename\") ?> ("
                      "    priv_id, priv_module, priv_name, priv_descrip "
                      ") VALUES ("
                      "    DEFAULT, <? value(\"module\") ?>,"
                      "    <? value(\"name\") ?>, <? value(\"notes\") ?>) "
                      "RETURNING priv_id AS id;");

  ParameterList params;
  params.append("tablename", "priv");
  params.append("module",    _module);

  return Loadable::writeToDB(QByteArray(), pkgname, errMsg, params);
}
