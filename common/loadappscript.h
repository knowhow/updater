/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __LOADAPPSCRIPT_H__
#define __LOADAPPSCRIPT_H__

#include "loadable.h"

class LoadAppScript : public Loadable
{
  public:
    LoadAppScript(const QString & name, const int order = 0,
                  const bool system = false, const bool enabled = true,
                  const QString & comment = QString::null,
                  const QString &filename = QString::null);
    LoadAppScript(const QDomElement &, const bool system,
                  QStringList &, QList<bool> &);

    virtual int writeToDB(const QByteArray &, const QString pkgname, QString &);

  protected:
    bool _enabled;
};

#endif
