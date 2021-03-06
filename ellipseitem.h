#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H


#include <QtWidgets>
#include <QDebug>

#include "graphicsnode.h"

class NodeItem;

class EllipseItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:

    enum { Type = UserType + 1 };

    EllipseItem(qreal x, qreal y, qreal r, NodeItem *node)
        : QGraphicsEllipseItem(0,0,2*r, 2*r), m_r(r), m_node(node)
    {
        setFlags(
                    QGraphicsItem::ItemIsSelectable
                    | QGraphicsItem::ItemIsMovable
                    | QGraphicsItem::ItemSendsGeometryChanges);
        setPos(x - r,y - r);
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if(change == GraphicsItemChange::ItemPositionChange)
        {
            emit nodeMove(scenePos() + QPointF(m_r, m_r));
        }
        return QGraphicsEllipseItem::itemChange(change, value);
    }

    int type() const override { return Type;}

    QPointF centerPos() const
    {
        return pos() + QPointF(m_r, m_r);
    }

    NodeItem *getNode() const
    {
        return m_node;
    }

signals:
    void nodeMove(const QPointF &);

private:
    qreal m_r;
    NodeItem *m_node;
};



#endif // ELLIPSEITEM_H
