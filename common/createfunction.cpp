/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "createfunction.h"

#include <QDomDocument>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()

#include "xsqlquery.h"

#define DEBUG false

CreateFunction::CreateFunction(const QString &filename, 
                               const QString &name, const QString &comment,
                               const QString &schema, const OnError onError)
  : CreateDBObj("createfunction", filename, name, comment, schema, onError)
{
  _pkgitemtype = "F";
}

CreateFunction::CreateFunction(const QDomElement &elem, QStringList &msg, QList<bool> &fatal)
  : CreateDBObj(elem, msg, fatal)
{
  _pkgitemtype = "F";

  if (elem.nodeName() != "createfunction")
  {
    msg.append(TR("Creating a CreateFunction element from a %1 node.")
              .arg(elem.nodeName()));
    fatal.append(false);
  }
}

int CreateFunction::writeToDB(const QByteArray &pdata, const QString pkgname,
                              QString &errMsg)
{
  if (DEBUG)
    qDebug("CreateFunction::writeToDb(%s, %s, &errMsg)",
           pdata.data(), qPrintable(pkgname));

  QString destschema;
  if (! _schema.isEmpty())
    destschema = _schema;
  else if (pkgname.isEmpty())
    destschema = "public";
  else if (! pkgname.isEmpty())
    destschema = pkgname;

  XSqlQuery oidq;
  QMap<QString,int> oldoids;

  oidq.prepare("SELECT pg_proc.oid, oidvectortypes(proargtypes) "
               "FROM pg_proc, pg_namespace "
               "WHERE ((pg_namespace.oid=pronamespace)"
               "  AND  (proname=:name)"
               "  AND  (nspname=:schema));");
  oidq.bindValue(":name",   _name);
  oidq.bindValue(":schema", destschema);

  oidq.exec();
  while (oidq.next())
    oldoids.insert(oidq.value(1).toString(), oidq.value(0).toInt());

  if (oidq.lastError().type() != QSqlError::NoError)
  {
    errMsg = _sqlerrtxt.arg(_filename)
                      .arg(oidq.lastError().databaseText())
                      .arg(oidq.lastError().driverText());
    return -1;
  }
  if (DEBUG)
  {
    QMap<QString, int>::const_iterator i = oldoids.constBegin();
    while (i != oldoids.constEnd())
    {
      qDebug("CreateFunction::writeToDB() %s(%s) -> %d",
             qPrintable(_name), qPrintable(i.key()), i.value());
      i++;
    }
  }

  int returnVal = Script::writeToDB(pdata, pkgname, errMsg);
  if (returnVal < 0)
    return returnVal;

  oidq.exec();        // reuse the query
  int count = 0;
  while (oidq.next())
  {
    // error checking - if none found then there's a bug somewhere
    if (DEBUG)
      qDebug("CreateFunction::writeToDB() oid = %d, argtypes = %s",
             oidq.value(0).toInt(), qPrintable(oidq.value(1).toString()));
    count++;
  }
  if (oidq.lastError().type() != QSqlError::NoError)
  {
    errMsg = _sqlerrtxt.arg(_filename)
                      .arg(oidq.lastError().databaseText())
                      .arg(oidq.lastError().driverText());
    return -5;
  }
  if (count == 0)
  {
    errMsg = TR("Could not find function %1 in the database for package %2. "
                "The script %3 does not match the package.xml description.")
              .arg(_name).arg(pkgname).arg(_filename);
    return -6;
  }

  return 0;
}
