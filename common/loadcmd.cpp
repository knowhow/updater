/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "loadcmd.h"

#include <QDomDocument>
#include <QSqlError>
#include <QStringList>
#include <QVariant>     // used by XSqlQuery::bindValue()

#include "loadable.h"
#include "xsqlquery.h"

#define DEBUG false

LoadCmd::LoadCmd(const QString &nodename, const QString &name,
                 const QString &module, const QString &title,
                 const QString &privname, const QString &executable,
                 const QStringList &args, const QString &comment)
  : Loadable(nodename, name, 0, false, comment)
{
  _args       = args;
  _executable = executable;
  _module     = module;
  _privname   = privname;
  _title      = title;
  _pkgitemtype= "D";
}

LoadCmd::LoadCmd(const QDomElement &elem, const bool system,
                 QStringList &msg, QList<bool> &fatal)
  : Loadable(elem, system, msg, fatal)
{
  _pkgitemtype= "D";

  // name isn't required, at least according to the db and guiclient

  if (elem.hasAttribute("title"))
    _title = elem.attribute("title");
  else
  {
    msg.append(TR("Node %1 '%2' does not have a title. Its menu item "
                           "will be blank.")
                         .arg(elem.nodeName()).arg(elem.attribute("name")));
    fatal.append(true);
  }

  if (elem.hasAttribute("module"))
    _module = elem.attribute("module");
  else
  {
    msg.append(TR("Node %1 '%2' does not name a module. The Custom "
                           "Command will not have a corresponding menu item.")
                       .arg(elem.nodeName()).arg(elem.attribute("name")));
    fatal.append(true);
  }

  if (elem.hasAttribute("executable"))
    _executable = elem.attribute("executable");
  else
  {
    msg.append(TR("Node %1 '%2' does not name an executable. Nothing "
                           "will be done when the user selects the menu item.")
                       .arg(elem.nodeName()).arg(elem.attribute("name")));
    fatal.append(true);
  }

  if (elem.hasAttribute("privname"))
    _privname = elem.attribute("privname");

  if (elem.hasAttribute("grade"))
  {
    msg.append(TR("Node %1 '%2' has a 'grade' attribute but Custom "
                      "Commands are not graded. This will be ignored.")
                       .arg(elem.nodeName()).arg(elem.attribute("name")));
    fatal.append(false);
  }

  for (QDomElement childElem = elem.firstChildElement("arg");
       ! childElem.isNull();
       childElem = childElem.nextSiblingElement("arg"))
  {
    if (childElem.hasAttribute("value"))
      _args.append(childElem.attribute("value").trimmed());
    else
    {
      msg.append(TR("Node %1 '%2' has an 'arg' with no 'value' attribute.")
                         .arg(elem.nodeName()).arg(elem.attribute("name")));
      fatal.append(false);
    }
  }

  if (DEBUG)
    qDebug("LoadCmd(QDomElement): %s %s, comment %s",
           qPrintable(_executable), qPrintable(_args.join(" ")),
           qPrintable(_comment));
}

QDomElement LoadCmd::createElement(QDomDocument &doc)
{
  QDomElement elem = doc.createElement("loadcmd");
  elem.setAttribute("name",        _name);
  elem.setAttribute("title",       _title);
  elem.setAttribute("executable",  _executable);
  elem.setAttribute("privname",    _privname);
  elem.setAttribute("module",      _module);

  if(!_comment.isEmpty())
    elem.appendChild(doc.createTextNode(_comment));

  if (! _args.isEmpty())
  {
    for (int i = 0; i < _args.size(); i++)
    {
      QDomElement argElem = doc.createElement("arg");
      elem.setAttribute("value", _args.at(i));
      elem.appendChild(argElem);
    }
  }

  return elem;
}

int LoadCmd::writeToDB(const QString pkgname, QString &errMsg)
{
  _selectMql = new MetaSQLQuery("SELECT cmd_id, -1, -1"
                      "  FROM <? literal(\"tablename\") ?> "
                      " WHERE (cmd_name=<? value(\"name\") ?>);");

  _updateMql = new MetaSQLQuery("UPDATE <? literal(\"tablename\") ?> "
                      "   SET cmd_module=<? value(\"module\") ?>, "
                      "       cmd_title=<? value(\"title\") ?>, "
                      "       cmd_privname=<? value(\"privname\") ?>, "
                      "       cmd_executable=<? value(\"executable\") ?>, "
                      "       cmd_descrip=<? value(\"notes\") ?> "
                      " WHERE (cmd_id=<? value(\"id\") ?>) "
                      "RETURNING cmd_id AS id;");

  _insertMql = new MetaSQLQuery("INSERT INTO <? literal(\"tablename\") ?> ("
                      "  cmd_id, cmd_module,"
                      "  cmd_title, cmd_descrip, "
                      "  cmd_privname,"
                      "  cmd_executable, cmd_name"
                      ") VALUES ("
                      "  DEFAULT, <? value(\"module\") ?>,"
                      "  <? value(\"title\") ?>, <? value(\"notes\") ?>,"
                      "  <? value(\"privname\") ?>,"
                      "  <? value(\"executable\") ?>, <? value(\"name\") ?>)"
                      " RETURNING cmd_id AS id;");

  ParameterList params;
  params.append("tablename", "cmd");
  params.append("module",    _module);
  params.append("title",     _title);
  params.append("privname",  _privname);
  params.append("executable",_executable);

  int cmdid = Loadable::writeToDB(QByteArray(), pkgname, errMsg, params);
  if (cmdid < 0)
    return cmdid;

  // alter the name of the loadable's table if necessary
  QString prefix;
  if (_schema.isEmpty()        &&   pkgname.isEmpty())
    ;   // leave it alone
  else if (_schema.isEmpty()   && ! pkgname.isEmpty())
    prefix = "pkg";
  else if ("public" == _schema &&   pkgname.isEmpty())
    ;   // leave it alone
  else if ("public" == _schema && ! pkgname.isEmpty())
    prefix = "public.";
  else if (! _schema.isEmpty() &&   pkgname.isEmpty())
    prefix = _schema + ".pkg";
  else if (! _schema.isEmpty() && ! pkgname.isEmpty())
    prefix = _schema + ".pkg";

  XSqlQuery delargs;
  delargs.prepare(QString("DELETE FROM %1cmdarg WHERE (cmdarg_cmd_id=:cmd_id);")
                          .arg(prefix));
  delargs.bindValue(":cmd_id", cmdid);
  if (! delargs.exec())
  {
    QSqlError err = delargs.lastError();
    errMsg = _sqlerrtxt.arg(_name).arg(err.driverText()).arg(err.databaseText());
    return -8;
  }

  if (_args.size() > 0)
  {
    XSqlQuery insargs;
    insargs.prepare(QString("INSERT INTO %1cmdarg (cmdarg_cmd_id, cmdarg_order, "
                            "cmdarg_arg) VALUES (:cmd_id, :order, :arg);")
                          .arg(prefix));
    for (int i = 0; i < _args.size(); i++)
    {
      insargs.bindValue(":cmd_id", cmdid);
      insargs.bindValue(":order",  i);
      insargs.bindValue(":arg",    _args.at(i));
      if (! insargs.exec())
      {
        QSqlError err = insargs.lastError();
        errMsg = _sqlerrtxt.arg(_name).arg(err.driverText()).arg(err.databaseText());
        return -9;
      }
    }
  }

  return cmdid;
}
