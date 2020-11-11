#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QMap>
#include "node.h"
#include "edge.h"
//class Node;

//! [0]
class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
//    ~GraphWidget() override;

    void itemMoved();

    void addNode(QString name,int nodeType=NODE_TYPE_NEURON);
    void addEdge(QString node1,QString node2,int edgeType=EDGE_TYPE_NEURON);
    void setCenterNode(QString name);
    void clear();
    void setpos(const QString name,qreal x,qreal y);
    void showlayer(int layer);

public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();    

signals:
    void SIGNAL_showlayer(int layer);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;
#endif
   // void drawBackground(QPainter *painter, const QRectF &rect) override;

    void scaleView(qreal scaleFactor);

private:
    int timerId;
    Node *centerNode;
    QMap<QString,Node*> nodesMap;
};
//! [0]

#endif // GRAPHWIDGET_H
