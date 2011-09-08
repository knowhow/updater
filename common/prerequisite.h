/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __PREREQUISITE_H__
#define __PREREQUISITE_H__

#include <QString>
#include <QList>
#include <QStringList>

class QDomDocument;
class QDomElement;

class PrerequisiteProvider
{
  public:
    PrerequisiteProvider(const QString & package = QString::null,
                         const QString & info = QString::null);
    PrerequisiteProvider(const QDomElement &);
    virtual ~PrerequisiteProvider();

    QDomElement createElement(QDomDocument &);

    bool isValid() const { return !_package.isEmpty(); }

    QString package() const { return _package; }
    void setPackage(const QString & package) { _package = package; }

    QString info() const { return _info; }
    void setInfo(const QString & info) { _info = info; }

  protected:
    QString _package;
    QString _info;
};

class DependsOn
{
  public:
    DependsOn(const QString &name, const QString &version = QString(),
               const QString &developer = QString())
    {
      _name      = name;
      _version   = version;
      _developer = developer;
    };

    QString developer() { return _developer; };
    QString name()      { return _name; };
    QString version()   { return _version; };

  private:
    QString _developer;
    QString _name;
    QString _version;
};

class Prerequisite
{
  public:
    Prerequisite();
    Prerequisite(const Prerequisite &);
    Prerequisite(const QDomElement &);
    virtual ~Prerequisite();

    QDomElement createElement(QDomDocument &);

    enum Type
    {
      None = 0,
      Query,
      License,
      Dependency
    };

    virtual bool met(QString &);
    virtual int  writeToDB(const QString, QString &);

    QString name() const { return _name; }
    void setName(const QString & name) { _name = name; }

    Type type() const { return _type; }
    void setType(Type type) { _type = type; }

    QString message() const { return _message; }
    void setMessage(const QString & message) { _message = message; }

    QString query() const { return _query; }
    void setQuery(const QString & query) { _query = query; }

    void setProvider(const PrerequisiteProvider &);
    bool removeProvider(const QString &);
    PrerequisiteProvider provider(const QString &) const;
    QStringList providerList() const;

    static QString typeToName(Type);
    static Type nameToType(const QString &);
    static QStringList typeList(bool includeNone = TRUE);

  protected:
    DependsOn  *_dependency;
    QString    _message;
    QString    _name;
    QString    _query;
    Type       _type;
    
    QList<PrerequisiteProvider> _providers;

    static QString _sqlerrtxt;
};

#endif

