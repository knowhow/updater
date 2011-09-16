#
# This file is part of the xTuple ERP: PostBooks Edition, a free and
# open source Enterprise Resource Planning software suite,
# Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
# It is licensed to you under the Common Public Attribution License
# version 1.0, the full text of which (including xTuple-specific Exhibits)
# is available at www.xtuple.com/CPAL.  By using this software, you agree
# to be bound by its terms.
#

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
exists($${OPENRPT_DIR}-desktop-build) {
  OPENRPT_BLD=$${OPENRPT_DIR}-desktop-build
}

exists(../xtuple) {
  XTUPLE_DIR = ../xtuple
}
exists(../xtuple/trunk) {
  XTUPLE_DIR = ../xtuple/trunk
}
exists(../../xtuple) {
  XTUPLE_DIR = ../../xtuple
}
exists(../../xtuple/trunk) {
  XTUPLE_DIR = ../../xtuple/trunk
}

! exists($${XTUPLE_DIR}) {
  error("Could not set the XTUPLE_DIR qmake variable.")
}

XTUPLE_BLD=$${XTUPLE_DIR}
exists($${XTUPLE_DIR}-desktop-build) {
  XTUPLE_BLD=$${XTUPLE_DIR}-desktop-build
}

TEMPLATE = subdirs
SUBDIRS = common \
          builder \
	  loader
