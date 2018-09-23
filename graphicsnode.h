#ifndef GRAPHICSNODE_H
#define GRAPHICSNODE_H

#include <QtWidgets>




class GraphicsNode : public QGraphicsItem
{
public:
    GraphicsNode(const QString &id, QPointF pos = QPointF(0,0));

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public:
    void setPosition(QPointF);
    QPointF getPosition();

private:
    QString id;
    QTextItem text_id;

    qreal radius;
    QPointF position;
};


#endif // GRAPHICSNODE_H
