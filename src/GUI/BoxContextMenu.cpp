/*
 * Copyright: LaBRI / SCRIME / L'Arboretum
 *
 * Authors: Pascal Baltazar, Nicolas Hincker, Luc Vercellin and Myriam Desainte-Catherine (as of 16/03/2014)
 *
 *iscore.contact@gmail.com
 *
 * This software is an interactive intermedia sequencer.
 * It allows the precise and flexible scripting of interactive scenarios.
 * In contrast to most sequencers, i-score doesn’t produce any media, 
 * but controls other environments’ parameters, by creating snapshots 
 * and automations, and organizing them in time in a multi-linear way.
 * More about i-score on http://www.i-score.org
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#include <iostream>

#include "BoxContextMenu.hpp"
#include "MaquetteScene.hpp"
#include "BasicBox.hpp"

#include <QDialog>
#include <QColorDialog>
#include <QGridLayout>
#include <QLineEdit>
#include "ViewRelations.hpp"

BoxContextMenu::BoxContextMenu(BasicBox *box)
  : QMenu()
{
  _box = box;

//  setWindowModality(Qt::ApplicationModal);

  _changeNameAct = new QAction(tr("Change name"), this);
  addAction(_changeNameAct);
  connect(_changeNameAct, SIGNAL(triggered()), this, SLOT(editName()));

  _commentMenu = new QMenu(tr("Comment"), this);

  _addCommentAct = new QAction(tr("Add Comment"), this);
  _commentMenu->addAction(_addCommentAct);
  connect(_addCommentAct, SIGNAL(triggered()), this, SLOT(addComment()));

  _removeCommentAct = new QAction(tr("Remove Comment"), this);
  _commentMenu->addAction(_removeCommentAct);
  connect(_removeCommentAct, SIGNAL(triggered()), this, SLOT(removeComment()));

  _addCommentAct->setDisabled(true);
  _removeCommentAct->setDisabled(true);
  addMenu(_commentMenu);

  _changeColorAct = new QAction(tr("Change color"), this);
  addAction(_changeColorAct);
  connect(_changeColorAct, SIGNAL(triggered()), this, SLOT(editColor()));

  _viewRelationAct = new QAction(tr("View relations"), this);
  addAction(_viewRelationAct);
  connect(_viewRelationAct, SIGNAL(triggered()), this, SLOT(viewRelations()));

  _detachAct = new QAction(tr("Detach from condition"), this);
  addAction(_detachAct);
  connect(_detachAct, SIGNAL(triggered()), this, SLOT(detachFromCondition()));

  _nameDialog = new QDialog(this);
  _nameDialog->setWindowModality(Qt::WindowModal);
  _nameLayout = new QGridLayout(this);
  _nameDialog->setLayout(_nameLayout);
  _nameLine = new QLineEdit(this);
  _nameLayout->addWidget(_nameLine, 0, 0, 1, 1);

  connect(_nameLine, SIGNAL(returnPressed()), this, SLOT(nameChanged()));
}

BoxContextMenu::~BoxContextMenu()
{
  delete _changeNameAct;
  delete _changeColorAct;
  delete _viewRelationAct;
}

void
BoxContextMenu::setDetachActionEnabled(bool enabled){
    _detachAct->setEnabled(enabled);
}

void
BoxContextMenu::editColor()
{
  _box->setColor(QColorDialog::getColor(Qt::white));
}

void
BoxContextMenu::nameChanged()
{
  _box->setName(_nameLine->text());
  _nameDialog->accept();
}

void
BoxContextMenu::editName()
{
  _nameLine->setText(_box->name());
  _nameDialog->exec();
}

void
BoxContextMenu::addComment()
{
  if (_box != NULL) {
      _box->addComment();
    }
}

void
BoxContextMenu::removeComment()
{
  if (_box != NULL) {
      if (_box->hasComment()) {
          static_cast<MaquetteScene*>(_box->scene())->removeComment(_box->comment());
        }
    }
}

void
BoxContextMenu::viewRelations()
{
  ViewRelations viewRelations(_box->ID(), static_cast<MaquetteScene*>(_box->scene()), NULL);
  viewRelations.exec();
}

void
BoxContextMenu::detachFromCondition(){
    _box->detachFromCondition();
}
