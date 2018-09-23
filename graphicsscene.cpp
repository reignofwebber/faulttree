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


QString GraphicsScene::getTempNodeId()
{
    static int i = 0;
    return QString("TEMP_") + QString::number(i++);
}
