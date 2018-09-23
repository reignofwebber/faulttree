#include "graphicsscene.h"



GraphicsScene::GraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
//    GraphicsNode *node = new GraphicsNode(getTempNodeId());
//    DiagramTextItem *text = new DiagramTextItem("jjjj");
//    text->setFlag(QGraphicsItem::ItemIsSelectable);
//    text->setTextInteractionFlags(Qt::TextEditorInteraction);
//    addItem(text);


}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit pressScene(event->scenePos());
    QGraphicsScene::mousePressEvent(event);
}


