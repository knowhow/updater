/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "package.h"

#include <QDomDocument>
#include <QList>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>

#include "createfunction.h"
#include "createtable.h"
#include "createtrigger.h"
#include "createview.h"
#include "data.h"
#include "loadcmd.h"
#include "loadappscript.h"
#include "loadappui.h"
#include "loadimage.h"
#include "loadmetasql.h"
#include "loadpriv.h"
#include "loadreport.h"
#include "prerequisite.h"
#include "script.h"
#include "finalscript.h"
#include "initscript.h"
#include "xsqlquery.h"
#include "xversion.h"

#define DEBUG false

Package::Package(const QString & id)
  : _id(id)
{
}

Package::Package(const QDomElement & elem, QStringList &msgList,
                 QList<bool> &fatalList)
{
  if (elem.tagName() != "package")
  {
    msgList << TR("The root tag must be 'package' but this package has a root "
                  "tag named '%1'").arg(elem.tagName());
    fatalList << true;
  }

  if (elem.hasAttribute("updater"))
  {
    // _version = updater/builder application global
    XVersion updaterversion(_version);
    if (! updaterversion.isValid())
    {
      msgList << TR("Could not parse the application's version string %1")
                  .arg(_version);
      fatalList << true;
      return;
    }

    XVersion requiredversion(elem.attribute("updater"));
    if (! requiredversion.isValid())
    {
      msgList << TR("Could not parse the updater version string %1 required "
                    "by the package") .arg(elem.attribute("updater"));
      fatalList << true;
      return;
    }

    if (updaterversion < requiredversion)
    {
      msgList << TR("This package requires a newer version of the updater "
                    "(%1) than you are currently running (%2). Please get "
                    "a newer updater.")
                  .arg(elem.attribute("updater")).arg(_version);
      fatalList << true;
      return;
    }
  }

  _id = elem.attribute("id");
  _name = elem.attribute("name");
  _developer = elem.attribute("developer");
  _descrip = elem.attribute("descrip");

  if (DEBUG)
    qDebug("Package::Package() - _name '%s', _developer '%s' => system %d",
           qPrintable(_name), qPrintable(_developer), system());

  if (elem.hasAttribute("version"))
  {
    _pkgversion.setVersion(elem.attribute("version"));
    if (! _pkgversion.isValid())
    {
      msgList << TR("Could not parse the package version string %1.")
                  .arg(elem.attribute("version"));
      fatalList << true;
      return;
    }
  }
  else if (! system())
  {
    msgList << TR("Add-on packages must have version numbers but the package "
                  "element has no version attribute.");
    fatalList << true;
    return;
  }

  QStringList reportedErrorTags;

  QDomNodeList nList = elem.childNodes();
  for(int n = 0; n < nList.count(); ++n)
  {
    QDomElement elemThis = nList.item(n).toElement();
    if (elemThis.tagName() == "createfunction")
      _functions.append(new CreateFunction(elemThis, msgList, fatalList));
    else if (elemThis.tagName() == "createtable")
      _tables.append(new CreateTable(elemThis, msgList, fatalList));
    else if (elemThis.tagName() == "createtrigger")
      _triggers.append(new CreateTrigger(elemThis, msgList, fatalList));
    else if (elemThis.tagName() == "createview")
      _views.append(new CreateView(elemThis, msgList, fatalList));
    else if(elemThis.tagName() == "loadmetasql")
      _metasqls.append(new LoadMetasql(elemThis, system(), msgList, fatalList));
    else if(elemThis.tagName() == "loadpriv")
      _privs.append(new LoadPriv(elemThis, system(), msgList, fatalList));
    else if(elemThis.tagName() == "loadreport")
      _reports.append(new LoadReport(elemThis, system(), msgList, fatalList));
    else if(elemThis.tagName() == "loadappui")
      _appuis.append(new LoadAppUI(elemThis, system(), msgList, fatalList));
    else if(elemThis.tagName() == "loadappscript")
      _appscripts.append(new LoadAppScript(elemThis, system(), msgList, fatalList));
    else if(elemThis.tagName() == "loadcmd")
      _cmds.append(new LoadCmd(elemThis, system(), msgList, fatalList));
    else if(elemThis.tagName() == "loadimage")
      _images.append(new LoadImage(elemThis, system(), msgList, fatalList));
    else if (elemThis.tagName() == "pkgnotes")
      _notes += elemThis.text();
    else if(elemThis.tagName() == "prerequisite")
      _prerequisites.append(new Prerequisite(elemThis));
    else if(elemThis.tagName() == "script")
      _scripts.append(new Script(elemThis, msgList, fatalList));
    else if(elemThis.tagName() == "finalscript")
      _finalscripts.append(new FinalScript(elemThis, msgList, fatalList));
    else if(elemThis.tagName() == "initscript")
      _initscripts.append(new InitScript(elemThis, msgList, fatalList));
    else if (! reportedErrorTags.contains(elemThis.tagName()))
    {
      QMessageBox::warning(0, TR("Unknown Package Element"),
                           TR("This package contains an element '%1'. "
                              "The application does not know how to "
                              "process it and so it will be ignored.")
                           .arg(elemThis.tagName()));
      reportedErrorTags << elemThis.tagName();
    }
  }

  if (DEBUG)
  {
    qDebug("Package::Package(QDomElement) msgList & fatalList at %d and %d",
           msgList.size(), fatalList.size());
    qDebug("_functions:     %d", _functions.size());
    qDebug("_tables:        %d", _tables.size());
    qDebug("_triggers:      %d", _triggers.size());
    qDebug("_views:         %d", _views.size());
    qDebug("_metasqls:      %d", _metasqls.size());
    qDebug("_privs:         %d", _privs.size());
    qDebug("_reports:       %d", _reports.size());
    qDebug("_appuis:        %d", _appuis.size());
    qDebug("_appscripts:    %d", _appscripts.size());
    qDebug("_cmds:          %d", _cmds.size());
    qDebug("_images:        %d", _images.size());
    qDebug("_prerequisites: %d", _prerequisites.size());
    qDebug("_scripts:       %d", _scripts.size());
  }
}

Package::~Package()
{
}

bool Package::system() const
{
  return _name.isEmpty() && (_developer == "xTuple" || _developer.isEmpty());
}

QDomElement Package::createElement(QDomDocument & doc)
{
  QDomElement elem = doc.createElement("package");
  elem.setAttribute("id", _id);
  elem.setAttribute("version", _pkgversion.toString());

  for(QList<Prerequisite*>::iterator i = _prerequisites.begin();
      i != _prerequisites.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<LoadPriv*>::iterator i = _privs.begin(); i != _privs.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<LoadMetasql*>::iterator i = _metasqls.begin(); i != _metasqls.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<Script*>::iterator i = _scripts.begin(); i != _scripts.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<LoadReport*>::iterator i = _reports.begin();
      i != _reports.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<LoadAppUI*>::iterator i = _appuis.begin(); i != _appuis.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<LoadAppScript*>::iterator i = _appscripts.begin();
      i != _appscripts.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<LoadCmd*>::iterator i = _cmds.begin(); i != _cmds.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for (QList<LoadImage*>::iterator i = _images.begin(); i != _images.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<FinalScript*>::iterator i = _finalscripts.begin(); i != _finalscripts.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  for(QList<InitScript*>::iterator i = _initscripts.begin(); i != _initscripts.end(); ++i)
    elem.appendChild((*i)->createElement(doc));

  return elem;
}

bool Package::containsReport(const QString & reportname) const
{
  QList<LoadReport*>::const_iterator it = _reports.begin();
  for(; it != _reports.end(); ++it)
  {
    if((*it)->name() == reportname)
      return true;
  }
  return false;
}

bool Package::containsScript(const QString & scriptname) const
{
  QList<Script*>::const_iterator it = _scripts.begin();
  for(; it != _scripts.end(); ++it)
  {
    if((*it)->name() == scriptname)
      return true;
  }
  return false;
}

bool Package::containsFinalScript(const QString & scriptname) const
{
  QList<FinalScript*>::const_iterator it = _finalscripts.begin();
  for(; it != _finalscripts.end(); ++it)
  {
    if((*it)->name() == scriptname)
      return true;
  }
  return false;
}

bool Package::containsInitScript(const QString & scriptname) const
{
  QList<InitScript*>::const_iterator it = _initscripts.begin();
  for(; it != _initscripts.end(); ++it)
  {
    if((*it)->name() == scriptname)
      return true;
  }
  return false;
}

bool Package::containsPrerequisite(const QString & prereqname) const
{
  QList<Prerequisite*>::const_iterator it = _prerequisites.begin();
  for(; it != _prerequisites.end(); ++it)
  {
    if((*it)->name() == prereqname)
      return true;
  }
  return false;
}

bool Package::containsAppScript(const QString &pname) const
{
  QList<LoadAppScript*>::const_iterator it = _appscripts.begin();
  for(; it != _appscripts.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsAppUI(const QString &pname) const
{
  QList<LoadAppUI*>::const_iterator it = _appuis.begin();
  for(; it != _appuis.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsImage(const QString &pname) const
{
  QList<LoadImage*>::const_iterator it = _images.begin();
  for(; it != _images.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsCmd(const QString &pname) const
{
  QList<LoadCmd*>::const_iterator it = _cmds.begin();
  for(; it != _cmds.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsFunction(const QString &pname) const
{
  QList<CreateFunction*>::const_iterator it = _functions.begin();
  for(; it != _functions.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsMetasql(const QString &pname) const
{
  QList<LoadMetasql*>::const_iterator it = _metasqls.begin();
  for(; it != _metasqls.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsPriv(const QString &pname) const
{
  QList<LoadPriv*>::const_iterator it = _privs.begin();
  for(; it != _privs.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsTable(const QString &pname) const
{
  QList<CreateTable*>::const_iterator it = _tables.begin();
  for(; it != _tables.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsTrigger(const QString &pname) const
{
  QList<CreateTrigger*>::const_iterator it = _triggers.begin();
  for(; it != _triggers.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

bool Package::containsView(const QString &pname) const
{
  QList<CreateView*>::const_iterator it = _views.begin();
  for(; it != _views.end(); ++it)
  {
    if((*it)->name() == pname)
      return true;
  }
  return false;
}

int Package::writeToDB(QString &errMsg)
{
  XSqlQuery select;
  XSqlQuery upsert;
  QString sqlerrtxt = TR("<font color=red>The following error was "
                         "encountered while trying to import %1 into "
                         "the database:<br>%2<br>%3</font>");

  if (_name.isEmpty())
    return 0;   // if there's no name then there's no package to create

  int pkgheadid = -1;
  select.prepare("SELECT pkghead_id FROM pkghead WHERE (pkghead_name=:name);");
  select.bindValue(":name", _name);
  select.exec();
  if (select.first())
  {
    pkgheadid = select.value(0).toInt();
    upsert.prepare("UPDATE pkghead "
                   "   SET pkghead_name=:name,"
                   "       pkghead_descrip=:descrip,"
                   "       pkghead_version=:version,"
                   "       pkghead_developer=:developer,"
                   "       pkghead_notes=:notes "
                   "WHERE (pkghead_id=:id);");
  }
  else if (select.lastError().type() != QSqlError::NoError)
  {
    QSqlError err = select.lastError();
    errMsg = sqlerrtxt.arg(_name).arg(err.driverText()).arg(err.databaseText());
    return -1;
  }
  else
  {
    upsert.exec("SELECT NEXTVAL('pkghead_pkghead_id_seq');");
    if (upsert.first())
      pkgheadid = upsert.value(0).toInt();
    else if (upsert.lastError().type() != QSqlError::NoError)
    {
      QSqlError err = upsert.lastError();
      errMsg = sqlerrtxt.arg(_name).arg(err.driverText()).arg(err.databaseText());
      return -2;
    }
    upsert.prepare("INSERT INTO pkghead ("
                   "       pkghead_id, pkghead_name, pkghead_descrip,"
                   "       pkghead_version, pkghead_developer, pkghead_notes"
                   ") VALUES ("
                   "     :id, :name, :descrip, :version, :developer, :notes);");
  } 
  upsert.bindValue(":id",        pkgheadid);
  upsert.bindValue(":name",      _name);
  upsert.bindValue(":descrip",   _descrip);
  upsert.bindValue(":version",   _pkgversion.toString());
  upsert.bindValue(":developer", _developer);
  upsert.bindValue(":notes",     _notes);
  if (!upsert.exec())
  {
    QSqlError err = upsert.lastError();
    errMsg = sqlerrtxt.arg(_name).arg(err.driverText()).arg(err.databaseText());
    return -3;
  }

  return pkgheadid;
}
