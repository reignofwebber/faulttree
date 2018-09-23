#include "graphicsnode.h"


NodeItem::NodeItem(GraphicsScene *scene, const QString &id, bool value, QPointF pos)
    :m_scene(scene), id(id), value(true), radius(30)
{
    m_node = new EllipseItem(-radius, -radius, 2*radius,2*radius);
    m_nodeId = new QGraphicsTextItem(id);
    m_nodeValue = new QGraphicsTextItem(QString::number(value));


    m_nodeId->setFlags(QGraphicsItem::ItemIsSelectable);
    m_nodeId->setTextInteractionFlags(Qt::TextEditorInteraction);

    m_nodeValue->setFlags(QGraphicsItem::ItemIsSelectable);
    m_nodeValue->setTextInteractionFlags(Qt::TextEditorInteraction);

    onNodeMove(m_node->pos());
    addToScene();


    connect(m_node, SIGNAL(nodeMove(QPointF)), this, SLOT(onNodeMove(QPointF)));

}

void NodeItem::onNodeMove(const QPointF &pos)
{
//    auto scenePos(pos+QPointF(radius, radius));
    auto center(pos-m_nodeValue->boundingRect().center());
    m_nodeValue->setPos(center);
    center = pos-m_nodeId->boundingRect().center();
    m_nodeId->setPos(center.x(), center.y()+radius+m_nodeId->boundingRect().height()/2);
}

void NodeItem::addToScene()
{
    m_scene->addItem(m_node);
    m_scene->addItem(m_nodeValue);
    m_scene->addItem(m_nodeId);
}

