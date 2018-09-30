#include "relation.h"

Relation::Relation(GraphicsScene *scene, NodeItem *parentNode, NodeItem *childNode, bool expectValue)
    :m_scene(scene), m_expectValue(expectValue), m_parentNode(parentNode), m_childNode(childNode)
{
    QLineF line(childNode->centerPos(), parentNode->centerPos());
    m_arrow = new Arrow(this, line);
    m_arrow->setZValue(-200);

    m_expect = new QGraphicsTextItem(QString::number(expectValue));
    m_expect->setFlags(QGraphicsItem::ItemIsSelectable);
    m_expect->setTextInteractionFlags(Qt::TextEditorInteraction);
    m_expect->setZValue(-100);
    updateExpectPos(line);

    addToScene();
}

Relation::~Relation()
{
    m_scene->removeItem(m_arrow);
    m_scene->removeItem(m_expect);

    m_parentNode->childRelationRemoved(this);
    m_childNode->parentRelationReomved(this);

    delete m_arrow;
    delete m_expect;
}

void Relation::setLine(const QLineF &line)
{
    m_arrow->setLine(line);
    updateExpectPos(line);
}

void Relation::updateExpectPos(const QLineF &line)
{
    QPointF center((line.p1()+line.p2())/2);
    m_expect->setPos(center-m_expect->boundingRect().center());
}

void Relation::addToScene()
{
    m_scene->addItem(m_arrow);
    m_scene->addItem(m_expect);
}

std::string Relation::childId()
{
    return m_childNode->id();
}

bool Relation::expectValue()
{
    QVariant tmp = m_expect->toPlainText();
    return tmp.toBool();
}
