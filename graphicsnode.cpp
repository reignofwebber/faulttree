#include "graphicsnode.h"


NodeItem::NodeItem(GraphicsScene *scene, const QString &id, QPointF pos, bool value)
    :m_scene(scene), id(id), value(true), radius(30)
{
    m_node = new EllipseItem(pos.x(), pos.y(), radius, this);
    m_nodeId = new QGraphicsTextItem(id);
    m_nodeValue = new QGraphicsTextItem(QString::number(value));
    m_formula = new QGraphicsTextItem("all");
    m_node->setBrush(Qt::white);

    m_nodeId->setFlags(QGraphicsItem::ItemIsSelectable);
    m_nodeId->setTextInteractionFlags(Qt::TextEditorInteraction);


    m_nodeValue->setFlags(QGraphicsItem::ItemIsSelectable);
    m_nodeValue->setTextInteractionFlags(Qt::TextEditorInteraction);

    m_formula->setFlags(QGraphicsItem::ItemIsSelectable);
    m_formula->setTextInteractionFlags(Qt::TextEditorInteraction);

    onNodeMove(pos);
    addToScene();


    connect(m_node, SIGNAL(nodeMove(QPointF)), this, SLOT(onNodeMove(QPointF)));

}

NodeItem::~NodeItem()
{
    m_scene->removeItem(m_node);
    m_scene->removeItem(m_nodeId);
    m_scene->removeItem(m_nodeValue);
    m_scene->removeItem(m_formula);

    //reserve for delete relation

    delete m_node;
    delete m_nodeId;
    delete m_nodeValue;
    delete m_formula;
}

void NodeItem::addParentRelation(Relation *relation)
{
    m_parentRelations.append(relation);
}

void NodeItem::addChildRelation(Relation *relation)
{
    m_childRelations.append(relation);
}

void NodeItem::parentRelationReomved(Relation *relation)
{
    m_parentRelations.removeOne(relation);
}

void NodeItem::childRelationRemoved(Relation *relation)
{
    m_childRelations.removeOne(relation);
}

void NodeItem::onNodeMove(const QPointF &pos)
{
    auto center(pos-m_nodeValue->boundingRect().center());
    m_nodeValue->setPos(center);

    center = pos-m_nodeId->boundingRect().center();
    m_nodeId->setPos(center.x(), center.y()+radius+m_nodeId->boundingRect().height()/2);

    center = pos-m_formula->boundingRect().center();
    m_formula->setPos(center.x(), center.y()-radius-m_formula->boundingRect().height()/2);

    //parent relation move
    foreach(Relation *relation, m_parentRelations)
    {
        QLineF lineCopy(relation->line());
        lineCopy.setP1(pos);
        relation->setLine(lineCopy);
    }

    //relation move
    foreach(Relation *relation, m_childRelations)
    {
        QLineF lineCopy(relation->line());
        lineCopy.setP2(pos);
        relation->setLine(lineCopy);
    }
}

void NodeItem::addToScene()
{
    m_scene->addItem(m_node);
    m_scene->addItem(m_nodeValue);
    m_scene->addItem(m_nodeId);
    m_scene->addItem(m_formula);
}

QPointF NodeItem::centerPos() const
{
    return m_node->centerPos();
}
