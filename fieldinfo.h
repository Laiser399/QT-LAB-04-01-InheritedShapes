#ifndef FIELDINFO_H
#define FIELDINFO_H

#include <QSize>

struct FieldInfo {
    QSize sceneSize;
    double x_min, x_max, y_min, y_max;
    FieldInfo(const QSize &sceneSize, double x_min, double x_max,
              double y_min, double y_max) {
        this->sceneSize = sceneSize;
        this->x_min = x_min;
        this->x_max = x_max;
        this->y_min = y_min;
        this->y_max = y_max;
    }
 };

#endif // FIELDINFO_H
