#include "network.h"
#include "ui_network.h"
#include "networkfile.h"
#include <QDebug>
#include <QDir>


Network::Network(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Network)
{
    ui->setupUi(this);
    this->ui->showallButton->setVisible(false);
   // ui->scrollAreaWidgetContents->setLayout(new QGridLayout());
    //this->setStyleSheet("background-color:gray;");

    //this->loadNetworkfile("/home/xuechao/xuechao/work/Lab/DeepAI/example/MNIST_FNN/","caffeprototxt_AI2_MNIST_FNN_1_testNetworkB.rlv");
    //this->saveNetwork();
}


void Network::saveNetwork()
{
    if(!this->isValueChanged) return;
    if(this->filePath == "" || this->fileName == "") return;
    QDir dir(this->filePath);
    QFile file(dir.absoluteFilePath(this->fileName));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << file.fileName() << "can not open";
        return;
    }
    for(int i=0;i<layers->size();i++)
    {
        layers->at(i)->saveLayer(file,i);
    }
    file.close();
    this->isValueChanged = false;
}

void Network::on_IsValueChanged()
{
    this->isValueChanged = true;
}

bool Network::isLoaded()
{
    return this->isLoadedFlag;
}

//void Network::loadNetworkfile(QString filePath, QString fileName)
//{
//    this->clear();

//    this->filePath = filePath;
//    this->fileName = fileName;
//    NetworkFile *nf = new NetworkFile(filePath, fileName);

//    //this->updateThread = new UpdateNetworkThread(filePath, fileName);

//    nf->updateNetworkUI(this);
//    //this->updateThread->start();

//    this->isLoadedFlag=true;
//    //this->layers->at(1)->setInputNodeStatus(1,NODE_STATUS_UNKNOWN);
//}

void Network::reload()
{
    if(this->isLoaded())
    {
        NetworkFile *nf = new NetworkFile(filePath, fileName);
        nf->updateNetworkUI(this);
    }

}
int Network::updateUI(QList<NetworkLayerData *> * layers)
{
    this->clear();
    QString networkstatus;
    int length =layers->size();
    int totalnodes=0;
    int hiddennodes=0;
    int outnodes=0;
    int inputnodes=0;
    QTextStream(&networkstatus)<<"Network: "<<this->fileName<<"  ";
    QTextStream(&networkstatus)<<"Layers: "<<length<<"  ";
    for(int i=0;i<length;i++){
        NetworkLayerData * layer =  layers->at(i);        
        QStringList * rowNames = new QStringList();
        QStringList * columnNames = new QStringList();
        layer->getRowHeadNames(rowNames);
        layer->getColumnHeadNames(columnNames);

        if(layer->getLayerType() == LAYER_TYPE_INPUT)
        {
            this->addLayer(layer->getName(),layer->getLayerType(),8,rowNames,columnNames);
            this->setLayerName(i,LAYER_TYPE_INPUT);
            inputnodes+=rowNames->size();

        }
        else if (layer->getLayerType() == LAYER_TYPE_LINEAR) {
            columnNames->removeFirst();
            this->addLayer(layer->getName(),layer->getLayerType(),15, columnNames, rowNames);
            this->setLayerName(i,LAYER_TYPE_LINEAR);
            //Bias
            for(int j=0;j<rowNames->size();j++){
               this->setBias(i,j,layer->getData(j,0));
            }
            //Weight
            for(int j=0;j<rowNames->size();j++){
                for(int k=0;k<columnNames->size();k++){

                    this->setWeight(i,j,k,layer->getData(j,k+1));
                }
            }

        }
        else if (layer->getLayerType() == LAYER_TYPE_RELU)
        {
            columnNames->removeFirst();
            this->addLayer(layer->getName(),layer->getLayerType(),15,columnNames,rowNames);
            this->setLayerName(i,LAYER_TYPE_RELU);
            //Bias

            for(int j=0;j<rowNames->size();j++){
               this->setBias(i,j,layer->getData(j,0));
            }
            //Weight
            for(int j=0;j<rowNames->size();j++){
                for(int k=0;k<columnNames->size();k++){
                    this->setWeight(i,j,k,layer->getData(j,k+1));
                }
            }

        }
        else if(layer->getLayerType() == LAYER_TYPE_MAXPOOL)
        {
            this->addLayer(layer->getName(),layer->getLayerType(),11,columnNames,rowNames);
            this->setLayerName(i,LAYER_TYPE_MAXPOOL);

            //Weight
            for(int j=0;j<rowNames->size();j++){
                for(int k=0;k<columnNames->size();k++){
                    this->setWeight(i,j,k,layer->getData(j,k));
                }
            }

        }

        outnodes=rowNames->size();
        totalnodes+=rowNames->size();
    }

    hiddennodes=totalnodes-outnodes-inputnodes;
    QTextStream(&networkstatus)<<"Input Nodes: "<<inputnodes<<"  ";
    QTextStream(&networkstatus)<<"Hidden Nodes: "<<hiddennodes<<"  ";
    QTextStream(&networkstatus)<<"Output Nodes: "<<outnodes<<"  ";
    //QTextStream(&networkstatus)<<"Nodes: "<<totalnodes<<"  ";
    emit showStatus(networkstatus);
    this->isLoadedFlag=true;
    return 0;

}
void Network::setBoundOfNode(int index, QString nodeName, QString inf, QString sup)
{
    double upper = sup.toDouble();
    double lower = inf.toDouble();
    int status = -1;
    if(lower >=0) status = NODE_STATUS_ACTIVE;
    else if(upper <=0) status = NODE_STATUS_UNACTIVE;
    else status = NODE_STATUS_UNKNOWN;

    NetworkLayer * layer = this->layers->at(index);
    layer->setOutput(nodeName,inf,sup);
    layer->setOutputNodeStatus(nodeName,status);
    if(index!=0)
    {
        if(status==NODE_STATUS_ACTIVE)this->activatedNeurons++;
        if(status==NODE_STATUS_UNACTIVE)this->deactivatedNeurons++;
        if(status==NODE_STATUS_UNKNOWN)this->unknownNeurons++;
    }
    if(index<this->layers->size()-1)
    {
        layer =  this->layers->at(index+1);
        layer->setInput(nodeName,inf,sup);
        layer->setInputNodeStatus(nodeName,status);
    }
}
void Network::clearBoundOfNode()
{
    for(int i=0;i<this->layers->size();i++)
    {
        NetworkLayer * layer = this->layers->at(i);
        for(int j=0;j<layer->getNumOfOutputNames();j++)
        {
            layer->setOutput(j,"","");
            layer->setOutputNodeStatus(j,NODE_STATUS_CLEAR);
        }
        if(i<this->layers->size()-1)
        {
            layer =  this->layers->at(i+1);
            for(int j=0;j<layer->getNumOfInputNames();j++)
            {
                layer->setInput(j,"","");
                layer->setInputNodeStatus(j,NODE_STATUS_CLEAR);
            }
        }
    }
}

int Network::layerIndexOf(QString layerName)
{
    for(int i=0;i<this->layers->size();i++){
        if(this->layers->at(i)->getName().compare(layerName) == 0) return i;
    }
    return -1;
}

void Network::setFileName(const QString name)
{
    this->fileName=name;
}

void Network::setFilePath(const QString path)
{
    this->filePath=path;
}

void Network::setLayerName(int index, QString name)
{
    this->layers->at(index)->setName(name);
}
void Network::setLayerDescription(int index, QString description)
{
    this->layers->at(index)->setDescription(description);
}

void Network::setWeight(int layerIndex, int row, int column, QString value)
{

    this->layers->at(layerIndex)->setWeight(row,column,value);
}

void Network::setBias(int layerIndex, int index, QString value)
{
    this->layers->at(layerIndex)->setBias(index,value);
}

int Network::addLayer(QString name, QString type, int flag, QStringList * inputName, QStringList * outputName)
{
    NetworkLayer * layer = new NetworkLayer(name,type,flag, inputName,outputName);
    //layer->setStyleSheet("background-color:red;");

    this->ui->network->addWidget(layer);
    this->layers->append(layer);
    emit addNetworkGraph(layer->getName(),layer->getLayerType(), layer->getNodeNum(), this->layers->size());
    connect(layer,SIGNAL(valueChanged()),this,SLOT(on_IsValueChanged()));
    connect(layer,SIGNAL(buttonClicked(Matrix *,const QString)),this,SLOT(on_LayerButtonClicked(Matrix *,const QString)));    
    return 0;
}

void Network::on_LayerButtonClicked(Matrix *matrix,const QString label)
{
    emit showMatrix(matrix,label);
}

void Network::clear()
{
    for(int i=0;i<this->layers->size();i++)
    {
        NetworkLayer * layer  = this->layers->at(i);
        this->ui->network->removeWidget(layer);
        delete layer;
    }
    this->layers->clear();
    isLoadedFlag = false;
}

void Network::clearCount()
{
    this->activatedNeurons=0;
    this->deactivatedNeurons=0;
    this->unknownNeurons=0;
}

const QString Network::getNeuronStatus()
{
    QString result;
    QTextStream(&result)<<"Activated Neurons: "<<this->activatedNeurons<<"\n"<<"Deactivated Neurons: "<<this->deactivatedNeurons<<"\n"<<"Unknown Neurons: "<<this->unknownNeurons<<"\n";
    double totalneurons=this->activatedNeurons+this->deactivatedNeurons+this->unknownNeurons;
    QTextStream(&result)<<"Activated Neuron Percentage: "<<QString::number(this->activatedNeurons/totalneurons*100,'g',5)<<"%\n";
    QTextStream(&result)<<"Deactivated Neuron Percentage: "<<QString::number(this->deactivatedNeurons/totalneurons*100,'g',5)<<"%\n";
    QTextStream(&result)<<"Unknown Neuron Percentage: "<<QString::number(this->unknownNeurons/totalneurons*100,'g',5)<<"%\n";
    return result;
}
void Network::showlayer(int layer)
{
    for(int i=0;i<this->layers->size();i++)
    {
        this->layers->at(i)->setVisible(false);
    }
    this->layers->at(layer)->setVisible(true);
    this->ui->showallButton->setVisible(true);
}
Network::~Network()
{
    delete ui;
}

void Network::on_showallButton_clicked()
{
    for(int i=0;i<this->layers->size();i++)
    {
        this->layers->at(i)->setVisible(true);
    }
    this->ui->showallButton->setVisible(false);
}
