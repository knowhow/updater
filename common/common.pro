#
# This file is part of the xTuple ERP: PostBooks Edition, a free and
# open source Enterprise Resource Planning software suite,
# Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
# It is licensed to you under the Common Public Attribution License
# version 1.0, the full text of which (including xTuple-specific Exhibits)
# is available at www.xtuple.com/CPAL.  By using this software, you agree
# to be bound by its terms.
#

include( ../global.pri )

TEMPLATE = lib
CONFIG += qt warn_on thread staticlib

TARGET = updatercommon
DESTDIR = ../lib
OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_SOURCES_DIR = tmp

FORMS   = licensewindow.ui

HEADERS = data.h \
          package.h \
          createdbobj.h \
          createfunction.h \
          createtable.h \
          createtrigger.h \
          createview.h \
          finalscript.h \
          initscript.h \
          script.h \
          licensewindow.h \
          loadable.h \
          loadappscript.h \
          loadappui.h \
          loadcmd.h \
          loadimage.h \
          loadmetasql.h \
          loadpriv.h \
          loadreport.h \
          pkgschema.h \
          prerequisite.h \
          xversion.h

SOURCES = data.cpp \
          package.cpp \
          createdbobj.cpp \
          createfunction.cpp \
          createtable.cpp \
          createtrigger.cpp \
          createview.cpp \
          finalscript.cpp \
          initscript.cpp \
          script.cpp \
          loadable.cpp \
          loadappscript.cpp \
          loadappui.cpp \
          loadcmd.cpp \
          loadimage.cpp \
          loadmetasql.cpp \
          loadpriv.cpp \
          loadreport.cpp \
          pkgschema.cpp \
          prerequisite.cpp \
          xversion.cpp

QT += xml sql
