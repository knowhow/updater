#
# This file is part of the xTuple ERP: PostBooks Edition, a free and
# open source Enterprise Resource Planning software suite,
# Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
# It is licensed to you under the Common Public Attribution License
# version 1.0, the full text of which (including xTuple-specific Exhibits)
# is available at www.xtuple.com/CPAL.  By using this software, you agree
# to be bound by its terms.
#

exists(../../../openrpt) {
  OPENRPT_DIR = ../../../openrpt
}
exists(../../openrpt) {
  OPENRPT_DIR = ../../openrpt
}
exists(../openrpt) {
  OPENRPT_DIR = ../openrpt
}

! exists($${OPENRPT_DIR}) {
  error("Could not set the OPENRPT_DIR qmake variable.")
}

OPENRPT_BLD=$${OPENRPT_DIR}
exists($${OPENRPT_DIR}-build-desktop) {
  OPENRPT_BLD=$${OPENRPT_DIR}-build-desktop
}

exists(../../../xtuple) {
  XTUPLE_DIR = ../../xtuple
}
exists(../../../xtuple/trunk) {
  XTUPLE_DIR = ../../xtuple/trunk
}
exists(../../xtuple) {
  XTUPLE_DIR = ../../xtuple
}
exists(../../xtuple/trunk) {
  XTUPLE_DIR = ../../xtuple/trunk
}
exists(../xtuple) {
  XTUPLE_DIR = ../xtuple
}
exists(../xtuple/trunk) {
  XTUPLE_DIR = ../xtuple/trunk
}

XTUPLE_BLD=$${XTUPLE_DIR}
exists($${XTUPLE_DIR}-build-desktop) {
  XTUPLE_BLD=$${XTUPLE_DIR}-build-desktop
}

UPDATER_BLD=.
exists(../updater-desktop-build) {
  UPDATER_BLD=../updater-desktop-build
}

message("Looking for OpenRPT code in $${OPENRPT_DIR}.")
message("Looking for xTuple code in $${XTUPLE_DIR}.")

INCLUDEPATH += ../common \
               ../$${OPENRPT_DIR}/common \
               ../$${OPENRPT_DIR}/MetaSQL \
               ../$${XTUPLE_DIR}/common  ../$${XTUPLE_BLD}/common
DEPENDPATH  += ../$${UPDATER_BLD}/common ../$${OPENRPT_BLD}/common ../$${OPENRPT_BLD}/MetaSQL

INCLUDEPATH = $$unique(INCLUDEPATH)

CONFIG += release

macx:exists(macx.pri) {
  include(macx.pri)
}

win32:exists(win32.pri) {
  include(win32.pri)
}

unix:exists(unix.pri) {
  include(unix.pri)
}
