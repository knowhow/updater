/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "packagewindow.h"

#include <QApplication>
#include <QDomDocument>
#include <QFileDialog>
#include <QLineEdit>
#include <QList>
#include <QMessageBox>
#include <QTextStream>
#include <QVariant>

#include <loadreport.h>
#include <prerequisite.h>
#include <script.h>

#include "texteditdialog.h"
#include "newprereqdialog.h"
#include "providerdialog.h"
#include "queryeditor.h"

/*
 *  Constructs a PackageWindow as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
PackageWindow::PackageWindow(QWidget* parent, const char* name, Qt::WindowFlags fl)
    : Q3MainWindow(parent, name, fl)
{
  setupUi(this);

  (void)statusBar();

  _onError->insertStringList(Script::onErrorList());
  fileNew();
}

/*
 *  Destroys the object and frees any allocated resources
 */
PackageWindow::~PackageWindow()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void PackageWindow::languageChange()
{
  retranslateUi(this);
}

void PackageWindow::fileNew()
{
  _filename = QString::null;

  if(_package)
  {
    delete _package;
    _package = 0;
  }
  _package = new Package();

  _prereqs->clear();
  sPrereqSelectionChanged();

  _scripts->clear();
  sScriptSelectionChanged();

  _reports->clear();
  sReportSelectionChanged();
}

void PackageWindow::fileOpen()
{
  QString filename = QFileDialog::getOpenFileName(this, "Choose a file to open",
                                                  _filename, tr("XML (*.xml)"));
  if(filename.isEmpty())
    return;

  QFile file(filename);

  QDomDocument doc = QDomDocument();
  QString errMsg;
  int errLine, errCol;
  if(doc.setContent(&file, &errMsg, &errLine, &errCol))
  {
    _filename = filename;

    if(_package)
    {
      delete _package;
      _package = 0;
    }
    QStringList msgList;
    QList<bool> fatalList;
    _package = new Package(doc.documentElement(), msgList, fatalList);

    _prereqs->clear();
    sPrereqSelectionChanged();

    _scripts->clear();
    sScriptSelectionChanged();

    _reports->clear();
    sReportSelectionChanged();

    QList<Prerequisite*>::iterator pit = _package->_prerequisites.begin();
    for(; pit != _package->_prerequisites.end(); ++pit)
      _prereqs->insertItem((*pit)->name());

    QList<Script*>::iterator sit = _package->_scripts.begin();
    for(; sit != _package->_scripts.end(); ++sit)
      _scripts->insertItem((*sit)->name());

    QList<LoadReport*>::iterator rit = _package->_reports.begin();
    for(; rit != _package->_reports.end(); ++rit)
      _reports->insertItem((*rit)->name());
    _reports->sort();
  }
  else
    QMessageBox::warning(this, tr("Error Loading File"), tr("An error was encountered while trying to load the file %1.\n%2\nLine %3, Column %4").arg(filename).arg(errMsg).arg(errLine).arg(errCol));
}

void PackageWindow::fileSave()
{
  if(_filename.isEmpty())
  {
    fileSaveAs();
    return;
  }

  QDomDocument doc = QDomDocument("packageManagerDef");
  doc.appendChild(_package->createElement(doc));

  QFile file(_filename);
  if(file.open(QIODevice::WriteOnly))
  {
    QTextStream ts(&file);
    ts << doc.toString();
    file.close();
  }
  else
    QMessageBox::warning(this, tr("Error Opening File"), tr("Could not open the file %1 for writing.").arg(_filename));
}

void PackageWindow::fileSaveAs()
{
  QString filename = QFileDialog::getSaveFileName(this, QString(), _filename);
  if(filename.isEmpty())
    return;

  _filename = filename;
  fileSave();
}

void PackageWindow::fileExit()
{
  qApp->closeAllWindows();
}

void PackageWindow::helpIndex()
{
  QMessageBox::information(this, tr("Not yet implimented"), tr("This feature has not yet been implimented."));
}

void PackageWindow::helpContents()
{
  QMessageBox::information(this, tr("Not yet implimented"), tr("This feature has not yet been implimented."));
}

void PackageWindow::helpAbout()
{
  QMessageBox::about(this, tr("Package Manager: Builder"),
    tr("Create package manager content files."
       "\n\nCopyright (c) 1999-2011 by OpenMFG LLC, d/b/a xTuple, All Rights Reserved"));
}

void PackageWindow::sPrereqSelectionChanged()
{
  if(_prereqs->currentItem() != -1)
  {
    bool found = false;
    QString name = _prereqs->currentText();
    QList<Prerequisite*>::iterator it = _package->_prerequisites.begin();
    for(; it != _package->_prerequisites.end(); ++it)
    {
      if((*it)->name() == name)
      {
        _raisePrereq->setEnabled(true);
        _lowerPrereq->setEnabled(true);
        _removePrereq->setEnabled(true);
        _typeLabel->setEnabled(true);
        _type->setText(Prerequisite::typeToName((*it)->type()));
        _type->setEnabled(true);
        _editConditions->setEnabled(true);
        _prereqMessage->setText((*it)->message());
        _gbPrereqMessage->setEnabled(true);
        _providers->clear();
        _providers->insertStringList((*it)->providerList());
        _providers->sort();
        _gbProviders->setEnabled(true);
        found = true;
      }
    }
    if(!found)
      QMessageBox::critical(this, tr("Not Found"), tr("The selected script was not found."));
  }
  else
  {
    _raisePrereq->setEnabled(false);
    _lowerPrereq->setEnabled(false);
    _removePrereq->setEnabled(false);
    _typeLabel->setEnabled(false);
    _type->setText(tr("None"));
    _type->setEnabled(false);
    _editConditions->setEnabled(false);
    _prereqMessage->clear();
    _gbPrereqMessage->setEnabled(false);
    _providers->clear();
    sProviderSelectionChanged();
    _gbProviders->setEnabled(false);
  }
}

void PackageWindow::sAddPrereq()
{
  NewPrereqDialog npd(this, "new prereq");
  bool done = false;
  while(!done)
  {
    if(npd.exec() == NewPrereqDialog::Accepted)
    {
      QString name = npd._name->text();
      if(_package->containsPrerequisite(name))
      {
        QMessageBox::warning(this, tr("Duplicate Name"), tr("The name you selected for the new prerequisite is already in use.\nPlease choose a different name.") );
        continue;
      }

      Prerequisite *prereq = new Prerequisite();
      prereq->setName(name);
      prereq->setType(Prerequisite::nameToType(npd._type->currentText()));
      _package->_prerequisites.append(prereq);
      _prereqs->insertItem(prereq->name());
      Q3ListBoxItem * item = _prereqs->findItem(prereq->name());
      _prereqs->setCurrentItem(item);
      _prereqs->ensureCurrentVisible();
    }
    done = true;
  }
}

void PackageWindow::sRaisePrereq()
{
  int idx = _prereqs->currentItem();
  if(idx <= 0)
    return;

  Prerequisite *p0 = _package->_prerequisites[idx-1];
  Prerequisite *p1 = _package->_prerequisites[idx];

  if(p1->name() != _prereqs->currentText())
  {
    QMessageBox::warning(this, tr("Data Error"), tr("The data does not appear to be in a correct state."));
    return;
  }

  _package->_prerequisites[idx-1] = p1;
  _package->_prerequisites[idx]   = p0;

  _prereqs->changeItem(p1->name(), idx-1);
  _prereqs->changeItem(p0->name(), idx);

  _prereqs->setCurrentItem(idx-1);
}


void PackageWindow::sLowerPrereq()
{
  int idx = _prereqs->currentItem();
  if(idx == (int)(_prereqs->count() - 1))
    return;

  Prerequisite *p0 = _package->_prerequisites[idx];
  Prerequisite *p1 = _package->_prerequisites[idx+1];

  if(p0->name() != _prereqs->currentText())
  {
    QMessageBox::warning(this, tr("Data Error"), tr("The data does not appear to be in a correct state."));
    return;
  }

  _package->_prerequisites[idx] = p1;
  _package->_prerequisites[idx+1]   = p0;

  _prereqs->changeItem(p1->name(), idx);
  _prereqs->changeItem(p0->name(), idx+1);

  _prereqs->setCurrentItem(idx+1);
}

void PackageWindow::sRemovePrereq()
{
  QString name = _prereqs->currentText();
  QList<Prerequisite*>::iterator it = _package->_prerequisites.begin();
  for(; it != _package->_prerequisites.end(); ++it)
    if((*it)->name() == name)
      it = _package->_prerequisites.remove(it);
  _prereqs->removeItem(_prereqs->currentItem());
}

void PackageWindow::sEditConditions()
{
  QString name = _prereqs->currentText();
  QList<Prerequisite*>::iterator it = _package->_prerequisites.begin();
  for(; it != _package->_prerequisites.end(); ++it)
  {
    if((*it)->name() == name)
    {
      if((*it)->type() == Prerequisite::Query)
      {
        QueryEditor qe(this, "query editor");
        qe._text->setText((*it)->query());
        if(qe.exec() == QueryEditor::Accepted)
        {
          (*it)->setQuery(qe._text->text());
        }
      }
      else
        QMessageBox::warning(this, tr("Not yet implemented"), tr("The functionality for this type has not been implemented yet.") );
    }
  }
}

void PackageWindow::sPrereqTextChanged()
{
  QString name = _prereqs->currentText();
  QList<Prerequisite*>::iterator it = _package->_prerequisites.begin();
  for(; it != _package->_prerequisites.end(); ++it)
    if((*it)->name() == name)
      (*it)->setMessage(_prereqMessage->text());
}

void PackageWindow::sEditPrereqMessage()
{
  TextEditDialog ted(this, "prereq message dialog");
  ted._text->setText(_prereqMessage->text());
  if(ted.exec() == TextEditDialog::Accepted)
    _prereqMessage->setText(ted._text->text());
}

void PackageWindow::sProviderSelectionChanged()
{
  bool selected = (_providers->currentItem() != -1);

  _editProvider->setEnabled(selected);
  _removeProvider->setEnabled(selected);
}

void PackageWindow::sAddProvider()
{
  ProviderDialog pd(this, "provider dialog");
  bool done = false;
  while(!done)
  {
    done = true;
    if(pd.exec() == ProviderDialog::Accepted)
    {
      PrerequisiteProvider provider;
      QString name = _prereqs->currentText();
      QList<Prerequisite*>::iterator it = _package->_prerequisites.begin();
      for(; it != _package->_prerequisites.end(); ++it)
      {
        if((*it)->name() == name)
        {
          provider = (*it)->provider(pd._name->text());
          if(!provider.isValid())
          {
            provider = PrerequisiteProvider(pd._name->text(), pd._text->text());
            (*it)->setProvider(provider);
            _providers->insertItem(provider.package());
            _providers->sort();
          }
          else
            done = false;
        }
      }
    }
  }
}

void PackageWindow::sEditProvider()
{
  QString name = _prereqs->currentText();
  QList<Prerequisite*>::iterator it = _package->_prerequisites.begin();
  for(; it != _package->_prerequisites.end(); ++it)
  {
    if((*it)->name() == name)
    {
      PrerequisiteProvider provider = (*it)->provider(_providers->currentText());
      if(provider.isValid())
      {
        ProviderDialog pd(this, "provider dialog");
        pd._name->setText(provider.package());
        pd._text->setText(provider.info());
        bool done = false;
        while(!done)
        {
          if(pd.exec() == ProviderDialog::Accepted)
          {
            if(pd._name->text() != provider.package())
            {
              if((*it)->provider(pd._name->text()).isValid())
              {
                QMessageBox::warning(this, tr("Duplicate Package"), tr("The new package name you specified already exists in the list of providers.") );
                continue;
              }
              (*it)->removeProvider(provider.package());
              provider.setPackage(pd._name->text());
              _providers->changeItem(provider.package(), _providers->currentItem());
              _providers->sort();
            }
            provider.setInfo(pd._text->text());
            (*it)->setProvider(provider);
          }
          done = true;
        }
      }
      else
        QMessageBox::critical(this, tr("Error locating data"), tr("There was an error locating the data for the selected provider.") );
    }
  }
}

void PackageWindow::sRemoveProvider()
{
  QString provider = _providers->currentText();
  QString name = _prereqs->currentText();
  QList<Prerequisite*>::iterator it = _package->_prerequisites.begin();
  for(; it != _package->_prerequisites.end(); ++it)
    if((*it)->name() == name)
      (*it)->removeProvider(provider);
  _providers->removeItem(_providers->currentItem());
}


void PackageWindow::sScriptSelectionChanged()
{
  if(_scripts->currentItem() != -1)
  {
    bool found = false;
    QString name = _scripts->currentText();
    QList<Script*>::iterator it = _package->_scripts.begin();
    for(; it != _package->_scripts.end(); ++it)
    {
      if((*it)->name() == name)
      {
        _raiseScript->setEnabled(true);
        _lowerScript->setEnabled(true);
        _removeScript->setEnabled(true);
        _onErrorLabel->setEnabled(true);
        _onError->setCurrentItem((int)(*it)->onError());
        _onError->setEnabled(true);
        _scriptMessage->setText((*it)->comment());
        _gbScriptMessage->setEnabled(true);
        found = true;
      }
    }
    if(!found)
      QMessageBox::critical(this, tr("Not Found"), tr("The selected script was not found."));
  }
  else
  {
    _raiseScript->setEnabled(false);
    _lowerScript->setEnabled(false);
    _removeScript->setEnabled(false);
    _onErrorLabel->setEnabled(false);
    _onError->setCurrentItem(0);
    _onError->setEnabled(false);
    _scriptMessage->clear();
    _gbScriptMessage->setEnabled(false);
  }
}

void PackageWindow::sAddScript()
{
  QString scriptfile =
    QFileDialog::getOpenFileName(this, tr("Choose a Script to add"), QString::null,
                                 tr("Script (*.*)") );
  if(scriptfile.isNull())
    return;

  if(_package->containsScript(scriptfile))
  {
    QMessageBox::warning(this, tr("Script Already Exists"),
      tr("The script you selected has already been loaded.") );
    return;
  }

  Script *script = new Script(scriptfile);
  _package->_scripts.append(script);
  _scripts->insertItem(script->name());
  Q3ListBoxItem * item = _scripts->findItem(script->name());
  _scripts->setCurrentItem(item);
  _scripts->ensureCurrentVisible();
}

void PackageWindow::sRaiseScript()
{
  int idx = _scripts->currentItem();
  if(idx <= 0)
    return;

  Script *s0 = _package->_scripts[idx-1];
  Script *s1 = _package->_scripts[idx];

  if(s1->name() != _scripts->currentText())
  {
    QMessageBox::warning(this, tr("Data Error"), tr("The data does not appear to be in a correct state."));
    return;
  }

  _package->_scripts[idx-1] = s1;
  _package->_scripts[idx]   = s0;

  _scripts->changeItem(s1->name(), idx-1);
  _scripts->changeItem(s0->name(), idx);

  _scripts->setCurrentItem(idx-1);
}

void PackageWindow::sLowerScript()
{
  int idx = _scripts->currentItem();
  if(idx == (int)(_scripts->count() - 1))
    return;

  Script *s0 = _package->_scripts[idx];
  Script *s1 = _package->_scripts[idx+1];

  if(s0->name() != _scripts->currentText())
  {
    QMessageBox::warning(this, tr("Data Error"), tr("The data does not appear to be in a correct state."));
    return;
  }

  _package->_scripts[idx] = s1;
  _package->_scripts[idx+1] = s0;

  _scripts->changeItem(s1->name(), idx);
  _scripts->changeItem(s0->name(), idx+1);

  _scripts->setCurrentItem(idx+1);
}

void PackageWindow::sRemoveScript()
{
  QString name = _scripts->currentText();
  QList<Script*>::iterator it = _package->_scripts.begin();
  for(; it != _package->_scripts.end(); ++it)
    if((*it)->name() == name)
      it = _package->_scripts.remove(it);
  _scripts->removeItem(_scripts->currentItem());
}

void PackageWindow::sOnErrorActivated( const QString & string )
{
  QString name = _scripts->currentText();
  QList<Script*>::iterator it = _package->_scripts.begin();
  for(; it != _package->_scripts.end(); ++it)
    if((*it)->name() == name)
      (*it)->setOnError(Script::nameToOnError(string));
}

void PackageWindow::sScriptTextChanged()
{
  QString name = _scripts->currentText();
  QList<Script*>::iterator it = _package->_scripts.begin();
  for(; it != _package->_scripts.end(); ++it)
    if((*it)->name() == name)
      (*it)->setComment(_scriptMessage->text());
}

void PackageWindow::sEditScriptMessage()
{
  TextEditDialog ted(this, "script message dialog");
  ted._text->setText(_scriptMessage->text());
  if(ted.exec() == TextEditDialog::Accepted)
    _scriptMessage->setText(ted._text->text());
}

void PackageWindow::sReportSelectionChanged()
{
  if(_reports->currentItem() != -1)
  {
    bool found = false;
    QString name = _reports->currentText();
    QList<LoadReport*>::iterator it = _package->_reports.begin();
    for(; it != _package->_reports.end(); ++it)
    {
      if((*it)->name() == name)
      {
        _removeReport->setEnabled(true);
        _gradeLabel->setEnabled(true);
        _grade->setValue((*it)->grade());
        _grade->setEnabled(true);
        _reportMessage->setText((*it)->comment());
        _gbReportMessage->setEnabled(true);
        found = true;
      }
    }
    if(!found)
      QMessageBox::critical(this, tr("Not Found"), tr("The selected report definition was not found."));
  }
  else
  {
    _removeReport->setEnabled(false);
    _gradeLabel->setEnabled(false);
    _grade->setValue(0);
    _grade->setEnabled(false);
    _reportMessage->clear();
    _gbReportMessage->setEnabled(false);
  }
}

void PackageWindow::sAddReport()
{
  QString reportfile =
    QFileDialog::getOpenFileName(this, tr("Choose a Report Definition to add"), QString::null,
                                 tr("Report Definitions (*.xml)") );
  if(reportfile.isNull())
    return;

  if(_package->containsReport(reportfile))
  {
    QMessageBox::warning(this, tr("Report Already Exists"),
      tr("The Report definition you selected has already been loaded.") );
    return;
  }

  LoadReport *report = new LoadReport(reportfile);
  _package->_reports.append(report);
  _reports->insertItem(report->name());
  Q3ListBoxItem * item = _reports->findItem(report->name());
  _reports->setCurrentItem(item);
  _reports->sort();
  _reports->ensureCurrentVisible();
}

void PackageWindow::sRemoveReport()
{
  QString name = _reports->currentText();
  QList<LoadReport*>::iterator it = _package->_reports.begin();
  for(; it != _package->_reports.end(); ++it)
    if((*it)->name() == name)
      it = _package->_reports.remove(it);
  _reports->removeItem(_reports->currentItem());
}

void PackageWindow::sGradeChanged( int value)
{
  QString name = _reports->currentText();
  QList<LoadReport*>::iterator it = _package->_reports.begin();
  for(; it != _package->_reports.end(); ++it)
    if((*it)->name() == name)
      (*it)->setGrade(value);
}

void PackageWindow::sReportTextChanged()
{
  QString name = _reports->currentText();
  QList<LoadReport*>::iterator it = _package->_reports.begin();
  for(; it != _package->_reports.end(); ++it)
    if((*it)->name() == name)
      (*it)->setComment(_reportMessage->text());
}

void PackageWindow::sEditReportMessage()
{
  TextEditDialog ted(this, "report message dialog");
  ted._text->setText(_reportMessage->text());
  if(ted.exec() == TextEditDialog::Accepted)
    _reportMessage->setText(ted._text->text());
}
