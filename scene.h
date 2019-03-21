#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QWidget>
#include "figures.h"
#include <QPainter>

class Scene : public QWidget
{
    Q_OBJECT
private:
    QVector<AbstractFigure*> figures;
    double x_min, x_max, y_min, y_max;
    bool rightButtonPressed;
    QPointF RBPressedPos;

    void drawAxes(QPainter &painter);
public:
    explicit Scene(QWidget *parent = nullptr);
    ~Scene();


    void appendFigure(AbstractFigure *figure);
    bool setArea(double x_min, double x_max,
                 double y_min, double y_max);
    void deleteLastFigure();
    void deleteAllFigures();
    int countFigures();
    double getAreaLastFigure();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void clicked(QPointF point);



};

#endif // SCENE_H
