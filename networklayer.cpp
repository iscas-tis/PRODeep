#include "networklayer.h"
#include "ui_networklayer.h"
#include <QDebug>

NetworkLayer::NetworkLayer(QString name, QString layerType,int flag,QStringList * inputName,QStringList * outputName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkLayer)
{
    ui->setupUi(this);

    this->ui->weightButton->hide();
    this->ui->biasButton->hide();

    this->layerType = layerType;
    this->name = name;
    this->nodenum=outputName->size();
    this->setName("Layer_"+layerType);
    this->setDescription("Layer_"+this->name);

    this->flag = flag;

    if((flag & 1) == 1){
        this->input = new Matrix("Input", inputName, nullptr);
        this->ui->V_1_2->addWidget(this->input,2);        
        for(int i=0;i<inputName->size();i++)
        {
            this->inputName->append(inputName->at(i));
        }

       // this->input->setEditable(false);
        connect(this->input,SIGNAL(onValueChanged()),this,SLOT(onInputViewChanged()));
    }
    if(((flag >>1) & 1) == 1)
    {        
        this->weight = new Matrix("Weight", outputName,inputName);
        this->weight->hide();
        connect(this->weight,SIGNAL(onValueChanged()),this,SLOT(onWeightViewChanged()));
        this->ui->V_1_2->addWidget(this->weight,3);
        this->ui->weightButton->show();
    }
    if(((flag >>2) & 1) == 1)
    {
        this->bias = new Matrix("Bias", outputName, nullptr,false);
        this->bias->hide();
        connect(this->bias,SIGNAL(onValueChanged()),this,SLOT(onBiasViewChanged()));
        this->ui->V_1_2->addWidget(this->bias,2);
        this->ui->biasButton->show();
    }
    if(((flag >>3) & 1) == 1)
    {
        if(flag == 8){
            this->output = new Matrix("data", nullptr, inputName);
            this->ui->V_1_2->addWidget(this->output,1);

            for(int i=0;i<inputName->size();i++)
            {
                this->outputName->append(inputName->at(i));
            }
            this->nodenum=inputName->size();
        }
        else{
            this->output = new Matrix("Output",outputName,nullptr);
            this->ui->V_1_2->addWidget(this->output,2);

            for(int i=0;i<outputName->size();i++)
            {
                this->outputName->append(outputName->at(i));
            }
            this->nodenum=outputName->size();
        }
       // this->output->setEditable(false);
        connect(this->output,SIGNAL(onValueChanged()),this,SLOT(onOutputViewChanged()));

    }

   connect(this->ui->weightButton,SIGNAL(clicked()),this,SLOT(onWeightButtonClicked()));
   connect(this->ui->biasButton,SIGNAL(clicked()),this,SLOT(onBiasButtonClicked()));

}

void NetworkLayer::onWeightButtonClicked()
{
    emit buttonClicked(this->weight,"Weight of Layer "+this->name);
}

void NetworkLayer::onBiasButtonClicked()
{
    emit buttonClicked(this->bias,"Bias of Layer "+this->name);
}

int NetworkLayer::getNumOfInputNames()
{
    return this->inputName->size();
}
int NetworkLayer::getNumOfOutputNames()
{
    return this->outputName->size();
}

const QStringList* NetworkLayer::getInputNames()
{
    return this->inputName;
}
const QStringList * NetworkLayer::getOutputNames()
{
    return this->outputName;
}

void NetworkLayer::onInputViewChanged()
{
    qDebug() << "input";
}
void NetworkLayer::onWeightViewChanged()
{
    qDebug() << "weight";
    emit valueChanged();
}
void NetworkLayer::onBiasViewChanged()
{
    qDebug() << "bias";
    emit valueChanged();
}
void NetworkLayer::onOutputViewChanged()
{
    qDebug() << "uotput";
}


void NetworkLayer::saveLayer(QFile & file, int index)
{
    if(!file.isOpen())
    {
        qDebug() << file.fileName() << "is not open";
        return;
    }
    if(!file.isWritable())
    {
        qDebug() << file.fileName() << "is not writable";
        return;
    }


    if(this->layerType == LAYER_TYPE_INPUT)
    {

        QStringList * inputName = this->output->getColumnHeadNames();
        int lines = inputName->size();
        //write head of layer
        QString head = "# Layer "+QString::number(index)+" "+QString::number(lines)+" "+this->layerType+" "+this->name + "\n";
        file.write(head.toUtf8());
        for(int i=0;i<lines;i++)
        {
            QString lineStr = this->layerType;
            lineStr += " "+inputName->at(i)+"\n";
            file.write(lineStr.toUtf8());

        }

    }
    else if(this->layerType == LAYER_TYPE_RELU)
    {
        int lines = this->weight->getRowNumber();
        //write head of layer
        QString head = "# Layer "+QString::number(index)+" "+QString::number(lines)+" "+this->layerType+" "+this->name + "\n";
        file.write(head.toUtf8());
        QStringList * inputName = this->weight->getColumnHeadNames();
        QStringList * outputName = this->weight->getRowHeadNames();
        for(int i=0;i<lines;i++)
        {
            QString lineStr = this->layerType + " "+ outputName->at(i) + " " + this->bias->getData(i,0);
            int length = inputName->size();
            for(int j=0;j<length;j++)
            {
                QString data = this->weight->getData(i,j);
                if(data.toDouble() != 0.0)
                {
                    lineStr += " "+data +" "+ inputName->at(j);
                }
            }
            lineStr+="\n";
            file.write(lineStr.toUtf8());
        }
    }
    else if (this->layerType == LAYER_TYPE_LINEAR) {

        int lines = this->weight->getRowNumber();
        //write head of layer
        QString head = "# Layer "+QString::number(index)+" "+QString::number(lines)+" "+this->layerType+" "+this->name + "\n";
        file.write(head.toUtf8());
        QStringList * inputName = this->weight->getColumnHeadNames();
        QStringList * outputName = this->weight->getRowHeadNames();
        for(int i=0;i<lines;i++)
        {
            QString lineStr = this->layerType + " "+ outputName->at(i) + " " + this->bias->getData(i,0);
            int length = inputName->size();
            for(int j=0;j<length;j++)
            {
                QString data = this->weight->getData(i,j);
                if(data.toDouble() != 0.0)
                {
                    lineStr += " "+data +" "+ inputName->at(j);
                }
            }
            lineStr+="\n";
            file.write(lineStr.toUtf8());
        }
    }
    else if (this->layerType == LAYER_TYPE_MAXPOOL) {

        int lines = this->weight->getRowNumber();
        //write head of layer
        QString head = "# Layer "+QString::number(index)+" "+QString::number(lines)+" "+this->layerType+" "+this->name + "\n";
        file.write(head.toUtf8());
        QStringList * inputName = this->weight->getColumnHeadNames();
        QStringList * outputName = this->weight->getRowHeadNames();
        for(int i=0;i<lines;i++)
        {
            QString lineStr = this->layerType + " "+ outputName->at(i);
            int length = inputName->size();
            for(int j=0;j<length;j++)
            {
                QString data = this->weight->getData(i,j);
                if(data == "1")
                {
                    lineStr += " "+inputName->at(j);
                }
            }
            lineStr+="\n";
            file.write(lineStr.toUtf8());
        }
    }
    else
    {
        qDebug()<<this->layerType<<"is not a valid layer type";
    }

}


QString NetworkLayer::getLayerType()
{
    return this->layerType;
}

QString NetworkLayer::getName()
{
    return this->name;
}

int NetworkLayer::getNodeNum()
{
    return this->nodenum;
}

void NetworkLayer::setInputNodeStatus(QString name, int status)
{
    if(! this->input) return;
    int index =-1;
    if(this->input->isSetColumnHeadName())
    {
        index = this->input->indexOfColumn(name);
    }
    else{
       index = this->input->indexOfRow(name);
    }
    this->setInputNodeStatus(index,status);
}
void NetworkLayer::setOutputNodeStatus(QString name, int status)
{
    if(! this->output) return;
    int index =-1;
    if(this->output->isSetColumnHeadName())
    {
        index = this->output->indexOfColumn(name);
    }
    else{
       index = this->output->indexOfRow(name);
    }
    this->setOutputNodeStatus(index,status);
}


void NetworkLayer::setInputNodeStatus(int index, int status)
{
    if(!this->input ) return;
    QColor  color;
    if(status == NODE_STATUS_ACTIVE){
        color = Qt::green;
    }
    else if(status == NODE_STATUS_UNKNOWN){
        color = Qt::yellow;
    }
    else if (status == NODE_STATUS_UNACTIVE) {
        color = Qt::red;
    }
    else if(status ==NODE_STATUS_CLEAR)
    {
        color = Qt::white;
    }
    else
    {
        qDebug() << "Can not found this kind of status code:"<< status;
            return;
    }
    if(this->input->isSetColumnHeadName())
    {
        this->input->setItemColor(0,index,color);
        this->input->setItemColor(1,index,color);
    }
    else
    {
        this->input->setItemColor(index,0,color);
        this->input->setItemColor(index,1,color);
    }
}
void NetworkLayer::setOutputNodeStatus(int index, int status)
{
    if(!this->output ) return;
    if(flag == 1) return;
    QColor  color;
    if(status == NODE_STATUS_ACTIVE){
        color = Qt::green;
    }
    else if(status == NODE_STATUS_UNKNOWN){
        color = Qt::yellow;
    }
    else if (status == NODE_STATUS_UNACTIVE) {
        color = Qt::red;
    }
    else if(status ==NODE_STATUS_CLEAR)
    {
        color = Qt::white;
    }
    else
    {
        qDebug() << "Can not found this kind of status code:"<< status;
            return;
    }
    if(this->output->isSetColumnHeadName())
    {
        this->output->setItemColor(0,index,color);
        this->output->setItemColor(1,index,color);
    }
    else
    {
        this->output->setItemColor(index,0,color);
        this->output->setItemColor(index,1,color);
    }
}

void NetworkLayer::setWeight(int row, int column, QString value)
{
    this->weight->insertItem(row,column,value);
}
void NetworkLayer::setInput(QString name, QString inf,QString sup)
{
    if(! this->input) return;
    int index =-1;
    if(this->input->isSetColumnHeadName())
    {
        index = this->input->indexOfColumn(name);
    }
    else{
       index = this->input->indexOfRow(name);
    }
    this->setInput(index,inf,sup);
}
void NetworkLayer::setInput(int index, QString inf,QString sup)
{
    if(! this->input) return;
    if(this->input->isSetRowHeadName())
    {
        this->input->insertItem(index,0,inf);
        this->input->insertItem(index,1,sup);

    }
    else
    {
        this->input->insertItem(0,index,inf);
        this->input->insertItem(1,index,sup);
    }
}
void NetworkLayer::setBias(int index, QString value)
{
    this->bias->insertItem(index,0,value);
}
void NetworkLayer::setOutput(QString name, QString inf,QString sup)
{
    if(! this->output) return;
    int index =-1;
    if(this->output->isSetColumnHeadName())
    {
        index = this->output->indexOfColumn(name);
    }
    else{
       index = this->output->indexOfRow(name);
    }
    this->setOutput(index,inf,sup);
}
void NetworkLayer::setOutput(int index, QString inf,QString sup)
{  if(! this->output) return;
    if(this->output->isSetRowHeadName())
    {
        this->output->insertItem(index,0,inf);
        this->output->insertItem(index,1,sup);
    }
    else
    {
        this->output->insertItem(0,index,inf);
        this->output->insertItem(1,index,sup);
    }
}

void NetworkLayer::setName(QString name)
{
    this->ui->layerName->setText(name);
}
void NetworkLayer::setDescription(QString description)
{
    this->ui->layerDescription->setText(description);
}

NetworkLayer::~NetworkLayer()
{
    delete ui;
}
