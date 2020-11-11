#include "graphwidget.h"
//#include "edge.h"
//#include "node.h"

#include <math.h>

#include <QKeyEvent>
//QRandom Generator is only available Qt 5.10+
//#include <QRandomGenerator>
#include <QDebug>

//! [0]
GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
   // scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //scene->setSceneRect(this->rect());
    //scene->setSceneRect(0, 0, 10, 10);
    //qDebug()<<this->width()<<this->height()<<this->rect();
    setScene(scene);
   // setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(2), qreal(2));
    //this->scene()->setSceneRect(0,0,600,600);
    //qDebug()<<this->scene()->sceneRect();
    //setMinimumSize(400, 400);
//! [0]

//! [1]
//    Node *node1 = new Node("1\n2\n3\n",this,NODE_TYPE_LAYER);
//    Node *node2 = new Node("2",this,NODE_TYPE_LAYER);
//    Node *node3 = new Node("3",this,NODE_TYPE_LAYER);
//    Node *node4 = new Node("4",this,NODE_TYPE_LAYER);
//    centerNode = new Node("center",this,NODE_TYPE_LAYER);
//    Node *node6 = new Node("6",this,NODE_TYPE_LAYER);
//    Node *node7 = new Node("7",this,NODE_TYPE_LAYER);
//    Node *node8 = new Node("8",this,NODE_TYPE_LAYER);
//    Node *node9 = new Node("9",this,NODE_TYPE_LAYER);
//    this->scene()->addItem(node1);
//    scene->addItem(node2);
//    scene->addItem(node3);
//    scene->addItem(node4);
//    scene->addItem(centerNode);
//    scene->addItem(node6);
//    scene->addItem(node7);
//    scene->addItem(node8);
//    scene->addItem(node9);
//    scene->addItem(new Edge(node1, node2,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node2, node3,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node2, centerNode,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node3, node6,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node4, node1,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node4, centerNode,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(centerNode, node6,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(centerNode, node8,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node6, node9,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node7, node4,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node8, node7,EDGE_TYPE_LAYER));
//    scene->addItem(new Edge(node9, node8,EDGE_TYPE_LAYER));

//    node1->setPos(-70, -70);
//    node2->setPos(0, -70);
//    node3->setPos(70, -70);
//    node4->setPos(-70, 0);
//    centerNode->setPos(0, 0);
//    node6->setPos(70, 0);
//    node7->setPos(-70, 70);
//    node8->setPos(0, 70);
//    node9->setPos(70, 70);
}
//! [1]


void GraphWidget::addNode(QString name, int NodeType)
{
    Node * item = new Node(name,this,NodeType);

    this->scene()->addItem(item);
    if(NodeType!=NODE_TYPE_LAYER)this->shuffle();
    if(NodeType==NODE_TYPE_LAYER)
    {
        item->setPos(this->nodesMap.size()*70,0);
    }
    if(this->nodesMap.size() == 0)
    {
        this->centerNode = item;
    }
    this->nodesMap.insert(name,item);
}
void GraphWidget::addEdge(QString node1,QString node2,int edgeType)
{
    Node * n1 = this->nodesMap.value(node1);
    Node * n2 = this->nodesMap.value(node2);

    this->scene()->addItem(new Edge(n1, n2, edgeType));
}
void GraphWidget::setpos(const QString name, qreal x, qreal y)
{
    Node *n = this->nodesMap.value(name);
    n->setPos(x,y);
}
void GraphWidget::setCenterNode(QString name)
{
    this->centerNode  = this->nodesMap.value(name);
}
void GraphWidget::clear()
{
    scene()->clear();
    this->nodesMap.clear();    
}
//! [2]
void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}
//! [2]

//! [3]
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
//! [3]

//! [4]
void GraphWidget::timerEvent(QTimerEvent *event)
{
    qDebug()<<"Timer Out!";
    //no pushing or pulling effects,so just return
    killTimer(timerId);
    timerId = 0;
    return;
    Q_UNUSED(event);

    QList<Node *> nodes;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    for (Node *node : qAsConst(nodes))
    {
        //if(node != this->centerNode)
            node->calculateForces();
    }

    bool itemsMoved = false;
    for (Node *node : qAsConst(nodes)) {
        if (node->advancePosition())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}
//! [4]

#if QT_CONFIG(wheelevent)
//! [5]
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}
//! [5]
#endif

//! [6]
/*void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    //Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = rect;//this->rect();//this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);


}*/
//! [6]

//! [7]
void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
//! [7]

void GraphWidget::shuffle()
{        
    //Since we have already set the position,no need to shuffle now
    return;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (qgraphicsitem_cast<Node *>(item))
        {
            //item->setPos(-150 + QRandomGenerator::global()->bounded(300), -150 + QRandomGenerator::global()->bounded(300));
            //item->setPos(QRandomGenerator::global()->bounded(30)*10.0,QRandomGenerator::global()->bounded(30)*10.0);
            //qDebug()<<QRandomGenerator::global()->bounded(30)*10.0<<QRandomGenerator::global()->bounded(30)*10.0;
        }
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}

//GraphWidget::~GraphWidget()
//{
//    this->clear();
//}

void GraphWidget::showlayer(int layer)
{
    emit SIGNAL_showlayer(layer);
}
