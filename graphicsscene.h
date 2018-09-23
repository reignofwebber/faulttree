#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QtWidgets>



class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    GraphicsScene(QObject *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);




signals:
    void pressScene(const QPointF &);
};

#endif // GRAPHICSSCENE_H
