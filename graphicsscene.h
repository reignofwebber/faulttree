#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QtWidgets>
#include <map>
#include <string>
#include "graphicsnode.h"
#include "relation.h"

#include <faulttree.h>

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

    void open(const QString &fileName);
    void save(const QString &fileName);

public slots:

    void setCurrentItem(bool checked);
    void calculate();
    void clear();
    void newScene();
    void deleteItem();

private:
    void displayTree();


signals:
    void pressScene(const QPointF &);
private:
    QGraphicsLineItem *tmpLine;
    Mode m_mode;

    IFaultTree *m_tree;
    std::map<std::string, FaultNode *> m_treenodes;
    std::map<int, std::vector<std::string>> m_structure;
    std::map<std::string, NodeItem *> m_nodeItems;
    std::vector<Relation *> m_relations;
};

#endif // GRAPHICSSCENE_H
