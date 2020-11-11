#include "edge.h"
#include "node.h"
#include "graphwidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
//QRandom Generator is only available Qt 5.10+
//#include <QRandomGenerator>

//! [0]
Node::Node(QString name,GraphWidget *graphWidget, int nodeType)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    this->name = name;
    this->nodeType=nodeType;
    this->isGroundtruth=true;    

}
//! [0]

//! [1]
void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

void Node::resetGroundTruth()
{
    this->isGroundtruth=false;
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}
//! [1]

//! [2]
void Node::calculateForces()
{
//    if (!scene() || scene()->mouseGrabberItem() == this) {
//        newPos = pos();
//        return;
//    }
//! [2]

//! [3]
    // Sum up all forces pushing this item away
//    qreal xvel = 0;
//    qreal yvel = 0;
//    const QList<QGraphicsItem *> items = scene()->items();
//    for (QGraphicsItem *item : items) {
//        Node *node = qgraphicsitem_cast<Node *>(item);
//        if (!node)
//            continue;

//        QPointF vec = mapToItem(node, 0, 0);
//        qreal dx = vec.x();
//        qreal dy = vec.y();
//        double l = 2.0 * (dx * dx + dy * dy);
//        if (l > 0) {
//            xvel += (dx * 150.0) / l;
//            yvel += (dy * 150.0) / l;
//        }
//    }
//! [3]

//! [4]
    // Now subtract all forces pulling items together
//    double weight = (edgeList.size() + 1) * 10;
//    for (const Edge *edge : qAsConst(edgeList)) {
//        QPointF vec;
//        if (edge->sourceNode() == this)
//            vec = mapToItem(edge->destNode(), 0, 0);
//        else
//            vec = mapToItem(edge->sourceNode(), 0, 0);
//        xvel -= vec.x() / weight;
//        yvel -= vec.y() / weight;
//    }
//! [4]

//! [5]
//    if (qAbs(xvel) < 2 && qAbs(yvel) < 2)
//        xvel = yvel = 0;
//! [5]

//! [6]
//!
//! No Pushing or Pulling Effect!
//    QRectF sceneRect = scene()->sceneRect();
//    newPos = pos() + QPointF(xvel, yvel);
//    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
//    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}
//! [6]

//! [7]
bool Node::advancePosition()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}
//! [7]

//! [8]
QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    if(this->nodeType==NODE_TYPE_NEURON)return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
    if(this->nodeType==NODE_TYPE_LAYER)return QRectF(-20 - adjust, -30 - adjust , 43+ adjust, 63+ adjust);
    return QRectF(-20 - adjust, -30 - adjust , 43+ adjust, 63+ adjust);
}
//! [8]

//! [9]
QPainterPath Node::shape() const
{
    QPainterPath path;
    if(this->nodeType==NODE_TYPE_NEURON)path.addEllipse(-10, -10, 20, 20);
    if(this->nodeType==NODE_TYPE_LAYER)path.addRect(-20, -30, 40, 60);
    return path;
}
//! [9]

//! [10]
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(this->nodeType==NODE_TYPE_NEURON)
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::darkGray);
        painter->drawEllipse(-7, -7, 20, 20);

        QRadialGradient gradient(-3, -3, 10);
        if (option->state & QStyle::State_Sunken) {
            gradient.setCenter(3, 3);
            gradient.setFocalPoint(3, 3);
            if(!this->isGroundtruth)gradient.setColorAt(1, QColor(Qt::yellow).lighter(120));else gradient.setColorAt(1, QColor(Qt::green).lighter(120));
            if(!this->isGroundtruth)gradient.setColorAt(0, QColor(Qt::darkYellow).lighter(120));else gradient.setColorAt(0, QColor(Qt::darkGreen).lighter(120));
        } else {
            if(!this->isGroundtruth)gradient.setColorAt(0, Qt::yellow);else gradient.setColorAt(0, Qt::green);
            if(!this->isGroundtruth)gradient.setColorAt(1, Qt::darkYellow);else gradient.setColorAt(1, Qt::darkGreen);
        }
        painter->setBrush(gradient);

        painter->setPen(QPen(Qt::black, 0));
        painter->drawEllipse(-10, -10, 20, 20);

        QFont font1;
        font1.setBold(true);
        font1.setPointSize(5);
        painter->setBrush(Qt::NoBrush);
        painter->setFont(font1);
        painter->setPen(Qt::white);
        //painter->setPen(Qt::black);
        painter->drawText(-10,-10,20,20,Qt::AlignCenter,this->name);
    }
    else
    {
        painter->setPen(QPen(Qt::black,0));
        if (option->state & QStyle::State_Sunken) painter->setBrush(Qt::darkGray); else painter->setBrush(Qt::gray);
        painter->drawRect(-20, -30, 40, 60);

        QFont font1;
        font1.setBold(false);
        font1.setPointSize(4);
        painter->setBrush(Qt::NoBrush);
        painter->setFont(font1);
        painter->setPen(Qt::white);
        painter->drawText(-20,-30,40,60,Qt::AlignCenter,this->name);
    }
}
//! [10]

//! [11]
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        for (Edge *edge : qAsConst(edgeList))
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    }

    return QGraphicsItem::itemChange(change, value);
}
//! [11]

//! [12]
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
    if(this->nodeType==NODE_TYPE_LAYER)
    {
        QStringList list=this->name.split("\n");
        //qDebug()<<list;
        this->graph->showlayer(list[0].toInt()-1);
    }
}
//! [12]
