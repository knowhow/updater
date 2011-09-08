/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef PACKAGEWINDOW_H
#define PACKAGEWINDOW_H

#include <Q3MainWindow>
#include <package.h>

#include "ui_packagewindow.h"

class PackageWindow : public Q3MainWindow, public Ui::PackageWindow
{
    Q_OBJECT

public:
    PackageWindow(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~PackageWindow();

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void fileExit();
    virtual void helpIndex();
    virtual void helpContents();
    virtual void helpAbout();

protected:
    Package * _package;
    QString _filename;

protected slots:
    virtual void languageChange();

    virtual void sPrereqSelectionChanged();
    virtual void sAddPrereq();
    virtual void sRaisePrereq();
    virtual void sLowerPrereq();
    virtual void sRemovePrereq();
    virtual void sEditConditions();
    virtual void sPrereqTextChanged();
    virtual void sEditPrereqMessage();
    virtual void sProviderSelectionChanged();
    virtual void sAddProvider();
    virtual void sEditProvider();
    virtual void sRemoveProvider();
    virtual void sScriptSelectionChanged();
    virtual void sAddScript();
    virtual void sRaiseScript();
    virtual void sLowerScript();
    virtual void sRemoveScript();
    virtual void sOnErrorActivated( const QString & string );
    virtual void sScriptTextChanged();
    virtual void sEditScriptMessage();
    virtual void sReportSelectionChanged();
    virtual void sAddReport();
    virtual void sRemoveReport();
    virtual void sGradeChanged( int value );
    virtual void sReportTextChanged();
    virtual void sEditReportMessage();

};

#endif // PACKAGEWINDOW_H
