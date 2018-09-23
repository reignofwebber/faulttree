#include "graphicsnode.h"
#include <QPainter>




GraphicsNode::GraphicsNode(const QString &id, QPointF pos)
    :id(id), radius(30),position(pos)
{
    setPos(pos);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF GraphicsNode::boundingRect() const
{
    return QRectF(position.x()-3*radius, position.y()-radius, 6*radius, 3.5*radius);
}

void GraphicsNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawEllipse(position,radius,radius);
    QRectF elipseRect(position.x()-radius, position.y()-radius, 2*radius, 2*radius);
    painter->drawText(elipseRect, Qt::AlignHCenter | Qt::AlignVCenter, "jjj");
    QRectF descRect(elipseRect.adjusted(0,2*radius + 10,0,2*radius));
    painter->drawText(descRect, Qt::AlignHCenter, "TEST");
}

void GraphicsNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->pos() << "..." << mapToScene(event->pos());
    setPos(mapToScene(event->pos()));
}

void GraphicsNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->type() << "..MousePress";
    setCursor(Qt::ClosedHandCursor);
}

void GraphicsNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << event->type() << "..MouseRelease";
    setCursor(Qt::OpenHandCursor);
}

void GraphicsNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}

void GraphicsNode::setPosition(QPointF point)
{
    this->position = point;
    update();
}

QPointF GraphicsNode::getPosition()
{
    return position;
}
