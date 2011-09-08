/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __PKGSCHEMA_H__
#define __PKGSCHEMA_H__

#include <QString>

class PkgSchema
{
  public:
    PkgSchema(const QString &name, const QString &comment = QString::null);
    virtual ~PkgSchema();

    virtual int create(QString &errMsg);
    virtual int clearPath(QString &errMsg);
    virtual int getPath(QString &path, QString &errMsg);
    virtual int setPath(QString &errMsg);

  protected:
    QString _name;
    QString _comment;

    static QString _sqlerrtxt;
};

#endif
