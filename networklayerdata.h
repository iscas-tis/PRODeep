#ifndef NETWORKLAYERDATA_H
#define NETWORKLAYERDATA_H

#define LAYER_TYPE_INPUT "Input"
#define LAYER_TYPE_LINEAR "Linear"
#define LAYER_TYPE_RELU "ReLU"
#define LAYER_TYPE_MAXPOOL "MaxPool"

#define LAYER_BIAS_NAME "Bias"

#include <QStringList>
#include <QFile>


class NetworkLayerData
{
public:
    NetworkLayerData(QString name,QString layerType);
    ~NetworkLayerData();
    void insertRowHeadName(QString name);
    void insertColumnHeadName(QString name);
    void setData(int row, int column, QString data);
    QString getData(int row, int column);

    bool containsRowHeadName(QString name);
    bool containsColumnHeadName(QString name);

    int getRowSize();
    int getColumnSize();

    int indexOfRowHead(QString name);
    int indexOfColumnHead(QString name);

    QString getRowHeadName(int index);
    QString getColumnHeadName(int index);

    int getRowHeadNames(QStringList * result);
    int getColumnHeadNames(QStringList * result);

    QString getLayerType();
    QString getName();




private:
    QString name;
    QString layerType;
    QStringList * rowHeadNames = new QStringList();
    QStringList * columnHeadNames = new QStringList();
    //For Qt 5.13.1
    //QVector<QVector<QString>* >  * data = new QVector<QVector<QString>* >();
    //For Qt 5.9.5 Ubuntu 18.04 defualt
    QVector<QVector<QString>* >  * data;
};

#endif // NETWORKLAYERDATA_H
