#ifndef ARROW_H
#define ARROW_H

#include <QtWidgets>


class Arrow : public QGraphicsLineItem
{
public:
    Arrow(const QLineF &line, QGraphicsItem *parent = 0)
        : QGraphicsLineItem(line, parent)
    {
        setFlags(QGraphicsItem::ItemIsSelectable);
    }
    Arrow(const QPointF &p1, const QPointF &p2, QGraphicsItem *parent = 0)
        : QGraphicsLineItem(QLineF(p1, p2), parent)
    {
        setFlags(QGraphicsItem::ItemIsSelectable);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);


        QPointF p1(line().p1()), p2(line().p2());
        QPointF startPos, endPos;

        double pi = acos(-1);
        startPos.setX(30*cos(line().angle()*pi/180) + p1.x());
        startPos.setY(-30*sin(line().angle()*pi/180) + p1.y());
        endPos.setX(-30*cos(line().angle()*pi/180) + p2.x());
        endPos.setY(30*sin(line().angle()*pi/180) + p2.y());

        QLineF exactLine(startPos, endPos);
        painter->drawLine(exactLine);

        QPointF circleCenterPos;
        circleCenterPos.setX(-4*cos(line().angle()*pi/180) + endPos.x());
        circleCenterPos.setY(4*sin(line().angle()*pi/180) + endPos.y());

        painter->setBrush(Qt::black);
        painter->drawEllipse(circleCenterPos,4,4);
        if(isSelected())
        {
            painter->setPen(QPen(Qt::black,1,Qt::DashLine));
            QLineF line = exactLine;
            line.translate(0,4.0);
            painter->drawLine(line);
            line.translate(0,-8.0);
            painter->drawLine(line);
        }
    }



private:


};


#endif // ARROW_H
