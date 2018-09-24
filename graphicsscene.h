#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QtWidgets>

#include "graphicsnode.h"
#include "arrow.h"

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode
    {
        CURSOR,
        NODE,
        LINE
    };

public:
    GraphicsScene(QObject *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


    QString getTempNodeId();
    void onScenePressed(const QPointF &pos);

public slots:

    void setCurrentItem(bool checked);


signals:
    void pressScene(const QPointF &);
private:
    QGraphicsLineItem *tmpLine;
    Mode m_mode;
};

#endif // GRAPHICSSCENE_H
