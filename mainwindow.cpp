#include "mainwindow.h"
#include <QMenuBar>
#include <QDebug>
#include <QGridLayout>
#include <QPaintEvent>
#include <QVector>
#include <QListWidget>
#include <QGroupBox>
#include <QRadioButton>
#include <QSignalMapper>
#include <QColorDialog>
#include <QMessageBox>


void MainWindow::slot_author() {
    qDebug() << "author";
}

void MainWindow::slot_figureChanged(int id)
{
    currFigure = id;
    fourAnglesCounter = 0;

    for (int i = 0; i < 3; i++) {
        paramLabels[i]->hide();
        paramEdits[i]->hide();
    }

    if (id != 0) {
        paramLabels[0]->show();
        paramEdits[0]->show();
        if (id != 1) {
            paramLabels[1]->show();
            paramEdits[1]->show();
        }
        paramLabels[2]->show();
        paramEdits[2]->show();
    }
}

void MainWindow::slot_selectColor(int id)
{
    if (id == 0) {
        QColor new_col = QColorDialog::getColor(penColor);
        if (new_col.isValid())
            penColor = new_col;
    }
    else if (id == 1) {
        QColor new_col = QColorDialog::getColor(brushColor);
        if (new_col.isValid())
            brushColor = new_col;
    }
}

void MainWindow::slot_changeDisplayArea() {
    double values[4];
    bool ok;
    for (int i = 0; i < 4; i++) {
        values[i] = areaEdits[i]->text().toDouble(&ok);
        if (!ok) break;
    }
    if (!ok || (values[0] >= values[1]) || (values[2] >= values[3])) {
        QMessageBox::information(this, "Ошибка", "Введены неверные данные!");
        return;
    }

    scene->setArea(values[0], values[1], values[2], values[3]);
}

void MainWindow::slot_sceneClick(QPointF point) {
    bool ok1, ok2, ok3;
    if (currFigure == 0) {
        fourAnglesPoints[fourAnglesCounter] = point;
        fourAnglesCounter = (fourAnglesCounter + 1) % 4;
        if (fourAnglesCounter == 0) {
            scene->appendFigure(new FourAngles(fourAnglesPoints, penColor, brushColor));
        }
    }
    else {
        double width = paramEdits[0]->text().toDouble(&ok1);
        double height = paramEdits[1]->text().toDouble(&ok2);
        double angle = paramEdits[2]->text().toDouble(&ok3);
        if (currFigure == 1) {
            if (!ok1 || !ok3) {
                QMessageBox::information(this, "Ошибка", "Ошибка параметров фигуры!");
                return;
            }
            scene->appendFigure(new Square(point, width, angle, penColor, brushColor));
        }
        else if (currFigure == 2) {
            if (!ok1 || !ok2 || !ok3) {
                QMessageBox::information(this, "Ошибка", "Ошибка параметров фигуры!");
                return;
            }
            scene->appendFigure(new Rectangle(point, width, height, angle, penColor, brushColor));
        }
        else if (currFigure == 3) {
            if (!ok1 || !ok2 || !ok3) {
                QMessageBox::information(this, "Ошибка", "Ошибка параметров фигуры!");
                return;
            }
            scene->appendFigure(new Rhomb(point, width, height, angle, penColor, brushColor));
        }
    }
}

void MainWindow::slot_deleteLastFigure() {
    scene->deleteLastFigure();
}

void MainWindow::slot_deleteAllFigures() {
    scene->deleteAllFigures();
}

void MainWindow::slot_showAreaLastFigure() {
    if (scene->countFigures() > 0) {
        QMessageBox::information(this, "Площадь", QString("Площадь последней фигуры: ") + QString::number(scene->getAreaLastFigure()));
    }
    else {
        QMessageBox::information(this, "Ошибка", "Список фигур пуст!");
    }
}

void MainWindow::setMenu()
{
    QMenu *mFile = new QMenu("&Файл");
    mFile->addAction("&Выход", this, SLOT(close()));



    QMenu *mDelete = new QMenu("&Удаление");
    mDelete->addAction("Удалить &последнюю фигуру", this, SLOT(slot_deleteLastFigure()));
    mDelete->addAction("Удалить &все фигуры", this, SLOT(slot_deleteAllFigures()));

    QMenu *mInfo = new QMenu("&Инфо");
    mInfo->addAction("Площадь последней фигуры", this, SLOT(slot_showAreaLastFigure()));

    QMenu *mHelp = new QMenu("&?");
    mHelp->addAction("&Автор", this, SLOT(slot_author()));

    menuBar()->addMenu(mFile);
    menuBar()->addMenu(mDelete);
    menuBar()->addMenu(mInfo);
    menuBar()->addMenu(mHelp);
}

void MainWindow::setWidgets()
{
    QWidget *w = new QWidget;
    QHBoxLayout *hLay = new QHBoxLayout;
        QVBoxLayout *leftPanelLay = new QVBoxLayout;
            QGroupBox *boxFigure = new QGroupBox("Фигура");
                QVBoxLayout *boxFigLay = new QVBoxLayout;
                    QRadioButton *rb4Angles = new QRadioButton("Четырехугольник");
                    QRadioButton *rbSquare = new QRadioButton("Квадрат");
                    QRadioButton *rbRect = new QRadioButton("Прямоугольник");
                    QRadioButton *rbRhomb = new QRadioButton("Ромб");
            QGroupBox *boxParams = new QGroupBox("Параметры фигуры");
                paramsLay = new QGridLayout;
                    QPushButton *bPenColor = new QPushButton("Цвет пера");
                    QPushButton *bBrushColor = new QPushButton("Цвет кисти");
                    paramLabels[0] = new QLabel("Ширина");
                    paramLabels[1] = new QLabel("Высота");
                    paramLabels[2] = new QLabel("Угол поворота");
                    for (int i = 0; i < 3; i++)
                        paramEdits[i] = new QLineEdit;
            QGroupBox *boxDisplayArea = new QGroupBox("Область отображения");
                QGridLayout *displayAreaLay = new QGridLayout;
                    QLabel *areaLabel_x = new QLabel("Ось x(от, до):");
                    QLabel *areaLabel_y = new QLabel("Ось y(от, до):");
                    for (int i = 0; i < 4; i++)
                        areaEdits[i] = new QLineEdit;
                    QPushButton *areaChangeButton = new QPushButton("Изменить");
//            QGroupBox *boxInfo = new QGroupBox("Информация");
//                QGridLayout *infoLay = new QGridLayout;
//                    QLabel *infoAreaLabel = new QLabel("Площадь фигуры:");
//                    QLineEdit *areaFigure = new QLineEdit;
        scene = new Scene;


    setCentralWidget(w);
    w->setLayout(hLay);
        hLay->addLayout(leftPanelLay, 1);
            leftPanelLay->addWidget(boxFigure, 1);
                boxFigure->setLayout(boxFigLay);
                    boxFigLay->addWidget(rb4Angles);
                    boxFigLay->addWidget(rbSquare);
                    boxFigLay->addWidget(rbRect);
                    boxFigLay->addWidget(rbRhomb);
            leftPanelLay->addWidget(boxParams, 1);
                boxParams->setLayout(paramsLay);
                    paramsLay->addWidget(bPenColor, 0, 0);
                    paramsLay->addWidget(bBrushColor, 0, 1);
                    paramsLay->addWidget(paramLabels[0], 1, 0);
                    paramsLay->addWidget(paramLabels[1], 2, 0);
                    paramsLay->addWidget(paramLabels[2], 3, 0);
                    paramsLay->addWidget(paramEdits[0], 1, 1);
                    paramsLay->addWidget(paramEdits[1], 2, 1);
                    paramsLay->addWidget(paramEdits[2], 3, 1);
            leftPanelLay->addWidget(boxDisplayArea, 1);
                boxDisplayArea->setLayout(displayAreaLay);
                    displayAreaLay->addWidget(areaLabel_x, 0, 0);
                    displayAreaLay->addWidget(areaLabel_y, 1, 0);
                    displayAreaLay->addWidget(areaEdits[0], 0, 1);
                    displayAreaLay->addWidget(areaEdits[1], 0, 2);
                    displayAreaLay->addWidget(areaEdits[2], 1, 1);
                    displayAreaLay->addWidget(areaEdits[3], 1, 2);
                    displayAreaLay->addWidget(areaChangeButton, 2, 0, 1, 3);
//            leftPanelLay->addWidget(boxInfo);
//                boxInfo->setLayout(infoLay);
//                    infoLay->addWidget(infoAreaLabel, 0, 0);
//                    infoLay->addWidget(areaFigure, 0, 1);
                        //areaFigure->setReadOnly(true);
        hLay->addWidget(scene, 10);

    //boxFigure->setMaximumSize(140, 120);
    //boxFigure->setMinimumSize(140, 120);

    QSignalMapper *figureMapper = new QSignalMapper(this);
    connect(figureMapper, SIGNAL(mapped(int)), this, SLOT(slot_figureChanged(int)));
    connect(rb4Angles, SIGNAL(pressed()), figureMapper, SLOT(map()));
    connect(rbSquare, SIGNAL(pressed()), figureMapper, SLOT(map()));
    connect(rbRect, SIGNAL(pressed()), figureMapper, SLOT(map()));
    connect(rbRhomb, SIGNAL(pressed()), figureMapper, SLOT(map()));
    figureMapper->setMapping(rb4Angles, 0);
    figureMapper->setMapping(rbSquare, 1);
    figureMapper->setMapping(rbRect, 2);
    figureMapper->setMapping(rbRhomb, 3);
    rb4Angles->click();

    QSignalMapper *colorMapper = new QSignalMapper(this);
    connect(colorMapper, SIGNAL(mapped(int)), this, SLOT(slot_selectColor(int)));
    connect(bPenColor, SIGNAL(clicked()), colorMapper, SLOT(map()));
    connect(bBrushColor, SIGNAL(clicked()), colorMapper, SLOT(map()));
    colorMapper->setMapping(bPenColor, 0);
    colorMapper->setMapping(bBrushColor, 1);



    connect(areaChangeButton, SIGNAL(clicked()), this, SLOT(slot_changeDisplayArea()));

}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMenu();
    setWidgets();

    fourAnglesCounter = 0;
    fourAnglesPoints.resize(4);

    currFigure = 0;
    penColor = Qt::black;
    brushColor = Qt::white;

    paramEdits[0]->setText("2");
    paramEdits[1]->setText("3");
    paramEdits[2]->setText("0");

    connect(scene, SIGNAL(clicked(QPointF)), this, SLOT(slot_sceneClick(QPointF)));


}

MainWindow::~MainWindow()
{

}


