/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef TEXTEDITDIALOG_H
#define TEXTEDITDIALOG_H

#include <QtGui/QDialog>

#include "ui_texteditdialog.h"

class TextEditDialog : public QDialog, public Ui::TextEditDialog
{
    Q_OBJECT

public:
    TextEditDialog(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~TextEditDialog();

protected slots:
    virtual void languageChange();

};

#endif // TEXTEDITDIALOG_H
