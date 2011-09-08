/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "data.h"

#include <QObject>

#if defined Q_WS_WIN
QString _name = QObject::tr("Update Manager for Windows");
#elif defined Q_WS_X11
QString _name = QObject::tr("Update Manager for Linux");
#elif defined Q_WS_MAC
QString _name = QObject::tr("Update Manager for Mac");
#else
QString _name = QObject::tr("Update Manager");
#endif


QString _copyright = QObject::tr("Copyright (c) 2004-2011 OpenMFG, LLC., d/b/a xTuple.");
QString _version = QObject::tr("2.2.4");

QString _user;
int     _usrid;

bool _evaluation = FALSE;
bool _loggedIn = FALSE;
