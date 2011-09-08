/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __LOADMETASQL_H__
#define __LOADMETASQL_H__

#include "loadable.h"

class LoadMetasql : public Loadable
{
  public:
    LoadMetasql(const QString & name, const QString &group,
                  const bool system = false,
                  const QString & comment = QString::null,
                  const QString &filename = QString::null);
    LoadMetasql(const QDomElement &, const bool system,
                QStringList &, QList<bool> &);

    virtual QString group()   const { return _group; }
    virtual bool    isValid() const { return !_nodename.isEmpty() &&
                                             !_name.isEmpty() &&
                                             !_group.isEmpty(); }
    virtual void    setGroup(const QString &group) { _group = group; }

    virtual int writeToDB(const QByteArray &, const QString pkgname, QString &);

  protected:
    QString _group;
};

#endif
