/*
 * This file is part of the xTuple ERP: PostBooks Edition, a free and
 * open source Enterprise Resource Planning software suite,
 * Copyright (c) 1999-2010 by OpenMFG LLC, d/b/a xTuple.
 * It is licensed to you under the Common Public Attribution License
 * version 1.0, the full text of which (including xTuple-specific Exhibits)
 * is available at www.xtuple.com/CPAL.  By using this software, you agree
 * to be bound by its terms.
 */

#include "newprereqdialog.h"

#include <QMessageBox>
#include "prerequisite.h"

/*
 *  Constructs a NewPrereqDialog as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
NewPrereqDialog::NewPrereqDialog(QWidget* parent, const char* name, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, name, modal, fl)
{
  setupUi(this);

  _type->insertStringList(Prerequisite::typeList());
}

/*
 *  Destroys the object and frees any allocated resources
 */
NewPrereqDialog::~NewPrereqDialog()
{
  // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void NewPrereqDialog::languageChange()
{
  retranslateUi(this);
}

void NewPrereqDialog::sAccept()
{
  if(_name->text().isEmpty())
  {
    QMessageBox::warning(this, tr("Incomplete information"), tr("You must eneter in a name for this new prerequisite."));
    return;
  }
  
  if(_type->currentText() == "None")
  {
    QMessageBox::warning(this, tr("Incomplete information"), tr("You must select a type, other than None, for this new prerequisite."));
    return;
  }
  
  accept();
}

