/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#ifndef LOADERWINDOW_H
#define LOADERWINDOW_H

class Loadable;
class Package;
class Script;
class TarFile;

#include <QMainWindow>

#include "ui_loaderwindow.h"

class LoaderWindow : public QMainWindow, public Ui::LoaderWindow
{
    Q_OBJECT

public:
    LoaderWindow(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window);
    ~LoaderWindow();

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileExit();
    virtual void helpContents();
    virtual void helpAbout();

    virtual void setMultipleTransactions(bool);
    virtual void setDebugPkg(bool);
    virtual bool openFile(QString filename);
    virtual void setWindowTitle();
    virtual bool sStart();

protected:
    Package * _package;
    TarFile * _files;

    virtual int  applySql(Script *, const QByteArray);
    virtual int  applyLoadable(Loadable *, const QByteArray);
    virtual void launchBrowser(QWidget *w, const QString &url);
    virtual void timerEvent( QTimerEvent * e );

    static QString _rollbackMsg;

protected slots:
    virtual void languageChange();

private:
    int _dbTimerId;
    bool _multitrans;
    bool _premultitransfile;
    QStringList _triggers;      // to be disabled and enabled

    int disableTriggers();
    int enableTriggers();

};

#endif // LOADERWINDOW_H
