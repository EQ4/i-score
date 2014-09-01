#ifndef TIMEBARWIDGET_HPP
#define TIMEBARWIDGET_HPP

#include <QWidget>

#include "MaquetteScene.hpp"

class TimeBarWidget : public QWidget {
  Q_OBJECT

  public:
    TimeBarWidget(QWidget *parent, MaquetteScene *scene);
    ~TimeBarWidget();
    static const float TIME_BAR_HEIGHT;
    static const float LEFT_MARGIN;
    static const float NUMBERS_POINT_SIZE;
    void init();
    void setZoomValue(float value); /// \todo setZoomValue() fait la même chose que le slot updateZoom(), et n'ai jamais appelée. (par jaime Chao)


  signals:
    void timeOffsetEntered(unsigned int timeOffset);

  public slots:
    void updateZoom(float newValue);

  protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void drawBackground(QPainter *painter, QRect rect);
    float _zoom;

  private:    

    MaquetteScene *_scene;
    float _sceneHeight;
    QRect _rect;
};
#endif // TIMEBARWIDGET_HPP
