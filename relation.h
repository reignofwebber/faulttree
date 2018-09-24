#ifndef RELATION_H
#define RELATION_H


#include <QtWidgets>
#include "graphicsscene.h"
#include "graphicsnode.h"
#include "arrow.h"

class GraphicsScene;
class NodeItem;

class Relation
{

public:
    Relation(GraphicsScene *scene, NodeItem *parentNode, NodeItem *childNode, bool expectValue = true);
    ~Relation();



    void setLine(const QLineF &line);


    QLineF line() const
    {
        return m_arrow->line();
    }

private:
    void updateExpectPos(const QLineF &line);
    void addToScene();


private:
    GraphicsScene *m_scene;

    bool m_expectValue;

    Arrow *m_arrow;
    QGraphicsTextItem *m_expect;

    NodeItem *m_parentNode;
    NodeItem *m_childNode;

};




#endif // RELATION_H
