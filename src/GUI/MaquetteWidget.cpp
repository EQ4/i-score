
/*

Copyright: LaBRI / SCRIME

Authors: Luc Vercellin and Bruno Valeze (08/03/2010)

luc.vercellin@labri.fr

This software is a computer program whose purpose is to provide
notation/composition combining synthesized as well as recorded
sounds, providing answers to the problem of notation and, drawing,
from its very design, on benefits from state of the art research
in musicology and sound/music computing.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software. You can use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty and the software's author, the holder of the
economic rights, and the successive licensors have only limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading, using, modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean that it is complicated to manipulate, and that also
therefore means that it is reserved for developers and experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and, more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#include <iostream>
using std::string;

#include <QPainter>
#include <QGridLayout>
#include <QBoxLayout>
#include <QMouseEvent>
#include <QString>
#include <QWidget>
#include <QGraphicsEffect>
#include <QGraphicsOpacityEffect>

#include "MaquetteWidget.hpp"
#include "MaquetteScene.hpp"
#include "MainWindow.hpp"

const float MaquetteWidget::HEADER_HEIGHT = 40;
const float MaquetteWidget::NAME_POINT_SIZE = 20;

MaquetteWidget::MaquetteWidget(QWidget *parent, MaquetteView *view, MaquetteScene *scene)
    : QWidget(parent){

    _view = view;
    _scene = scene;
    _color = QColor(Qt::white);
    _maquetteLayout = new QGridLayout();
    _nameLabel = new QLabel;
    _toolBar = new QToolBar;
    _header = new QWidget(NULL);
    _comboBox = new QComboBox;

    createActions();
    createToolBar();
    createNameLabel();
    createHeader();

    _maquetteLayout->addWidget(_header);
    _maquetteLayout->addWidget(_view);
    _maquetteLayout->setContentsMargins(0,0,0,0);
    _maquetteLayout->setVerticalSpacing(0);
    setLayout(_maquetteLayout);

    connect(_scene,SIGNAL(stopPlaying()),this,SLOT(stop()));
}

void
MaquetteWidget::init(){

}

MaquetteWidget::~MaquetteWidget(){

}

void
MaquetteWidget::createNameLabel(){
    QFont *font = new QFont();
    font->setPointSize(NAME_POINT_SIZE);
    _nameLabel->setFont(*font);
    _nameLabel->setText("Scenario");
}

void
MaquetteWidget::createActions(){
    _playAction = new QAction(QIcon(":/images/play.svg"), tr("Play"), this);
    _playAction->setShortcut(QString("Space"));
    _playAction->setStatusTip(tr("Play composition"));
    _playAction->setCheckable(true);

    _stopAction = new QAction(QIcon(":/images/stop.svg"), tr("Stop"), this);
    _stopAction->setShortcut(QString("Enter"));
    _stopAction->setStatusTip(tr("Stop composition audio preview"));
    _stopAction->setCheckable(true);

    _accelerationSlider = new LogarithmicSlider(Qt::Horizontal,this);
    _accelerationSlider->setStatusTip(tr("Acceleration"));
    _accelerationSlider->setFixedWidth(200);
    _accelerationSlider->setSliderPosition(50);

    connect(_accelerationSlider,SIGNAL(valueChanged(int)),this,SLOT(accelerationValueModified(int)));
    connect(_playAction,SIGNAL(triggered()), this, SLOT(play()));
    connect(_stopAction,SIGNAL(triggered()), this, SLOT(stop()));
}

void
MaquetteWidget::createToolBar(){
    _toolBar->addAction(_playAction);
}

void
MaquetteWidget::createHeader(){
    _header->setGeometry(0,0,width(),HEADER_HEIGHT);
    _header->setPalette(QPalette(_color));
    _header->setAutoFillBackground(true);

//    QGridLayout *layout= new QGridLayout;
//    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

//    layout->addWidget(_toolBar,0,0);
//    layout->addWidget(_nameLabel,0,1);
//    layout->addWidget(_accelerationSlider,0,2);
//    layout->setAlignment(_accelerationSlider,Qt::AlignRight);
//    layout->setContentsMargins(0,0,0,0);
//    _header->setLayout(layout);

    QBoxLayout *layout= new QBoxLayout(QBoxLayout::LeftToRight);
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    layout->addWidget(_toolBar);
    layout->addWidget(_nameLabel);
    layout->addWidget(_accelerationSlider);
    layout->insertStretch(2,_scene->width()/2);
    layout->insertSpacing(4,20);
    layout->setAlignment(_accelerationSlider,Qt::AlignRight);
    layout->setContentsMargins(0,0,0,0);
    _header->setLayout(layout);
}

void
MaquetteWidget::accelerationValueModified(int value){
    emit(accelerationValueChanged(value));
}

void
MaquetteWidget::play(){
    _scene->play();
    updateHeader();
}

void
MaquetteWidget::stop(){
    _scene->stop();
    updateHeader();
}

void
MaquetteWidget::updateHeader(){
    setAvailableAction(_scene->playing() ? _stopAction : _playAction);
}

void
MaquetteWidget::setName(QString name){
    _nameLabel->setText(name);
}

void
MaquetteWidget::setAvailableAction(QAction *action){
    if(!_toolBar->actions().empty()){
        _toolBar->removeAction(_toolBar->actions().first());
        _toolBar->addAction(action);
    }
}

