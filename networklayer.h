#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

#include <QWidget>

#include "matrix.h"
#include "networklayerdata.h"

#define NODE_STATUS_ACTIVE 0
#define NODE_STATUS_UNKNOWN 1
#define NODE_STATUS_UNACTIVE 2
#define NODE_STATUS_CLEAR 3

namespace Ui {
class NetworkLayer;
}


class NetworkLayer : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkLayer(QString name,QString layerType,int flag =15, QStringList * inputName = nullptr, QStringList * outputName = nullptr, QWidget *parent = nullptr);
    ~NetworkLayer();

    void setWeight(int row, int column, QString value);
    void setInput(int index, QString inf,QString sup);
    void setInput(QString name, QString inf,QString sup);
    void setBias(int index, QString value);
    void setOutput(int index, QString inf, QString sup);
    void setOutput(QString name, QString inf, QString sup);
    void setName(QString name);
    void setDescription(QString description);

    void setInputNodeStatus(int index, int status);
    void setOutputNodeStatus(int index, int status);
    void setInputNodeStatus(QString name, int status);
    void setOutputNodeStatus(QString name, int status);



    void saveLayer(QFile & file, int index);
    QString getLayerType();
    QString getName();
    int getNodeNum();

    const QStringList* getInputNames();
    const QStringList * getOutputNames();
    int getNumOfInputNames();
    int getNumOfOutputNames();

public slots:
    void onInputViewChanged();
    void onWeightViewChanged();
    void onBiasViewChanged();
    void onOutputViewChanged();
    void onWeightButtonClicked();
    void onBiasButtonClicked();

signals:
    void valueChanged();
    void buttonClicked(Matrix *matrix,const QString label);
private:
    Ui::NetworkLayer *ui;
    Matrix * input=nullptr;
    Matrix * weight=nullptr;
    Matrix * bias=nullptr;
    Matrix * output=nullptr;
    int  flag;
    int  nodenum;
    QString layerType;
    QString name;

    QStringList * inputName = new QStringList();
    QStringList * outputName = new QStringList();

};

#endif // NETWORKLAYER_H
