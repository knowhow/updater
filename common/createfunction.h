/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __CREATEFUNCTION_H__
#define __CREATEFUNCTION_H__

#include "createdbobj.h"

class CreateFunction : public CreateDBObj
{
  public:
    CreateFunction(const QString &filename, const QString &name,
                   const QString &comment = QString::null,
                   const QString &schema = QString::null,
                   const OnError onError = Default);
    CreateFunction(const QDomElement &, QStringList &, QList<bool> &);

    virtual int writeToDB(const QByteArray &, const QString pkgname, QString &);

  protected:
};

#endif
