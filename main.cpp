#include <QApplication>
#include <QGraphicsScene>
#include <QtWidgets>

#include "mainwindow.h"

#include <iostream>
#include <faulttree.h>


int main(int argc, char **argv)
{
    QApplication app(argc, argv);


    IFaultTree *tree = getFaultTree();


    std::map<std::string, FaultNode *> m_nodes = tree->getTreeModel();


    MainWindow w;

    w.showMaximized();

    return app.exec();
}
