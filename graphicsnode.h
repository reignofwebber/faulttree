#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QtWidgets>
#include <ellipseitem.h>

#include "graphicsscene.h"
#include "relation.h"

class GraphicsScene;
class EllipseItem;

class Relation;


class NodeItem : public QObject
{
    Q_OBJECT

public:
    NodeItem(GraphicsScene *scene, const QString &id, QPointF pos = QPointF(0,0), bool value = true);
    ~NodeItem();

    void addParentRelation(Relation *arrow);
    void addChildRelation(Relation *arrow);

    void parentRelationReomved(Relation *relation);
    void childRelationRemoved(Relation *relation);

    QPointF centerPos() const;

private:
   void addToScene();

private slots:
   void onNodeMove(const QPointF &pos);



private:
    GraphicsScene *m_scene;

    QString id;
    bool value;
    qreal radius;

    EllipseItem *m_node;
    QGraphicsTextItem *m_nodeId;
    QGraphicsTextItem *m_nodeValue;

    QList<Relation *> m_parentRelations;
    QList<Relation *> m_childRelations;

};


#endif // GRAPHICSNODE_H
