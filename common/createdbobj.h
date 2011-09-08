/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __CREATEDBOBJ_H__
#define __CREATEDBOBJ_H__

#include <QString>

#include "script.h"

#include <parameter.h>

class QDomDocument;
class QDomElement;
class MetaSQLQuery;

#define TR(a) QObject::tr(a)

class CreateDBObj : public Script
{
  public:
    CreateDBObj(const QString &nodename, const QString &filename,
                const QString &name = QString::null,
                const QString &comment = QString::null,
                const QString &schema = QString::null,
                const OnError onError = Default);
    CreateDBObj(const QDomElement &, QStringList &, QList<bool> &);

    virtual ~CreateDBObj();

    virtual QDomElement createElement(QDomDocument &doc);

    virtual QString filename() const { return _filename; }
    virtual bool    isValid()  const { return !_nodename.isEmpty() &&
                                              !_name.isEmpty() &&
                                              !_filename.isEmpty(); }

  protected:
    QString       _filename;
    QString       _nodename;
    MetaSQLQuery *_oidMql;
    QString       _pkgitemtype;
    QString       _schema;

    CreateDBObj();
    virtual int writeToDB(const QByteArray &pdata, const QString pkgname,
                          ParameterList &params, QString &errMsg);
};

#endif
