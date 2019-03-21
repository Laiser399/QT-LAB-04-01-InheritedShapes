#include "figures.h"
#include <math.h>
#include <QDebug>

//   functions   |
QPointF rotatePoint(const QPointF &point, const double angle)
{
    // относительно 0 0
    double x = point.x() * cos(angle) - point.y() * sin(angle);
    double y = point.x() * sin(angle) + point.y() * cos(angle);
    return QPointF(x, y);
}

QPointF rotatePoint(const QPointF &rel_point, const QPointF &point, const double angle)
{
    // отсносительно rel_point
    double x1 = point.x() - rel_point.x();
    double y1 = point.y() - rel_point.y();
    double x = x1 * cos(angle) - y1 * sin(angle);
    double y = x1 * sin(angle) + y1 * cos(angle);
    return QPointF(x + rel_point.x(), y + rel_point.y());
}

double vectorLength(const QPointF &vec)
{
    return sqrt(vec.x() * vec.x() + vec.y() * vec.y());
}


//--------------------|
//   AbstractFigure   |
//--------------------|
void AbstractFigure::setShift(const QPointF &shift) {
    this->shift = shift;
}

//---------------|
//   FourAngle   |
//---------------|
FourAngles::FourAngles(QVector<QPointF> &points, QColor penColor, QColor brushColor)
{
    if (points.size() != 4)
        throw QString("FourAngles: constructor: number of points != 4!");

    this->points.resize(4);
    for (int i = 0; i < 4; i++)
        this->points[i] = points[i];
    this->penColor = penColor;
    this->brushColor = brushColor;
}

void FourAngles::draw(QPainter *painter, FieldInfo &fInfo)
{
    if (!painter->isActive())
        return;
    QVector<QPointF> new_points(4);
    double x_coef = fInfo.sceneSize.width() / (fInfo.x_max - fInfo.x_min),
           y_coef = fInfo.sceneSize.height() / (fInfo.y_max - fInfo.y_min);
    for (int i = 0; i < 4; i++) {
        new_points[i] = QPointF((points[i].x() + shift.x() - fInfo.x_min) * x_coef,
                                (fInfo.y_max - (points[i].y() + shift.y())) * y_coef);
    }
    painter->setPen(penColor);
    painter->setBrush(brushColor);
    painter->drawPolygon(QPolygonF(new_points));
}

double FourAngles::area()
{
    QPointF v1 = points[0] - points[2],
            v2 = points[1] - points[3];
    double d1 = vectorLength(v1), d2 = vectorLength(v2);
    double cos_a = (v1.x() * v2.x() + v1.y() * v2.y()) / (d1 * d2);
    double sin_a = sqrt(1 - cos_a * cos_a);
    Q_ASSERT(sin_a >= 0);
    return 0.5 * d1 * d2 * sin_a;
}

void FourAngles::confirmShift() {
    for (auto it = points.begin(); it != points.end(); ++it)
        *it += shift;
    shift = QPointF(0, 0);
}

//------------|
//   Square   |
//------------|
Square::Square(const QPointF &center, double width, double rotateAngle,
               QColor penColor, QColor brushColor)
{
    this->center = center;
    this->width = width;
    this->rotateAngle = rotateAngle;
    this->penColor = penColor;
    this->brushColor = brushColor;
}

void Square::draw(QPainter *painter, FieldInfo &fInfo) {
    double x_coef = fInfo.sceneSize.width() / (fInfo.x_max - fInfo.x_min),
           y_coef = fInfo.sceneSize.height() / (fInfo.y_max - fInfo.y_min);
    painter->setPen(penColor);
    painter->setBrush(brushColor);
    if (rotateAngle != 0) {
        QVector<QPointF> points(4);
        points[0] = QPointF(center.x() - width / 2, center.y() + width / 2);
        points[1] = QPointF(center.x() + width / 2, center.y() + width / 2);
        points[2] = QPointF(center.x() + width / 2, center.y() - width / 2);
        points[3] = QPointF(center.x() - width / 2, center.y() - width / 2);
        for (int i = 0; i < 4; i++) {
            points[i] = rotatePoint(center, points[i], rotateAngle);
            points[i] = QPointF((points[i].x() + shift.x() - fInfo.x_min) * x_coef,
                                (fInfo.y_max - (points[i].y() + shift.y())) * y_coef);
        }
        painter->drawPolygon(QPolygonF(points));
    }
    else {
        QPointF new_center((center.x() + shift.x() - fInfo.x_min) * x_coef,
                           (fInfo.y_max - (center.y() + shift.y())) * y_coef);
        double new_width = width * x_coef;
        double new_height = width * y_coef;
        painter->drawRect(new_center.x() - new_width / 2, new_center.y() - new_height / 2,
                          new_width, new_height);
    }
}

double Square::area()
{
    return width * width;
}

void Square::confirmShift() {
    center += shift;
    shift = QPointF(0, 0);
}

//---------------|
//   Rectangle   |
//---------------|
Rectangle::Rectangle(const QPointF &center, double width,
                     double height, double rotateAngle,
                     QColor penColor, QColor brushColor):
    Square(center, width, rotateAngle, penColor, brushColor)
{
    this->height = height;
}

void Rectangle::draw(QPainter *painter, FieldInfo &fInfo)
{
    double x_coef = fInfo.sceneSize.width() / (fInfo.x_max - fInfo.x_min),
           y_coef = fInfo.sceneSize.height() / (fInfo.y_max - fInfo.y_min);
    painter->setPen(penColor);
    painter->setBrush(brushColor);
    if (rotateAngle != 0) {
        QVector<QPointF> points(4);
        points[0] = QPointF(center.x() - width / 2, center.y() + height / 2);
        points[1] = QPointF(center.x() + width / 2, center.y() + height / 2);
        points[2] = QPointF(center.x() + width / 2, center.y() - height / 2);
        points[3] = QPointF(center.x() - width / 2, center.y() - height / 2);
        for (int i = 0; i < 4; i++) {
            points[i] = rotatePoint(center, points[i], rotateAngle);
            points[i] = QPointF((points[i].x() + shift.x() - fInfo.x_min) * x_coef,
                                (fInfo.y_max - (points[i].y() + shift.y())) * y_coef);
        }
        painter->drawPolygon(QPolygonF(points));
    }
    else {
        QPointF new_center((center.x() + shift.x() - fInfo.x_min) * x_coef,
                           (fInfo.y_max - (center.y() + shift.y())) * y_coef);
        double new_width = width * x_coef;
        double new_height = height * y_coef;
        painter->drawRect(new_center.x() - new_width / 2, new_center.y() - new_height / 2,
                          new_width, new_height);
    }
}

double Rectangle::area()
{
    return width * height;
}


//-----------|
//   Rhomb   |
//-----------|
Rhomb::Rhomb(const QPointF &center, double width,
             double height, double rotateAngle,
             QColor penColor, QColor brushColor):
    Rectangle(center, width, height, rotateAngle, penColor, brushColor) {}

void Rhomb::draw(QPainter *painter, FieldInfo &fInfo)
{
    double x_coef = fInfo.sceneSize.width() / (fInfo.x_max - fInfo.x_min),
           y_coef = fInfo.sceneSize.height() / (fInfo.y_max - fInfo.y_min);
    QVector<QPointF> points(4);
    points[0] = QPointF(center.x() - width / 2, center.y());
    points[1] = QPointF(center.x(), center.y() + height / 2);
    points[2] = QPointF(center.x() + width / 2, center.y());
    points[3] = QPointF(center.x(), center.y() - height / 2);
    if (rotateAngle != 0)
        for (int i = 0; i < 4; i++)
            points[i] = rotatePoint(center, points[i], rotateAngle);
    for (int i = 0; i < 4; i++)
        points[i] = QPointF((points[i].x() + shift.x()- fInfo.x_min) * x_coef,
                            (fInfo.y_max - (points[i].y() + shift.y())) * y_coef);
    painter->setPen(penColor);
    painter->setBrush(brushColor);
    painter->drawPolygon(QPolygonF(points));
}

double Rhomb::area()
{
    return 0.5 * width * height;
}









