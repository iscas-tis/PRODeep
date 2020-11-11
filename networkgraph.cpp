#include "networkgraph.h"
#include "ui_networkgraph.h"

NetworkGraph::NetworkGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkGraph)
{
    ui->setupUi(this);
    this->graph=new GraphWidget();
    ui->imageLayout->addWidget(this->graph);
    connect(this->graph,SIGNAL(SIGNAL_showlayer(int)),this,SLOT(showlayer(int)));
}

void NetworkGraph::showlayer(int layer)
{
    emit SIGNAL_showlayer(layer);
}

NetworkGraph::~NetworkGraph()
{
    delete ui;
}

void NetworkGraph::addNode(QString name)
{
    this->graph->addNode(name,NODE_TYPE_LAYER);
}
void NetworkGraph::addEdge(QString name1,QString name2)
{
    this->graph->addEdge(name1,name2,EDGE_TYPE_LAYER);
}

void NetworkGraph::clear()
{
    this->graph->clear();
}
