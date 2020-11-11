#include "comparerelation.h"
#include "ui_comparerelation.h"

#include <QDebug>

CompareRelation::CompareRelation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompareRelation)
{
    ui->setupUi(this);
    graph = new GraphWidget();
    this->ui->Image->addWidget(graph);
}

void CompareRelation::addNode(QString name)
{
    this->graph->addNode(name);
}
void CompareRelation::addEdge(QString name1,QString name2)
{
    this->graph->addEdge(name1,name2);
}

void CompareRelation::setpos(const QString name, qreal x, qreal y)
{
    this->graph->setpos(name,x,y);
}

void CompareRelation::clear()
{
    this->graph->clear();
    //this->graph->setSceneRect(-0.5*this->width(),-0.5*this->height(),this->width(),this->height());
}
CompareRelation::~CompareRelation()
{
    delete ui;
}
