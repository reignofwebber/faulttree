#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QtWidgets>
#include <ellipseitem.h>

#include "graphicsscene.h"
#include "relation.h"

class Child;
class GraphicsScene;
class EllipseItem;

class Relation;


class NodeItem : public QObject
{
    Q_OBJECT

public:
    NodeItem(GraphicsScene *scene, const QString &id, QPointF pos = QPointF(0,0), const QString &formula = "all", bool value = true);
    ~NodeItem();

    void addParentRelation(Relation *arrow);
    void addChildRelation(Relation *arrow);

    void parentRelationReomved(Relation *relation);
    void childRelationRemoved(Relation *relation);

    QPointF centerPos() const;
    QString getNodeId() const;

    void setValue(bool value);
    void setInitalCaled();

    void updateValue(bool value);
    void setCaled(bool value);

    std::string id();
    std::string name();
    bool value();
    std::string formula();
    std::vector<Child> children();



private:
   void addToScene();

private slots:
   void onNodeMove(const QPointF &pos);



private:
    GraphicsScene *m_scene;

    QString m_id;
    bool val;
    qreal radius;

    EllipseItem *m_node;
    QGraphicsTextItem *m_nodeId;
    QGraphicsTextItem *m_nodeValue;
    QGraphicsTextItem *m_formula;

    QList<Relation *> m_parentRelations;
    QList<Relation *> m_childRelations;

};


#endif // GRAPHICSNODE_H
