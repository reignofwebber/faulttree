#include "graphicsscene.h"

GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent),
      tmpLine(0),
      m_mode(CURSOR),
      m_tree(0)
{
//    GraphicsNode *node = new GraphicsNode(getTempNodeId());
//    DiagramTextItem *text = new DiagramTextItem("jjjj");
//    text->setFlag(QGraphicsItem::ItemIsSelectable);
//    text->setTextInteractionFlags(Qt::TextEditorInteraction);
//    addItem(text);
//    NodeItem *node = new NodeItem(this, getTempNodeId());
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

            Relation *relation = new Relation(this, endItem->getNode(), startItem->getNode());
//            QGraphicsLineItem * item = new QGraphicsLineItem(line);
//            startItem->addArrow(arrow);
//            endItem->addArrow(arrow);
//            arrow->setZValue(-1000.0);
            startItem->getNode()->addParentRelation(relation);
            endItem->getNode()->addChildRelation(relation);
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

void GraphicsScene::open(const QString &fileName)
{
    clear();
    m_tree = getFaultTree();
    if(m_tree->parse(fileName.toStdString()))
    {
        qDebug() << "success";
    }
    m_structure = m_tree->getStructure();
    m_treenodes = m_tree->getTreeModel();

    displayTree();
}

void GraphicsScene::calculate()
{
    if(m_tree == 0) return;
    //reset
    m_tree->reset();

    // 更新叶子结点
    for(auto itr = m_nodeItems.begin(); itr != m_nodeItems.end(); ++itr)
    {
        std::string id = itr->first;
        NodeItem *item = itr->second;
        if(m_treenodes.find(id) == m_treenodes.end())
        {
            qDebug() << "CALCULATE INPUT ERROR, .. CAN NOT FIND id " << QString::fromStdString(id) << " IGNORE this";
            continue;
        }
        FaultNode *node = m_treenodes[id];
        if(node->children.size() == 0)
        {
            m_tree->input(id, item->value());
            item->setValue(item->value());
        }
    }

    // 计算
    if(!m_tree->calculate(false)) return;
    for(auto itr = m_treenodes.begin(); itr != m_treenodes.end(); ++itr)
    {
        std::string id = itr->first;
        FaultNode *node = itr->second;
        if(node->caled)
        {
            if(m_nodeItems.find(id) == m_nodeItems.end())
            {
                qDebug() << "CALCULATE ERROR ... CAN NOT FIND id " << QString::fromStdString(id);
                return;
            }
            if(node->children.size() != 0)
            {
                m_nodeItems[id]->updateValue(node->value);
            }

        }
    }
}


void GraphicsScene::onScenePressed(const QPointF &pos)
{
    QString id;
    switch (m_mode) {
    case CURSOR:
        break;
    case NODE:
        id = getTempNodeId();
        if(m_nodeItems.find(id.toStdString()) != m_nodeItems.end())
        {
            qDebug() << "CREATE NODE ERROR ... DUPLICATED ID .. " << id;
            return;
        }
        m_nodeItems[id.toStdString()] = new NodeItem(this, id, pos);
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

void GraphicsScene::displayTree()
{
    size_t levelUnit = 200, columnUnit = 120;
    size_t columnSize = 0;
    for(auto itr = m_structure.begin();itr != m_structure.end(); ++itr)
    {
        if(columnSize < itr->second.size())
        {
            columnSize = itr->second.size();
        }
    }

    for(auto itr = m_structure.begin(); itr !=m_structure.end(); ++itr)
    {
        qreal x = levelUnit * itr->first;
        auto node_count = itr->second.size();
        for(size_t i = 0; i < node_count; ++i)
        {
            qreal y = columnUnit * columnSize * (i+1) /(node_count + 1);
            auto id = itr->second[i];
            //去除子节点重复
            if(m_nodeItems.find(id) != m_nodeItems.end()) continue;
            if(m_treenodes.find(id) == m_treenodes.end())
            {
                qDebug() << "Can not find id in model .. " << QString::fromStdString(id);
                m_nodeItems[id] = new NodeItem(this, QString::fromStdString(id), QPointF(x, y));
                m_nodeItems[id]->setInitalCaled();
            }else
            {
                auto qformula = QString::fromStdString(m_treenodes[id]->formula);
                auto name = m_treenodes[id]->name;
                QString qname = QString::fromStdString(id + "\n") + QString::fromStdString(name)/*QString::fromLocal8Bit(name.data())*/;
                m_nodeItems[id] = new NodeItem(this, qname, QPointF(x, y), qformula);

                if(m_treenodes[id]->caled) m_nodeItems[id]->setInitalCaled();
                if(m_treenodes[id]->children.size() == 0)
                {
                    m_tree->input(id, true);
                    m_nodeItems[id]->setValue(true);
                }
            }

        }

    }

    for(auto itr = m_nodeItems.begin(); itr != m_nodeItems.end(); ++itr)
    {
        std::string id = itr->first;
        if(m_treenodes.find(id) == m_treenodes.end())
        {
            qDebug() << "Can not find parent id in model .." << QString::fromStdString(id);
            continue;
        }
        for(const auto &child : m_treenodes[id]->children)
        {
            if(m_nodeItems.find(child.id) == m_nodeItems.end())
            {
                qDebug() << "Can not find child id in view .. " << QString::fromStdString(child.id);
            }
            Relation *relation = new Relation(this, m_nodeItems[id], m_nodeItems[child.id], child.expectValue);
            m_relations.push_back(relation);
            m_nodeItems[id]->addChildRelation(relation);
            m_nodeItems[child.id]->addParentRelation(relation);
        }
    }

}

void GraphicsScene::clear()
{
    if(m_tree != 0)
    {
        delete m_tree;
        m_tree = 0;
    }

    for(auto itr = m_nodeItems.begin(); itr != m_nodeItems.end(); ++itr)
    {
        delete itr->second;
    }
    m_nodeItems.clear();
}

void GraphicsScene::newScene()
{
    clear();
}

void GraphicsScene::save(const QString &fileName)
{
    if(m_tree != 0) qDebug() << "WARNING EXPORTSCENE TREE IS NOT NULL";
    else m_tree = getFaultTree();

    m_tree->reset();
    for(auto itr = m_nodeItems.begin();
        itr != m_nodeItems.end(); ++itr)
    {
        NodeItem *item = itr->second;
        FaultNode *node = new FaultNode;
        node->name = item->name();
        node->desc = "";
        node->caled = false;
        node->formula = item->formula();
        node->value = item->value();
        node->children = item->children();
        m_tree->addNode(item->id(), node);
    }
    // write to json file.
    m_tree->esrap(fileName.toStdString());
}







