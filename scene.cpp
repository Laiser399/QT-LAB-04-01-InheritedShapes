#include "scene.h"
#include <QSize>
#include <QMouseEvent>
#include <cmath>

#include <QDebug>

Scene::Scene(QWidget *parent) : QWidget(parent)
{
    x_min = -10;
    x_max = 0;
    y_min = -10;
    y_max = 10;

    rightButtonPressed = false;

    //   TEST   //
    QVector<QPointF> p = { QPointF(-1, 1), QPointF(2, 2),
                           QPointF(3, -3), QPointF(-4, -4)};
    appendFigure(new FourAngles(p));
    //qDebug() << figures[0]->area();

    //appendFigure(new Square(QPointF(-1, -1), 3, 0.09));
    appendFigure(new Rectangle(QPointF(-5, -6), 4, 1, 0.5));
    appendFigure(new Rhomb(QPointF(-5, -6), 4, 1, 0.5));
}

Scene::~Scene()
{
    for (int i = 0; i < figures.size(); i++)
        delete figures[i];
}

void Scene::appendFigure(AbstractFigure *figure)
{
    figures.append(figure);
    update();
}

bool Scene::setArea(double x_min, double x_max,
                    double y_min, double y_max)
{
    if ((x_min >= x_max) || (y_min >= y_max))
        return false;
    this->x_min = x_min;
    this->x_max = x_max;
    this->y_min = y_min;
    this->y_max = y_max;
    update();
    return true;
}

void Scene::deleteLastFigure() {
    if (figures.size() > 0) {
        delete figures[figures.size() - 1];
        figures.removeLast();
    }
    update();
}

void Scene::deleteAllFigures() {
    for (int i = 0; i < figures.size(); i++)
        delete figures[i];
    figures.clear();
    update();
}

int Scene::countFigures() {
    return figures.size();
}

double Scene::getAreaLastFigure() {
    if (figures.size() > 0)
        return figures[figures.size() - 1]->area();
    else
        return -1;
}


double getNormalStep(double step) {
    int pow10 = 0;
    while (step >= 10) {
        step /= 10;
        pow10++;
    }
    while (step < 1) {
        step *= 10;
        pow10--;
    }
    double res_step;
    if (step < 1.5)
        res_step = 1;
    else if (step < 3.5)
        res_step = 2;
    else if (step < 7.5)
        res_step = 5;
    else
        res_step = 10;
    return res_step * pow(10, pow10);
}

#define STEP_COUNT 10
void Scene::drawAxes(QPainter &painter) {
    // оси координат
    painter.setPen(Qt::black);
    painter.drawLine(0, height() - 1, width(), height() - 1);
    painter.drawLine(0, 0, 0, height());

    double range_x = x_max - x_min, range_y = y_max - y_min;
    double step_x = range_x / STEP_COUNT, step_y = range_y / STEP_COUNT;
    step_x = getNormalStep(step_x);
    step_y = getNormalStep(step_y);

    painter.setBrush(QBrush(Qt::NoBrush));
    int i_x = x_min / step_x, i_x_max = x_max / step_x;
    while (i_x <= i_x_max) {
        double curr_x = step_x * i_x;
        int pos_x = (curr_x - x_min) / range_x * width();

        painter.setPen(QColor(0, 0, 0, 60));
        painter.drawLine(pos_x, 0, pos_x, height() - 19);
        painter.setPen(Qt::black);
        painter.drawLine(pos_x, height(), pos_x, height() - 4);
        QString printed_text;
        if (i_x == 0)
            printed_text = "0";
        else
            printed_text = QString::number(curr_x);

        int shift = painter.fontMetrics().width(printed_text);
        pos_x -= shift / 2;
        if ((pos_x > 1) && (pos_x <= width() - shift - 2))
            painter.drawText(pos_x, height() - 7, QString::number(curr_x));

        i_x++;
    }

    int i_y = y_min / step_y, i_y_max = y_max / step_y;
    while (i_y <= i_y_max) {
        double curr_y = step_y * i_y;
        int pos_y = (y_max - curr_y) / range_y * height();

        painter.setPen(QColor(0, 0, 0, 60));
        painter.drawLine(21, pos_y, width(), pos_y);
        painter.setPen(Qt::black);
        painter.drawLine(0, pos_y, 3, pos_y);
        QString printed_text;
        if (i_x == 0)
            printed_text = "0";
        else
            printed_text = QString::number(curr_y);

        int shift = painter.fontMetrics().height() / 2;
        if ((pos_y > shift + 1) && (pos_y < height() - shift - 1))
            painter.drawText(8, pos_y + shift - 2, printed_text);
        i_y++;
    }
}

void Scene::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setBrush(QBrush(Qt::white));
    painter.setPen(Qt::white);
    painter.drawRect(0, 0, this->width(), this->height());

    FieldInfo fInfo(QSize(width(), height()), x_min, x_max, y_min, y_max);

    painter.setPen(Qt::red);
    painter.setBrush(Qt::green);

    for (int i = 0; i < figures.size(); i++) {
        figures[i]->draw(&painter, fInfo);
    }

    drawAxes(painter);



    painter.end();
}

void Scene::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        double x = (double)event->x() / width() * (x_max - x_min) + x_min;
        double y = y_max - ((double)event->y() / height() * (y_max - y_min));
        emit clicked(QPointF(x, y));
    }
    else if (event->button() == Qt::RightButton) {
        rightButtonPressed = true;
        double x = (double)event->x() / width() * (x_max - x_min) + x_min;
        double y = y_max - ((double)event->y() / height() * (y_max - y_min));
        RBPressedPos = QPointF(x, y);
    }

}

void Scene::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        if (figures.size() > 0)
            figures.last()->confirmShift();
        rightButtonPressed = false;
        update();
    }
}

void Scene::mouseMoveEvent(QMouseEvent *event) {
    if (rightButtonPressed) {
        if (figures.size() == 0)
            return;
        double x = (double)event->x() / width() * (x_max - x_min) + x_min;
        double y = y_max - ((double)event->y() / height() * (y_max - y_min));
        QPointF shift = QPointF(x, y) - RBPressedPos;
        figures.last()->setShift(shift);
        update();
    }
}


