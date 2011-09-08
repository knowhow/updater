/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef NEWPREREQDIALOG_H
#define NEWPREREQDIALOG_H

#include <QDialog>

#include "ui_newprereqdialog.h"

class NewPrereqDialog : public QDialog, public Ui::NewPrereqDialog
{
    Q_OBJECT

public:
    NewPrereqDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~NewPrereqDialog();

protected slots:
    virtual void languageChange();

    virtual void sAccept();

};

#endif // NEWPREREQDIALOG_H
