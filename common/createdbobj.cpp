/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "createdbobj.h"

#include <QDomDocument>
#include <QMessageBox>
#include <QRegExp>
#include <QSqlError>
#include <QVariant>

#include "metasql.h"
#include "xsqlquery.h"

#define DEBUG false

CreateDBObj::CreateDBObj()
{
}

CreateDBObj::CreateDBObj(const QString &nodename, const QString &filename,
                         const QString &name,     const QString &comment,
                         const QString &schema,   OnError onError)
{
  _comment  = comment;
  _filename = filename;
  _name     = name;
  _nodename = nodename;
  _oidMql   = 0;
  _schema   = schema;
  _onError  = onError;
}

CreateDBObj::CreateDBObj(const QDomElement & elem, QStringList &msg, QList<bool> &fatal)
{
  _nodename = elem.nodeName();

  if (elem.hasAttribute("name"))
    _name = elem.attribute("name");
  else
  {
    msg.append(TR("The contents.xml must name the object for %1.")
               .arg(_nodename));
    fatal.append("false");
  }

  if (elem.hasAttribute("file"))
    _filename = elem.attribute("file");
  else
  {
    msg.append(TR("The contents.xml must name the file for %1.")
               .arg(_nodename));
    fatal.append(true);
  }

  if (elem.hasAttribute("schema"))
    _schema = elem.attribute("schema");

  if (elem.hasAttribute("onerror"))
    _onError = nameToOnError(elem.attribute("onerror"));

  _comment = elem.text().trimmed();
}

CreateDBObj::~CreateDBObj()
{
}

QDomElement CreateDBObj::createElement(QDomDocument & doc)
{
  QDomElement elem = doc.createElement(_nodename);

  elem.setAttribute("file", _filename);

  if (! _name.isEmpty())
    elem.setAttribute("name", _name);

  if (! _schema.isEmpty())
    elem.setAttribute("schema", _schema);

  if (!_comment.isEmpty())
    elem.appendChild(doc.createTextNode(_comment));

  return elem;
}

int CreateDBObj::writeToDB(const QByteArray &pdata, const QString pkgname,
                           ParameterList &params, QString &errMsg)
{
  if (DEBUG)
    qDebug("CreateDBObj::writeToDB(%s, %s, &errMsg)",
           pdata.data(), qPrintable(pkgname));

  QString destschema;
  if (! _schema.isEmpty())
    destschema = _schema;
  else if (pkgname.isEmpty())
    destschema = "public";
  else if (! pkgname.isEmpty())
    destschema = pkgname;

  int returnVal = Script::writeToDB(pdata, pkgname, errMsg);
  if (returnVal < 0)
    return returnVal;

  params.append("name", _name);
  params.append("schema", destschema);

  XSqlQuery oidq = _oidMql->toQuery(params);
  if (oidq.first())
    ; // passed error check
  else if (oidq.lastError().type() != QSqlError::NoError)
  {
    errMsg = _sqlerrtxt.arg(_filename)
                       .arg(oidq.lastError().databaseText())
                       .arg(oidq.lastError().driverText());
    return -7;
  }
  else // not found
  {
    errMsg = TR("Could not find %1 in the database. The "
                "script %2 does not match the package.xml description.")
            .arg(_name).arg(_filename);
    return -8;
  }

  return returnVal;
}
