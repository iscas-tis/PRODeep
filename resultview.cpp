#include "resultview.h"
#include "ui_resultview.h"
#include "util.h"
#include <QDebug>
#include <QJsonArray>

ResultView::ResultView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultView)
{
    ui->setupUi(this);
    this->resultGraph =  new CompareRelation();
    this->networkGraph = new NetworkGraph();
    this->showCompareRelation();
    this->showNetworkGraph();
    connect(ui->result,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTab(int)));
    connect(this->networkGraph,SIGNAL(SIGNAL_showlayer(int)),this,SLOT(showlayer(int)));
}

void ResultView::showlayer(int layer)
{
    emit SIGNAL_showlayer(layer);
}

void ResultView::showCompareRelation()
{
    if(this->ui->result->indexOf(this->resultGraph) == -1){
        this->ui->result->addTab(this->resultGraph,"Relation");
    }
}

void ResultView::showNetworkGraph()
{
    if(this->ui->result->indexOf(this->networkGraph) == -1){
        this->ui->result->addTab(this->networkGraph,"Layers");
    }
}

void ResultView::on_addNetworkGraph(const QString name, const QString layertype, int layernum, int layerwhich)
{
    this->showNetworkGraph();
    if(layerwhich==1)
    {
        this->networkGraph->clear();
        this->ui->result->setCurrentWidget(this->networkGraph);
    }
    QString layerinfo;
    QTextStream(&layerinfo)<<layerwhich<<"\nLayer type:\n"<<layertype<<"\nLayer name:\n"<<name<<"\nLayer num:\n"<<layernum;
    this->networkGraph->addNode(layerinfo);
    if(layerwhich>1)this->networkGraph->addEdge(this->lastLayer,layerinfo);
    this->lastLayer=layerinfo;
}

void ResultView::parsingJsonFile(QString jsonFile)
{
    this->resultGraph->clear();    
    this->showCompareRelation();
    this->ui->result->setCurrentWidget(this->resultGraph);

    QJsonObject * jsonObj = Util::parseJsonFile(jsonFile);
    if(jsonObj==nullptr) {
        qDebug() << "jsonObj is unvalid";
        return ;
    }
    emit this->updateNetworkNodeStatus(*jsonObj);
    if(jsonObj->contains("graph"))
    {        
        QJsonObject graph = jsonObj->value("graph").toObject();
        int nodes = 0;
        if(graph.contains("number_of_nodes")){
            nodes = graph.value("number_of_nodes").toString().toInt();
            for(int i=0;i<nodes;i++)
            {
                //this->resultGraph->addNode(QString::number(i));
            }
        }
        QStringList names;
        int du[nodes+2];
        memset(du,0,sizeof(du));
        int G[nodes+2][nodes+2];
        memset(G,0,sizeof(G));
        if(graph.contains(("names")))
        {            
            QJsonValue value = graph.value("names");
            if(value.isArray())
            {                
                QJsonArray array = value.toArray();
                int size =array.size();
                for(int i=0;i<size;i++){
                    names.append(array.at(i).toString());
                    this->resultGraph->addNode(array.at(i).toString());
                }
            }
        }

        if(graph.contains("edges"))
        {
            QJsonValue value = graph.value("edges");
            if(value.isArray())
            {
                QJsonArray array = value.toArray();
                int size =array.size();
                for(int i=0;i<size;i++){
                    int indexFrom = array.at(i).toObject().value("from").toString().toInt();
                    int indexTo = array.at(i).toObject().value("to").toString().toInt();
                    this->resultGraph->addEdge(names.at(indexFrom),names.at(indexTo));
                    //qDebug()<<indexFrom<<indexTo;
                    du[indexTo]++;
                    G[indexFrom][indexTo]=1;
                }
            }
        }
//        for(int i=0;i<nodes;i++)
//            for(int j=0;j<nodes;j++)
//                if(G[i][j]!=0)qDebug()<<i<<" "<<j<<" "<<G[i][j];
        //Caution:May cause some dead loop
        //qDebug()<<du[0]<<du[1]<<du[2]<<du[3];
        int posy=0;
        while(true)
        {
            int posx=0;
            for(int i=0;i<nodes;i++)
                if(du[i]==0)
                {
                    this->resultGraph->setpos(names.at(i),posx*40+posy*5,posy*40);
                    //qDebug()<<i<<" "<<posx<<" "<<posy;
                    du[i]=-posy-1;
                    posx++;
                }
            if(posx==0)break;
            for(int i=0;i<nodes;i++)
                if(du[i]==-posy-1)
                {
                    for(int j=0;j<nodes;j++)
                        if(G[i][j]==1)du[j]--;
                }
            posy++;
        }
    }    

}


void ResultView::removeSubTab(int index)
{
    this->ui->result->removeTab(index);
}
ResultView::~ResultView()
{
    delete ui;
}
