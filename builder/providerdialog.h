/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef PROVIDERDIALOG_H
#define PROVIDERDIALOG_H

#include <QDialog>

#include "ui_providerdialog.h"

class ProviderDialog : public QDialog, public Ui::ProviderDialog
{
    Q_OBJECT

public:
    ProviderDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~ProviderDialog();

protected slots:
    virtual void languageChange();

};

#endif // PROVIDERDIALOG_H
