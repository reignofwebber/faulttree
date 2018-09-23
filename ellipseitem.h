#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H


#include <QtWidgets>
#include <QDebug>

class EllipseItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    EllipseItem(qreal x, qreal y, qreal r)
        : QGraphicsEllipseItem(0,0,2*r, 2*r), m_r(r)
    {
        setFlags(
                    QGraphicsItem::ItemIsSelectable
                    | QGraphicsItem::ItemIsMovable
                    | QGraphicsItem::ItemSendsGeometryChanges);
        qDebug() << "customELlipse" << ".." << x << ".." << y;
        setPos(x - r,y - r);
        qDebug() << "position  " << pos() << ".." << scenePos();
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if(change == GraphicsItemChange::ItemPositionChange)
        {
            emit nodeMove(scenePos() + QPointF(m_r, m_r));
        }
        return QGraphicsEllipseItem::itemChange(change, value);
    }

signals:
    void nodeMove(const QPointF &);

private:
    qreal m_r;
};



#endif // ELLIPSEITEM_H
