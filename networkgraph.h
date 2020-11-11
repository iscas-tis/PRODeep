#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include <QObject>
#include <QWidget>
#include "graphwidget.h"
#include "node.h"

namespace Ui {
class NetworkGraph;
}

class NetworkGraph : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkGraph(QWidget *parent = nullptr);
    ~NetworkGraph();
    void addNode(QString name);
    void addEdge(QString name1,QString name2);
    void clear();

signals:
    void SIGNAL_showlayer(int layer);

public slots:
    void showlayer(int layer);

private:
    Ui::NetworkGraph *ui;
    GraphWidget *graph;
};

#endif // NETWORKGRAPH_H
