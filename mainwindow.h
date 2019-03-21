#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QGraphicsView>
#include "scene.h"
//#include "figures.h"
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void slot_author();
    void slot_figureChanged(int id);
    void slot_selectColor(int id);
    void slot_changeDisplayArea();
    void slot_sceneClick(QPointF point);
    void slot_deleteLastFigure();
    void slot_deleteAllFigures();
    void slot_showAreaLastFigure();
private:
    int fourAnglesCounter;
    QVector<QPointF> fourAnglesPoints;

    int currFigure;
    QColor penColor, brushColor;

    QGridLayout *paramsLay;
    // ширина, высота, угол поворота (последовательно)
    QLabel *paramLabels[3];
    QLineEdit *paramEdits[3];
    // параметры области отображения: minX, maxX, minY, maxY (последовательно)
    QLineEdit *areaEdits[4];

    Scene *scene;

    void setMenu();
    void setWidgets();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();


};

#endif // MAINWINDOW_H
