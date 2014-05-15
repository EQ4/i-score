/*
 * Copyright: LaBRI / SCRIME
 *
 * Authors: Luc Vercellin (17/05/2010)
 *
 * luc.vercellin@labri.fr
 *
 * This software is a computer program whose purpose is to provide
 * notation/composition combining synthesized as well as recorded
 * sounds, providing answers to the problem of notation and, drawing,
 * from its very design, on benefits from state of the art research
 * in musicology and sound/music computing.
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
#include "BasicBox.hpp"
#include "ParentBox.hpp"
#include "ParentBoxContextMenu.hpp"
#include "MaquetteScene.hpp"
#include "Maquette.hpp"
#include "AbstractParentBox.hpp"
#include "TriggerPoint.hpp"
#include "Relation.hpp"

#include <QString>
#include <QFileDialog>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QBuffer>
#include <QPainter>
#include "TextEdit.hpp"

#include <vector>
#include <map>
#include <sstream>
#include <string>
using std::vector;
using std::map;
using std::string;
using std::stringstream;

ParentBox::ParentBox(const QPointF &corner1, const QPointF &corner2, MaquetteScene *parent)
  : BasicBox(corner1, corner2, parent)
{
  AbstractBox *abstract = new AbstractParentBox(*_abstract);
  delete _abstract;

  _abstract = abstract;

  init();
  update();
}

ParentBox::ParentBox(AbstractParentBox *abstract, MaquetteScene *parent) :
  BasicBox((AbstractBox*)abstract, parent)
{
}

ParentBox::~ParentBox()
{
}

void
ParentBox::init()
{
  BasicBox::init();
  _contextMenu = new ParentBoxContextMenu(this);

  addToComboBox(BasicBox::SUB_SCENARIO_MODE_TEXT);
  _hasContextMenu = true;
}

Abstract*
ParentBox::abstract() const
{
  return (AbstractParentBox*)_abstract;
}

int
ParentBox::type() const
{
  return PARENT_BOX_TYPE;
}

void
ParentBox::updateDisplay(QString displayMode)
{    
  BasicBox *curBox;
  std::map<unsigned int, BasicBox*>::iterator it;
  if (displayMode == SUB_SCENARIO_MODE_TEXT) {
      for (it = _children.begin(); it != _children.end(); ++it) {
          curBox = it->second;
          curBox->lower(false);
          curBox->setEnabled(true);
        }
    }
  else {
      for (it = _children.begin(); it != _children.end(); ++it) {
          curBox = it->second;
          curBox->lower(true);
          curBox->setEnabled(false);
        }
    }
}

bool
ParentBox::hasChild(unsigned int childID) const
{
  return(_children.find(childID) != _children.end());
}

bool
ParentBox::addChild(unsigned int childID)
{
  if (hasChild(childID)) {
      return false;
    }
  else {
      BasicBox *child = _scene->getBox(childID);
      if (child != NULL) {
          _children[childID] = _scene->getBox(childID);
          return true;
        }
    }
  return false;
}

bool
ParentBox::removeChild(unsigned int childID)
{
  if (hasChild(childID)) {
      _children.erase(childID);

      //BasicBox *child = _scene->getBox(childID);

      /* if (child != NULL) {
       *      child->setParentItem(this);
       * }*/

      //child->setGroup(0);
      return true;
    }
  else {
      return false;
    }
}

map<unsigned int, BasicBox*>
ParentBox::children() const
{
  return _children;
}

bool
ParentBox::empty() const
{
  return _children.empty();
}

//QVariant
//ParentBox::itemChange(GraphicsItemChange change, const QVariant &value)
//{
//  QVariant newValue = BasicBox::itemChange(change, value);
//  if (change == ItemPositionChange) {
//      QPointF newPos = newValue.toPoint();
//      std::map<unsigned int, BasicBox*>::iterator it;
//      for (it = _children.begin(); it != _children.end(); ++it) {
//          if (_scene->resizeMode() == NO_RESIZE) {
//              it->second->setTopLeft(QPointF(it->second->beginPos(), it->second->getTopLeft().y() + (newPos.y() - pos().y())));
//              it->second->setPos(it->second->getCenter());
//            }
//          else {
//              if (_scene->resizeMode() == HORIZONTAL_RESIZE || _scene->resizeMode() == DIAGONAL_RESIZE) {
//                  if (it->second->getBottomRight().x() >= (newPos.x() + (newPos.x() - _abstract->topLeft().x()))) {
//                      // Handled by ParentBox::resizeWidthEdition()
//                    }
//                }
//              if (_scene->resizeMode() == VERTICAL_RESIZE || _scene->resizeMode() == DIAGONAL_RESIZE) {
//                  if (it->second->getBottomRight().y() >= (newPos.y() + _abstract->height() / 2.)) {
//                      setSize(QPointF(_abstract->width(), it->second->getBottomRight().y() - _abstract->topLeft().y()));
//                      newPos.setY(it->second->getBottomRight().y() - _abstract->height() / 2.);
//#ifdef DEBUG
//                      std::cerr << "ParentBox::itemChange : Trying to COMPRESS vertically" << std::endl;
//#endif
//                    }
//                }
//            }
//        }
//      newValue = QVariant(newPos);
//    }
//  return newValue;
//}

void
ParentBox::resizeWidthEdition(int width)
{    
  BasicBox::resizeWidthEdition(width);
  float newWidth = _abstract->width();
  std::map<unsigned int, BasicBox*>::iterator it;
  for (it = _children.begin(); it != _children.end(); ++it) {
      if (it->second->getBottomRight().x() >= (_abstract->topLeft().x() + width)) {
          newWidth = it->second->getBottomRight().x() - _abstract->topLeft().x();
        }
    }
  _abstract->setWidth(newWidth);
}

void
ParentBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  BasicBox::mousePressEvent(event);

  if (event->button() == Qt::LeftButton) {
      if (_scene->currentMode() != CREATION_MODE) {
          setSelected(true);
          if (cursor().shape() == Qt::ClosedHandCursor) {
              unlock();
            }
          else if (cursor().shape() == Qt::CrossCursor) {
              lock();
              if (event->pos().x() < boxRect().topLeft().x() + RESIZE_TOLERANCE) {
                  _scene->setRelationFirstBox(_abstract->ID(), BOX_START);
                }
              else if (event->pos().x() > boxRect().topRight().x() - RESIZE_TOLERANCE) {
                  _scene->setRelationFirstBox(_abstract->ID(), BOX_END);
                }
            }
          else if (cursor().shape() == Qt::PointingHandCursor) {
              lock();
              if (event->pos().x() < boxRect().topLeft().x() + RESIZE_TOLERANCE) {
                  addTriggerPoint(BOX_START);
                }
              else if (event->pos().x() > boxRect().topRight().x() - RESIZE_TOLERANCE) {
                  addTriggerPoint(BOX_END);
                }
            }
          else {
              if (cursor().shape() == Qt::SizeHorCursor) {
                  unlock();
                  _scene->setResizeMode(HORIZONTAL_RESIZE);
                }
              else if (cursor().shape() == Qt::SizeVerCursor) {
                  unlock();
                  _scene->setResizeMode(VERTICAL_RESIZE);
                }
              else if (cursor().shape() == Qt::SizeFDiagCursor) {
                  unlock();
                  _scene->setResizeMode(DIAGONAL_RESIZE);
                }
              _scene->setResizeBox(_abstract->ID());
            }
          update();
        }
      else {
          if (cursor().shape() == Qt::OpenHandCursor && event->modifiers() == Qt::ControlModifier) {
              lock();
            }
        }
    }
  else {
    }
}

void
ParentBox::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  QGraphicsItem::mouseMoveEvent(event);
  if (_scene->resizeMode() == NO_RESIZE && cursor().shape() == Qt::ClosedHandCursor) {
      _scene->selectionMoved();
    }
  else if (_scene->resizeMode() != NO_RESIZE && (cursor().shape() == Qt::SizeVerCursor
                                                 || cursor().shape() == Qt::SizeHorCursor || cursor().shape() == Qt::SizeFDiagCursor)) {
      switch (_scene->resizeMode()) {
          case HORIZONTAL_RESIZE:
          resizeWidthEdition(std::max(double(_abstract->width() + event->pos().x() - boxRect().topRight().x()), (double)BOX_MARGIN));
            break;

          case VERTICAL_RESIZE:
            resizeHeightEdition(std::max(double(_abstract->height() + event->pos().y() - _boxRect.bottomRight().y()), (double)BOX_MARGIN));
            break;

          case DIAGONAL_RESIZE:
            resizeAllEdition(std::max(double(_abstract->width() + event->pos().x() - boxRect().topRight().x()), (double)BOX_MARGIN),
                             std::max(double(_abstract->height() + event->pos().y() - boxRect().bottomRight().y()), (double)BOX_MARGIN));
        }
      QPainterPath nullPath;
      nullPath.addRect(QRectF(QPointF(0., 0.), QSizeF(0., 0.)));
      _scene->setSelectionArea(nullPath);
      setSelected(true);
      _scene->boxResized();
    }
}

void
ParentBox::play()
{
  /// \todo old TODO updated (by jC) : parent box play function
}
