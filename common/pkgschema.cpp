/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "pkgschema.h"

#include <QMessageBox>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::bindValue()

#include <xsqlquery.h>

#define TR(a) QObject::tr(a)

#define DEBUG false

QString PkgSchema::_sqlerrtxt = TR("<font color=red>The following error was "
                                  "encountered while trying to import %1 into "
                                  "the database:<br>%2<br>%3</font>");

PkgSchema::PkgSchema(const QString &name, const QString &comment)
{
  _name    = name;
  _comment = comment;
}

PkgSchema::~PkgSchema()
{
}

int PkgSchema::create(QString &errMsg)
{
  if (DEBUG)
    qDebug("PkgSchema::create(&errMsg)");

  if (_name.isEmpty())
  {
    errMsg = QString("Cannot create a schema for this package without a name.");
    return -1;
  }

  int namespaceoid;
  XSqlQuery create;
  // Issue 8835: This SQL looks for an existing schema first.  Would be best if the createPkgSchema 
  // function did this correctly so it could be smart enough to add new tables if necessary,
  // but we can't work that out until it becomes required in a subsequent xTuple ERP release.
  create.prepare("SELECT COALESCE( "
                 " (SELECT oid "
                 "  FROM pg_namespace "
                 "  WHERE (LOWER(nspname)=LOWER(:name))), "
                 " createPkgSchema(:name, :descrip)) AS result;"); 
  create.bindValue(":name",    _name);
  create.bindValue(":descrip", _comment);

  create.exec();
  if (create.first())
    namespaceoid = create.value(0).toInt();
  else if (create.lastError().type() != QSqlError::NoError)
  {
    errMsg = _sqlerrtxt.arg(_name)
                      .arg(create.lastError().databaseText())
                      .arg(create.lastError().driverText());
    return -2;
  }

  int patherr = setPath(errMsg);
  if (patherr < 0)
    return patherr;

  return 0;
}

int PkgSchema::getPath(QString &path, QString &errMsg)
{
  XSqlQuery pathq;
  pathq.exec("SELECT CURRENT_SCHEMAS(false);");
  if (pathq.first())
    path = pathq.value(0).toString();
  else
  {
    errMsg = _sqlerrtxt.arg(_name)
                      .arg(pathq.lastError().databaseText())
                      .arg(pathq.lastError().driverText());
    return -8;
  }

  if (DEBUG)
    qDebug("PkgSchema::getPath() selected %s", qPrintable(path));

  path.remove(0, path.indexOf("{") + 1);
  path.remove(path.indexOf("}"), path.size());

  if (DEBUG)
    qDebug("PkgSchema::getPath() extracted %s", qPrintable(path));

  return 0;
}

int PkgSchema::setPath(QString &errMsg)
{
  QString path;
  int result = getPath(path, errMsg);
  if (result < 0)
    return result;

  XSqlQuery schemaq;
  schemaq.exec(QString("SET SEARCH_PATH TO %1,%2;")
                 .arg(_name.toLower()).arg(path));
  if (schemaq.lastError().type() != QSqlError::NoError)
  {
    errMsg = _sqlerrtxt.arg(_name)
                      .arg(schemaq.lastError().databaseText())
                      .arg(schemaq.lastError().driverText());
    return -8;
  }

  return 0;
}

int PkgSchema::clearPath(QString &errMsg)
{
  QString path;
  int result = getPath(path, errMsg);
  if (result < 0)
    return result;

  path.remove(QRegExp("\\s*" + _name + ",", Qt::CaseInsensitive));

  XSqlQuery schemaq;
  schemaq.exec(QString("SET SEARCH_PATH TO %1;").arg(path));
  if (schemaq.lastError().type() != QSqlError::NoError)
  {
    errMsg = _sqlerrtxt.arg(_name)
                      .arg(schemaq.lastError().databaseText())
                      .arg(schemaq.lastError().driverText());
    return -9;
  }

  return 0;
}
