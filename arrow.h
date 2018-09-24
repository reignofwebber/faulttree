#ifndef ARROW_H
#define ARROW_H

#include <QtWidgets>


class Arrow : public QGraphicsLineItem
{
public:
    Arrow(const QLineF &line, QGraphicsItem *parent = 0)
        : QGraphicsLineItem(line, parent)
    {

    }
    Arrow(const QPointF &p1, const QPointF &p2, QGraphicsItem *parent = 0)
        : QGraphicsLineItem(QLineF(p1, p2), parent)
    {

    }
};


#endif // ARROW_H
