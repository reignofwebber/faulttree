#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QtWidgets>
#include <ellipseitem.h>

#include "graphicsscene.h"
#include "arrow.h"

class GraphicsScene;
class EllipseItem;


class NodeItem : public QObject
{
    Q_OBJECT

public:
    NodeItem(GraphicsScene *scene, const QString &id, QPointF pos = QPointF(0,0), bool value = true);

    void setParentRelation(Arrow *arrow);
    void addChildRelation(Arrow *arrow);

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

    Arrow * m_parentRelation;
    QList<Arrow *> m_childRelation;

};


#endif // GRAPHICSNODE_H
