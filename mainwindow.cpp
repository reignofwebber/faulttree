#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    QActionGroup *toolsGroup = new QActionGroup(toolBar);
    toolsGroup->addAction(actionCursor);
    toolsGroup->addAction(actionNode);
    toolsGroup->addAction(actionLine);

    m_scene = new GraphicsScene(this);
    graphicsView->setScene(m_scene);


//    DiagramTextItem * text = new DiagramTextItem("abc");
//    m_scene->addItem(text);

    NodeItem *item = new NodeItem(m_scene, "TEST");

    connect(actionCursor,SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));
    connect(actionNode, SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));
    connect(actionLine,SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));
}

void MainWindow::setCurrentItem(bool checked)
{

}
