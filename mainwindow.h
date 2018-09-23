#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "ui_mainwindow.h"
#include "graphicsnode.h"
#include "graphicsscene.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    enum Mode
    {
        CURSOR,
        NODE,
        LINE
    };

public:
    MainWindow(QWidget *parent = 0);


public slots:
    void setCurrentItem(bool);
    void onScenePressed(const QPointF &);

private:
    QString getTempNodeId();


private:
    GraphicsScene *m_scene;
    Mode m_mode;
};

#endif // MAINWINDOW_H
