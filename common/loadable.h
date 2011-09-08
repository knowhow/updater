/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __LOADABLE_H__
#define __LOADABLE_H__

#include <QString>
#include <QStringList>

#include "script.h"

#include <metasql.h>

class QDomDocument;
class QDomElement;

#define TR(a) QObject::tr(a)

class Loadable
{
  public:
    Loadable(const QString &nodename, const QString &name,
             const int grade = 0, const bool system = false,
             const QString &schema  = QString::null,
             const QString &comment = QString::null,
             const QString &filename = QString::null);
    Loadable(const QDomElement &, const bool system, QStringList &, QList<bool> &);
    virtual ~Loadable();

    virtual QDomElement createElement(QDomDocument &doc);

    virtual QString comment()  const { return _comment; }
    virtual QString filename() const { return _filename; }
    virtual int     grade()    const { return _grade; }
    virtual bool    isValid()  const { return !_nodename.isEmpty() &&
                                              !_name.isEmpty();}
    virtual QString name()     const { return _name; }
    virtual QString nodename() const { return _nodename; }
    virtual Script::OnError onError() const { return _onError; }
    virtual QString schema()   const;
    virtual void    setComment(const QString & comment) { _comment  = comment; }
    virtual void    setFilename(const QString &filename){ _filename = filename;}
    virtual void    setGrade(int grade)                 { _grade = grade; }
    virtual void    setName(const QString & name)       { _name = name; }
    virtual void    setOnError(Script::OnError onError) { _onError = onError; }
    virtual void    setSystem(const bool p)             { _system = p; }
    virtual bool    system()   const { return _system; }
    virtual int writeToDB(const QByteArray &pdata, const QString pkgname,
                          QString &errMsg) = 0;

    static QRegExp trueRegExp;
    static QRegExp falseRegExp;

  protected:

    QString      _comment;
    QString      _filename;
    int          _grade;
    bool         _inpackage;
    MetaSQLQuery *_insertMql;
    MetaSQLQuery *_selectMql;
    MetaSQLQuery *_maxMql;
    MetaSQLQuery *_minMql;
    QString      _name;
    QString      _nodename;
    Script::OnError _onError;
    QString      _pkgitemtype;
    QString      _schema;
    bool         _system;
    MetaSQLQuery *_updateMql;

    virtual int writeToDB(const QByteArray &pdata, const QString pkgname,
                          QString &errMsg, ParameterList &params);

    static QString      _sqlerrtxt;
};

#endif
