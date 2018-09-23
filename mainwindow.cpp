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

    NodeItem *item = new NodeItem(m_scene, getTempNodeId());

    connect(actionCursor,SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));
    connect(actionNode, SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));
    connect(actionLine,SIGNAL(toggled(bool)), this, SLOT(setCurrentItem(bool)));

    connect(m_scene, SIGNAL(pressScene(QPointF)), this, SLOT(onScenePressed(QPointF)));
}

void MainWindow::setCurrentItem(bool checked)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action->text() == "Cursor" && checked)
    {
        m_mode = CURSOR;
    }else if(action->text() == "Node" && checked)
    {
        m_mode = NODE;
    }else if(action->text() == "Line" && checked)
    {
        m_mode = LINE;
    }
}


void MainWindow::onScenePressed(const QPointF &pos)
{
    NodeItem *node;
    switch (m_mode) {
    case CURSOR:
        break;
    case NODE:
        node = new NodeItem(m_scene, getTempNodeId(), pos);
        qDebug() << "OnScenePressed .." << pos;
        break;
    default:
        break;
    }
}

QString MainWindow::getTempNodeId()
{
    static int i = 0;
    return QString("TEMP_") + QString::number(i++);
}
