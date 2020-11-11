#include "networklayerdata.h"
#include <QDebug>
#include "assert.h"

NetworkLayerData::NetworkLayerData(QString name,QString layerType)
{
    //For Qt5.9.5 Ubuntu 18.04 default
    data = new QVector<QVector<QString>* >();

    this->name = name;
    this->layerType = layerType;
    rowHeadNames->clear();
    columnHeadNames->clear();
    data->clear();

}

QString NetworkLayerData::getName()
{
    return this->name;
}


void NetworkLayerData::insertRowHeadName(const QString name)
{
//    name.squeeze();
    assert(this->rowHeadNames);
    assert(this->data);
    this->rowHeadNames->append(name);
    QVector<QString>* rowTmp = new QVector<QString>();
    assert(rowTmp);
    int columnSize = this->columnHeadNames->size();
    for(int i=0;i<columnSize;i++){
        rowTmp->append("0");
    }
    data->append(rowTmp);
    rowTmp->squeeze();
    data->squeeze();
}
void NetworkLayerData::insertColumnHeadName(const QString name)
{
//    name.squeeze();
    this->columnHeadNames->append(name);
    int rowSize = this->rowHeadNames->size();
    for(int i=0;i<rowSize;i++){
        assert(data->at(i));
        data->at(i)->append("0");
//        data->at(i)->append("");
    }
}
void NetworkLayerData::setData(int row, int column,const QString data)
{
    //data.squeeze();
    assert(this->rowHeadNames);
    assert(this->columnHeadNames);
    if(row >= this->rowHeadNames->size() || column > this->columnHeadNames->size()){
        qDebug() << "Invalid setting data";
        return;
    }
    assert(this->data->at(row));
    this->data->at(row)->replace(column,data);
//    qDebug()<<this->data->at(row)->at(column).capacity()<<this->data->at(row)->at(column);
//    this->data->at(row)->at(column).squeeze();
    this->data->at(row)->squeeze();
    this->data->squeeze();
}

bool NetworkLayerData::containsRowHeadName(const QString name)
{
    assert(this->rowHeadNames);
    return this->rowHeadNames->contains(name);
}
bool NetworkLayerData::containsColumnHeadName(const QString name)
{
    assert(this->columnHeadNames);
    return this->columnHeadNames->contains(name);
}

int NetworkLayerData::getRowSize()
{
    assert(this->rowHeadNames);
    return this->rowHeadNames->size();
}
int NetworkLayerData::getColumnSize()
{
    assert(this->columnHeadNames);
    return this->columnHeadNames->size();
}

int NetworkLayerData::indexOfRowHead(QString name)
{
    assert(this->rowHeadNames);
    return this->rowHeadNames->indexOf(name);
}
int NetworkLayerData::indexOfColumnHead(QString name)
{
    assert(this->columnHeadNames);
    return this->columnHeadNames->indexOf(name);
}

QString NetworkLayerData::getData(int row, int column)
{
    return this->data->at(row)->at(column);
}

QString NetworkLayerData::getRowHeadName(int index){
    return this->rowHeadNames->at(index);
}
QString NetworkLayerData::getColumnHeadName(int index){
    return this->columnHeadNames->at(index);
}
int NetworkLayerData::getRowHeadNames(QStringList * result)
{
    if(result == nullptr) return -1;
    for(int i=0;i<this->getRowSize();i++){
        result->append(this->rowHeadNames->at(i));
    }
    return 0;
}
int NetworkLayerData::getColumnHeadNames(QStringList * result)
{
    if(result == nullptr) return -1;
    for(int i=0;i<this->getColumnSize();i++){
        result->append(this->columnHeadNames->at(i));
    }
    return 0;
}

QString NetworkLayerData::getLayerType()
{
    return this->layerType;
}
