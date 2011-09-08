/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "createview.h"

#include <QDomDocument>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()

#include "metasql.h"
#include "xsqlquery.h"

#define DEBUG false

CreateView::CreateView(const QString &filename, const QString &name,
                       const QString &comment,  const QString &schema,
                       const OnError onError)
  : CreateDBObj("createview", filename, name, comment, schema, onError)
{
  _pkgitemtype = "V";
  _relkind     = "v";
}

CreateView::CreateView(const QDomElement &elem, QStringList &msg, QList<bool> &fatal)
  : CreateDBObj(elem, msg, fatal)
{
  _pkgitemtype = "V";
  _relkind     = "v";

  if (elem.nodeName() != "createview")
  {
    msg.append(TR("Creating a CreateView element from a %1 node.")
              .arg(elem.nodeName()));
    fatal.append(false);
  }
}

int CreateView::writeToDB(const QByteArray &pdata, const QString pkgname, QString &errMsg)
{
  if (DEBUG)
    qDebug("CreateView::writeToDb(%s, %s, &errMsg)",
           pdata.data(), qPrintable(pkgname));

  _oidMql = new MetaSQLQuery("SELECT pg_class.oid AS oid "
                             "FROM pg_class, pg_namespace "
                             "WHERE ((relname=<? value(\"name\") ?>)"
                             "  AND  (relkind=<? value(\"relkind\") ?>)"
                             "  AND  (relnamespace=pg_namespace.oid)"
                             "  AND  (nspname=<? value(\"schema\") ?>));");
  ParameterList params;
  params.append("relkind", _relkind);

  int returnVal = CreateDBObj::writeToDB(pdata, pkgname, params, errMsg);

  delete _oidMql;
  _oidMql = 0;

  return returnVal;
}
