/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef QUERYEDITOR_H
#define QUERYEDITOR_H

#include <QtGui/QDialog>

#include "ui_queryeditor.h"

class QueryEditor : public QDialog, public Ui::QueryEditor
{
    Q_OBJECT

public:
    QueryEditor(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~QueryEditor();

protected slots:
    virtual void languageChange();

};

#endif // QUERYEDITOR_H
