#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QtWidgets>

#include "graphicsnode.h"

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


    QString getTempNodeId();
    void onScenePressed(const QPointF &pos);

public slots:

    void setCurrentItem(bool checked);


signals:
    void pressScene(const QPointF &);
private:
    Mode m_mode;
};

#endif // GRAPHICSSCENE_H
