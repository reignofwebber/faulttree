#include "graphicsscene.h"



GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent),tmpLine(0),m_mode(CURSOR)
{
//    GraphicsNode *node = new GraphicsNode(getTempNodeId());
//    DiagramTextItem *text = new DiagramTextItem("jjjj");
//    text->setFlag(QGraphicsItem::ItemIsSelectable);
//    text->setTextInteractionFlags(Qt::TextEditorInteraction);
//    addItem(text);
    NodeItem *node = new NodeItem(this, getTempNodeId());

}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    onScenePressed(event->scenePos());
    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(tmpLine != 0 && m_mode == LINE)
    {
        QLineF line(tmpLine->line().p1(), event->scenePos());
        tmpLine->setLine(line);
    }
    else if(m_mode == CURSOR)
    {
        QGraphicsScene::mouseMoveEvent(event);
    }
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(tmpLine != 0 && m_mode == LINE)
    {
        QList<QGraphicsItem *> startItems = items(tmpLine->line().p1());
        if (startItems.count() && startItems.first() == tmpLine)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(tmpLine->line().p2());

        if (endItems.count() && endItems.first() == tmpLine)
            endItems.removeFirst();

        removeItem(tmpLine);
        delete tmpLine;
        tmpLine = 0;



        if(startItems.count() > 0 && endItems.count() > 0 &&
                startItems.first()->type() == EllipseItem::Type &&
                endItems.first()->type() == EllipseItem::Type &&
                startItems.first() != endItems.first())
        {
            EllipseItem *startItem = qgraphicsitem_cast<EllipseItem *>(startItems.first());
            EllipseItem *endItem = qgraphicsitem_cast<EllipseItem *>(endItems.first());

            QLineF line(startItem->centerPos(), endItem->centerPos());
            Arrow *arrow = new Arrow(line);
//            QGraphicsLineItem * item = new QGraphicsLineItem(line);
//            startItem->addArrow(arrow);
//            endItem->addArrow(arrow);
//            arrow->setZValue(-1000.0);
            startItem->getNode()->setParentRelation(arrow);
            endItem->getNode()->addChildRelation(arrow);
            addItem(arrow);
//            addItem(item);
        }

    }
    QGraphicsScene::mouseReleaseEvent(event);
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
    case LINE:
        tmpLine = new QGraphicsLineItem(QLineF(pos,pos));
                   tmpLine->setPen(QPen(Qt::red, 2));
                   addItem(tmpLine);
    default:
        break;
    }
}

QString GraphicsScene::getTempNodeId()
{
    static int i = 0;
    return QString("TEMP_") + QString::number(i++);
}
