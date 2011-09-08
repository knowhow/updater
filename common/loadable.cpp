/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "loadable.h"

#include <QDomDocument>
#include <QRegExp>
#include <QSqlError>
#include <QVariant>     // used by XSqlQuery::value()
#include <limits.h>

#include "xsqlquery.h"

QRegExp Loadable::trueRegExp("^t(rue)?$",   Qt::CaseInsensitive);
QRegExp Loadable::falseRegExp("^f(alse)?$", Qt::CaseInsensitive);

QString Loadable::_sqlerrtxt = TR("The following error was "
                                  "encountered while trying to import %1 into "
                                  "the database:<br><pre>%2<br>%3</pre>");

Loadable::Loadable(const QString &nodename, const QString &name,
                   const int grade, const bool system, const QString &schema,
                   const QString &comment,
                   const QString &filename)
{
  _nodename = nodename;
  _name     = name;
  _grade    = grade;
  _system   = system;
  _schema   = schema;
  _comment  = comment;
  _filename = (filename.isEmpty() ? name : filename);

  _minMql    = 0;
  _maxMql    = 0;
  _selectMql = 0;
  _insertMql = 0;
  _updateMql = 0;
}

Loadable::Loadable(const QDomElement & elem, const bool system,
                   QStringList &/*msg*/, QList<bool> &/*fatal*/)
{
  _system = system;
  _nodename = elem.nodeName();
  _grade = 0;

  if (elem.hasAttribute("name"))
    _name   = elem.attribute("name");

  if (elem.hasAttribute("grade"))
  {
    if (elem.attribute("grade").contains("highest", Qt::CaseInsensitive))
      _grade = INT_MAX;
    else if (elem.attribute("grade").contains("lowest", Qt::CaseInsensitive))
      _grade = INT_MIN;
    else
      _grade = elem.attribute("grade").toInt();
  }
  else if (elem.hasAttribute("order"))
  {
    if (elem.attribute("order").contains("highest", Qt::CaseInsensitive))
      _grade = INT_MAX;
    else if (elem.attribute("order").contains("lowest", Qt::CaseInsensitive))
      _grade = INT_MIN;
    else
      _grade = elem.attribute("order").toInt();
  }

  if (elem.hasAttribute("file"))
    _filename = elem.attribute("file");
  else
    _filename = _name;

  if (elem.hasAttribute("schema"))
    _schema = elem.attribute("schema");

  if (elem.hasAttribute("onerror"))
    _onError = Script::nameToOnError(elem.attribute("onerror"));
  else
    _onError = Script::nameToOnError("Stop");

  _comment = elem.text().trimmed();

  _minMql    = 0;
  _maxMql    = 0;
  _selectMql = 0;
  _insertMql = 0;
  _updateMql = 0;
}

Loadable::~Loadable()
{
  if (_minMql)    delete _minMql;
  if (_maxMql)    delete _maxMql;
  if (_selectMql) delete _selectMql;
  if (_insertMql) delete _insertMql;
  if (_updateMql) delete _updateMql;
}

QString Loadable::schema() const
{
  return _schema;
}

QDomElement Loadable::createElement(QDomDocument & doc)
{
  QDomElement elem = doc.createElement(_nodename);
  elem.setAttribute("name", _name);
  elem.setAttribute("grade", _grade);
  elem.setAttribute("file", _filename);
  if (! _schema.isEmpty())
    elem.setAttribute("schema", _schema);

  if(!_comment.isEmpty())
    elem.appendChild(doc.createTextNode(_comment));

  return elem;
}

int Loadable::writeToDB(const QByteArray &pdata, const QString pkgname,
                        QString &errMsg, ParameterList &params)
{
  params.append("name",   _name);
  params.append("type",   _pkgitemtype);
  params.append("source", QString(pdata));
  params.append("notes",  _comment);

  // alter the name of the loadable's table if necessary
  QString destschema = "public";
  QString prefix;
  if (_schema.isEmpty()        &&   pkgname.isEmpty())
    ;   // leave it alone
  else if (_schema.isEmpty()   && ! pkgname.isEmpty())
  {
    prefix = "pkg";
    destschema = pkgname;
  }
  else if ("public" == _schema &&   pkgname.isEmpty())
    ;   // leave it alone
  else if ("public" == _schema && ! pkgname.isEmpty())
    prefix = "public.";
  else if (! _schema.isEmpty())
  {
    prefix = _schema + ".pkg";
    destschema = _schema;
  }

  if (! prefix.isEmpty())
  {
    params.append("pkgname", destschema);

    // yuck - no Parameter::operator==(Parameter&) and no replace()
    QString tablename = params.value("tablename").toString();
    for (int i = 0; i < params.size(); i++)
    {
      if (params.at(i).name() == "tablename")
      {
        params.takeAt(i);
        params.append("tablename", prefix + tablename);
        break;
      }
    }
  }

  if (_minMql && _minMql->isValid() && _grade == INT_MIN)
  {
    XSqlQuery minOrder = _minMql->toQuery(params);
    if (minOrder.first())
      _grade = minOrder.value(0).toInt();
    else if (minOrder.lastError().type() != QSqlError::NoError)
    {
      QSqlError err = minOrder.lastError();
      errMsg = _sqlerrtxt.arg(_filename).arg(err.driverText()).arg(err.databaseText());
      return -3;
    }
    else
      _grade = 0;
  }
  else if (_maxMql && _maxMql->isValid() && _grade == INT_MAX)
  {
    XSqlQuery maxOrder = _maxMql->toQuery(params);
    if (maxOrder.first())
      _grade = maxOrder.value(0).toInt();
    else if (maxOrder.lastError().type() != QSqlError::NoError)
    {
      QSqlError err = maxOrder.lastError();
      errMsg = _sqlerrtxt.arg(_filename).arg(err.driverText()).arg(err.databaseText());
      return -4;
    }
    else
      _grade = 0;
  }

  params.append("grade", _grade);

  XSqlQuery select;
  int itemid = -1;
  select = _selectMql->toQuery(params);

  if (select.first())
    itemid = select.value(0).toInt();
  else if (select.lastError().type() != QSqlError::NoError)
  {
    QSqlError err = select.lastError();
    errMsg = _sqlerrtxt.arg(_filename).arg(err.driverText()).arg(err.databaseText());
    return -5;
  }
  params.append("id", itemid);

  XSqlQuery upsert;
  if (itemid >= 0)
    upsert = _updateMql->toQuery(params);
  else
    upsert = _insertMql->toQuery(params);

  if (upsert.first())
    itemid = upsert.value("id").toInt();
  else if (upsert.lastError().type() != QSqlError::NoError)
  {
    QSqlError err = upsert.lastError();
    errMsg = _sqlerrtxt.arg(_filename)
                .arg(err.driverText())
                .arg(err.databaseText());
    return -7;
  }

  return itemid;
}
