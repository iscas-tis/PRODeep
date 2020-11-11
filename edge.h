#ifndef EDGE_H
#define EDGE_H

#define EDGE_TYPE_NEURON 0
#define EDGE_TYPE_LAYER 1

#include <QGraphicsItem>

class Node;

//! [0]
class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode, int layertype=EDGE_TYPE_NEURON);

    Node *sourceNode() const;
    Node *destNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Node *source, *dest;
    int edgetype;

    QPointF sourcePoint;
    QPointF destPoint;
    qreal arrowSize;
};
//! [0]

#endif // EDGE_H
