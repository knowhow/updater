/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef __INITSCRIPT_H__
#define __INITSCRIPT_H__

#include "script.h"

class InitScript : public Script
{
  public:
    InitScript(const QString & name = QString::null, OnError onError = Default,
           const QString & comment = QString::null);
    InitScript(const QDomElement &, QStringList &msg, QList<bool> &fatal);

    virtual ~InitScript();

    virtual QDomElement createElement(QDomDocument &);

  protected:
    QString _nodename;
};

#endif

