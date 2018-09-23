#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    QActionGroup *toolsGroup = new QActionGroup(toolBar);
    toolsGroup->addAction(actionCursor);
    toolsGroup->addAction(actionNode);
    toolsGroup->addAction(actionLine);

    QGraphicsScene *scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);

    GraphicsNode *node = new GraphicsNode("jjj");
    scene->addItem(node);


    connect(actionCursor,SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));
    connect(actionNode, SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));
    connect(actionLine,SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));
}

void MainWindow::setCurrentItem(bool checked)
{

}
