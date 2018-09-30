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

    connect(actionNew, SIGNAL(triggered()), m_scene, SLOT(newScene()));
    connect(actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(actionCursor,SIGNAL(toggled(bool)), m_scene, SLOT(setCurrentItem(bool)));
    connect(actionNode, SIGNAL(toggled(bool)), m_scene, SLOT(setCurrentItem(bool)));
    connect(actionLine,SIGNAL(toggled(bool)), m_scene, SLOT(setCurrentItem(bool)));
    connect(actionCalculate, SIGNAL(triggered()), m_scene, SLOT(calculate()));
    connect(actionClear, SIGNAL(triggered()), m_scene, SLOT(clear()));

    statusBar()->showMessage("Ready");
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open", ".", "*.json");
    if(!fileName.isEmpty())
    {
        m_scene->open(fileName);
    }
    statusBar()->showMessage("Ready");
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save", ".", "*.json");
    if(!fileName.isEmpty())
    {
        m_scene->save(fileName);
    }
    statusBar()->showMessage("Ready");
}

