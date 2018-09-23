#include "graphicsscene.h"



GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
//    GraphicsNode *node = new GraphicsNode(getTempNodeId());
//    DiagramTextItem *text = new DiagramTextItem("jjjj");
//    text->setFlag(QGraphicsItem::ItemIsSelectable);
//    text->setTextInteractionFlags(Qt::TextEditorInteraction);
//    addItem(text);


}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    onScenePressed(event->scenePos());
    QGraphicsScene::mousePressEvent(event);
}


void GraphicsScene::setCurrentItem(bool checked)
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


void GraphicsScene::onScenePressed(const QPointF &pos)
{
    NodeItem *node;
    switch (m_mode) {
    case CURSOR:
        break;
    case NODE:
        node = new NodeItem(this, getTempNodeId(), pos);
        qDebug() << "OnScenePressed .." << pos;
        break;
    default:
        break;
    }
}

QString GraphicsScene::getTempNodeId()
{
    static int i = 0;
    return QString("TEMP_") + QString::number(i++);
}
