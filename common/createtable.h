/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __CREATETABLE_H__
#define __CREATETABLE_H__

#include "createdbobj.h"

class CreateTable : public CreateDBObj
{
  public:
    CreateTable(const QString &filename, const QString &name,
                const QString &comment = QString::null,
                const QString &schema  = QString::null,
                const OnError onError = Default);
    CreateTable(const QDomElement &, QStringList &, QList<bool> &);

    virtual int writeToDB(const QByteArray &, const QString pkgname, QString &);

  protected:
    QString _relkind;
};

#endif
