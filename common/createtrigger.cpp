/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "createtrigger.h"

#include <QDomDocument>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()

#include "metasql.h"
#include "xsqlquery.h"

#define DEBUG false

CreateTrigger::CreateTrigger(const QString &filename, const QString &name,
                             const QString &comment,  const QString &schema,
                             const OnError onError)
  : CreateDBObj("createtrigger", filename, name, comment, schema, onError)
{
  _pkgitemtype = "G";
}

CreateTrigger::CreateTrigger(const QDomElement &elem, QStringList &msg, QList<bool> &fatal)
  : CreateDBObj(elem, msg, fatal)
{
  _pkgitemtype = "G";

  if (elem.nodeName() != "createtrigger")
  {
    msg.append(TR("Creating a CreateTrigger element from a %1 node.")
              .arg(elem.nodeName()));
    fatal.append(false);
  }
}

int CreateTrigger::writeToDB(const QByteArray &pdata, const QString pkgname, QString &errMsg)
{
  if (DEBUG)
    qDebug("CreateTrigger::writeToDb(%s, %s, &errMsg)",
           pdata.data(), qPrintable(pkgname));

  _oidMql = new MetaSQLQuery("SELECT pg_trigger.oid AS oid "
                             "FROM pg_trigger, pg_class, pg_namespace "
                             "WHERE ((tgname=<? value(\"name\") ?>)"
                             "  AND  (tgrelid=pg_class.oid)"
                             "  AND  (relnamespace=pg_namespace.oid)"
                             "  AND  (nspname=<? value(\"schema\") ?>));");
  ParameterList params;
  int returnVal = CreateDBObj::writeToDB(pdata, pkgname, params, errMsg);

  delete _oidMql;
  _oidMql = 0;

  return returnVal;
}
