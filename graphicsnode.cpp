#include "graphicsnode.h"


NodeItem::NodeItem(GraphicsScene *scene, const QString &id, QPointF pos, bool value)
    :m_scene(scene), id(id), value(true), radius(30), m_parentRelation(0)
{
    qDebug() << "Should.." << pos;
    m_node = new EllipseItem(pos.x(), pos.y(), radius, this);
    m_nodeId = new QGraphicsTextItem(id);
    m_nodeValue = new QGraphicsTextItem(QString::number(value));


    m_nodeId->setFlags(QGraphicsItem::ItemIsSelectable);
    m_nodeId->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_nodeId->setZValue(-100);

    m_nodeValue->setFlags(QGraphicsItem::ItemIsSelectable);
    m_nodeValue->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_nodeValue->setZValue(-100);

    onNodeMove(pos);
    addToScene();


    connect(m_node, SIGNAL(nodeMove(QPointF)), this, SLOT(onNodeMove(QPointF)));

}


void NodeItem::setParentRelation(Arrow *arrow)
{
    if(m_parentRelation)
    {
        m_scene->removeItem(m_parentRelation);
        delete m_parentRelation;
    }
    m_parentRelation = arrow;
}

void NodeItem::addChildRelation(Arrow *arrow)
{
    m_childRelation.append(arrow);
}

void NodeItem::onNodeMove(const QPointF &pos)
{
    auto center(pos-m_nodeValue->boundingRect().center());
    m_nodeValue->setPos(center);
    center = pos-m_nodeId->boundingRect().center();
    m_nodeId->setPos(center.x(), center.y()+radius+m_nodeId->boundingRect().height()/2);


    //parent relation move
    if(m_parentRelation)
    {
        QLineF lineCopy(m_parentRelation->line());
        lineCopy.setP1(pos);
        m_parentRelation->setLine(lineCopy);
    }

    //relation move
    foreach(Arrow *arrow, m_childRelation)
    {
        QLineF lineCopy(arrow->line());
        lineCopy.setP2(pos);
        arrow->setLine(lineCopy);

    }
}

void NodeItem::addToScene()
{
    m_scene->addItem(m_node);
    m_scene->addItem(m_nodeValue);
    m_scene->addItem(m_nodeId);
}

