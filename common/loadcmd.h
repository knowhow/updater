/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __LOADCMD_H__
#define __LOADCMD_H__

#include <QStringList>
#include "loadable.h"

class LoadCmd : public Loadable
{
  public:
    LoadCmd(const QString &nodename, const QString &name,
            const QString &module, const QString &title,
            const QString &privname, const QString &executable,
            const QStringList &args = QStringList(),
            const QString &descrip = QString::null);
    LoadCmd(const QDomElement &, const bool system,
            QStringList &, QList<bool> &);

    virtual QDomElement createElement(QDomDocument &doc);

    virtual bool isValid()  const { return !_name.isEmpty()
                                        && !_title.isEmpty()
                                        && !_executable.isEmpty(); }

    virtual int writeToDB(const QByteArray &, const QString pkgname,
                          QString &errMsg) { return writeToDB(pkgname, errMsg); }
    virtual int writeToDB(const QString pkgname, QString &errMsg);

  protected:
    QStringList _args;
    QString     _executable;
    QString     _module;
    QString     _privname;
    QString     _title;
};

#endif
