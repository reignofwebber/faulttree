#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "ui_mainwindow.h"
#include "graphicsnode.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);


public slots:
    void setCurrentItem(bool);




private:
};

#endif // MAINWINDOW_H
