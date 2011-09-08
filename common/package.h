/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __PACKAGE_H__
#define __PACKAGE_H__

#include <QString>
#include <QList>

#include "xversion.h"

class QDomDocument;
class QDomElement;

class CreateFunction;
class CreateTable;
class CreateTrigger;
class CreateView;
class LoadAppScript;
class LoadAppUI;
class LoadCmd;
class LoadImage;
class LoadMetasql;
class LoadPriv;
class LoadReport;
class Prerequisite;
class Script;
class FinalScript;
class InitScript;

class Package
{
  public:
    Package(const QString & id = QString::null);
    Package(const QDomElement &, QStringList &, QList<bool> &);

    virtual ~Package();

    QDomElement createElement(QDomDocument &); 
    int writeToDB(QString &errMsg);

    QString id() const { return _id; }
    void setId(const QString & id) { _id = id; }

    QString developer() const { return _developer; }
    QString name()      const { return _name; }
    bool     system()   const;
    XVersion version()  const { return _pkgversion; }

    QList<CreateFunction*> _functions;
    QList<CreateTable*>    _tables;
    QList<CreateTrigger*>  _triggers;
    QList<CreateView*>     _views;
    QList<LoadAppScript*>  _appscripts;
    QList<LoadAppUI*>      _appuis;
    QList<LoadCmd*>        _cmds;
    QList<LoadImage*>      _images;
    QList<LoadMetasql*>    _metasqls;
    QList<LoadPriv*>       _privs;
    QList<Prerequisite*>   _prerequisites;
    QList<Script*>         _scripts;
    QList<FinalScript*>    _finalscripts;
    QList<InitScript*>     _initscripts;
    QList<LoadReport*>     _reports;

    bool containsAppScript(const QString &name)    const;
    bool containsAppUI(const QString &name)        const;
    bool containsCmd(const QString &name)          const;
    bool containsFunction(const QString &name)     const;
    bool containsImage(const QString &name)        const;
    bool containsPrerequisite(const QString &name) const;
    bool containsMetasql(const QString &name)      const;
    bool containsPriv(const QString &name)         const;
    bool containsReport(const QString &name)       const;
    bool containsScript(const QString &name)       const;
    bool containsFinalScript(const QString &name)  const;
    bool containsInitScript(const QString &name)  const;
    bool containsTable(const QString &name)        const;
    bool containsTrigger(const QString &name)      const;
    bool containsView(const QString &name)         const;

  protected:
    QString     _developer;
    QString     _descrip;
    QString     _id;
    XVersion    _pkgversion;
    QString     _name;
    QString     _notes;
};

#endif
