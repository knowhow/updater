/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __LOADPRIV_H__
#define __LOADPRIV_H__

#include "loadable.h"

class LoadPriv : public Loadable
{
  public:
    LoadPriv(const QString &nodename,
             const QString &name, const QString &module = "Custom",
             const bool system = false, const QString &descrip = QString::null);
    LoadPriv(const QDomElement &, const bool system,
             QStringList &, QList<bool> &);

    virtual QDomElement createElement(QDomDocument &doc);

    virtual bool isValid()  const { return !_name.isEmpty() && !_module.isEmpty(); }

    virtual int writeToDB(const QByteArray &, const QString pkgname, QString &errMsg) { return writeToDB(pkgname, errMsg); }
    virtual int writeToDB(const QString pkgname, QString &errMsg);

  protected:
    QString _module;
};

#endif
