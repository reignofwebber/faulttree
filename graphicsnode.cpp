#include "graphicsnode.h"

NodeItem::NodeItem(GraphicsScene *scene, const QString &id, QPointF pos, const QString &formula, bool value)
    :m_scene(scene), m_id(id), val(value), radius(30)
{
    m_node = new EllipseItem(pos.x(), pos.y(), radius, this);
    m_nodeId = new QGraphicsTextItem(m_id);
    m_nodeValue = new QGraphicsTextItem(QString::number(val));
    m_formula = new QGraphicsTextItem(formula);
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
    foreach (Relation *relation , m_childRelations)
    {
        delete relation;
    }

    foreach (Relation *relation , m_parentRelations)
    {
        delete relation;
    }

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

QString NodeItem::getNodeId() const
{
    return m_nodeId->toPlainText();
}

void NodeItem::setValue(bool value)
{
    m_nodeValue->setPlainText(QString::number(value));
    setInitalCaled();
}

void NodeItem::setInitalCaled()
{
    m_node->setBrush(QColor(167,205,35));
}

void NodeItem::updateValue(bool value)
{
    m_nodeValue->setPlainText(QString::number(value));
    setCaled(value);
}

void NodeItem::setCaled(bool value)
{
    if(value) m_node->setBrush(Qt::green);
    else m_node->setBrush(Qt::red);
}

bool NodeItem::value()
{
    QVariant tmpValue = m_nodeValue->toPlainText();
    return tmpValue.toBool();
}

std::string NodeItem::id()
{
    QString str = m_nodeId->toPlainText();
    QStringList strs = str.split("\n");
    return strs[0].toStdString();
}

std::string NodeItem::name()
{
    QString str = m_nodeId->toPlainText();
    int newlinePos = str.indexOf('\n');
    if(newlinePos == -1) return "";
    else return str.mid(newlinePos + 1).toStdString();
}

std::string NodeItem::formula()
{
    return m_formula->toPlainText().toStdString();
}

std::vector<Child> NodeItem::children()
{
    std::vector<Child> t_children;
    foreach(Relation *relation , m_childRelations)
    {
        Child child;
        child.expectValue = relation->expectValue();
        child.id = relation->childId();
        t_children.push_back(child);
    }
    return t_children;
}

