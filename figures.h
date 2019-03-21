#ifndef FIGURES_H
#define FIGURES_H

#include <QVector>
#include <QPoint>
#include <QPainter>
#include "fieldinfo.h"

class AbstractFigure {
protected:
    QColor penColor, brushColor;
    QPointF shift;
public:
    virtual void draw(QPainter *painter, FieldInfo &fInfo) = 0;
    virtual double area() = 0;
    void setShift(const QPointF &shift);
    virtual void confirmShift() = 0;
};

class FourAngles: public AbstractFigure {
protected:
    QVector<QPointF> points;
public:
    FourAngles(QVector<QPointF> &points, QColor penColor = Qt::black,
               QColor brushColor = Qt::white);
    void draw(QPainter *painter, FieldInfo &fInfo);
    double area();
    void confirmShift();
};

class Square: public AbstractFigure {
protected:
    double rotateAngle, width;
    QPointF center;
public:
    Square(const QPointF &center, double width, double rotateAngle = 0,
           QColor penColor = Qt::black, QColor brushColor = Qt::white);
    void draw(QPainter *painter, FieldInfo &fInfo);
    double area();
    void confirmShift();
};

class Rectangle: public Square {
protected:
    double height;
public:
    Rectangle(const QPointF &center, double width, double height, double rotateAngle = 0,
              QColor penColor = Qt::black, QColor brushColor = Qt::white);
    void draw(QPainter *painter, FieldInfo &fInfo);
    double area();
};

class Rhomb: public Rectangle {
public:
    Rhomb(const QPointF &center, double width, double height, double rotateAngle = 0,
          QColor penColor = Qt::black, QColor brushColor = Qt::white);
    void draw(QPainter *painter, FieldInfo &fInfo);
    double area();

};


#endif // FIGURES_H
