#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H


#include <QtWidgets>
#include <QDebug>

class EllipseItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    EllipseItem(qreal x, qreal y, qreal w, qreal h)
        : QGraphicsEllipseItem(x,y,w,h)
    {
        setFlags(
                    QGraphicsItem::ItemIsSelectable
                    | QGraphicsItem::ItemIsMovable
                    | QGraphicsItem::ItemSendsGeometryChanges);
        qDebug() << "customELlipse";
    }

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        qDebug() << change << "///" << value;
        if(change == GraphicsItemChange::ItemPositionChange)
        {
            emit nodeMove(value.toPointF());
        }
        return QGraphicsEllipseItem::itemChange(change, value);
    }

signals:
    void nodeMove(const QPointF &);
};



#endif // ELLIPSEITEM_H
