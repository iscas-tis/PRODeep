#ifndef NETWORK_H
#define NETWORK_H

#include <QWidget>
#include "networklayer.h"
#include "matrix.h"

namespace Ui {
class Network;
}

class Network : public QWidget
{
    Q_OBJECT

public:
    explicit Network(QWidget *parent = nullptr);
    ~Network();
    int addLayer(QString name, QString type, int flag=15, QStringList * inputName = nullptr, QStringList * outputName = nullptr);



    void setWeight(int layerIndex, int row, int column, QString value);
    void setBias(int layerIndex, int index, QString value);
    void setLayerName(int index, QString name);
    void setLayerDescription(int index, QString description);
    void setFileName(const QString name);
    void setFilePath(const QString path);

    void saveNetwork();

    void clear();

    bool isLoaded();

    int layerIndexOf(QString layerName);

    void setBoundOfNode(int index, QString nodeName, QString inf, QString sup);
    void clearBoundOfNode();

    void reload();
    int updateUI(QList<NetworkLayerData *> * layers);

    void clearCount();
    void showlayer(int layer);

    const QString getNeuronStatus();

public slots:
    //void loadNetworkfile(QString filePath, QString fileName);
    void on_IsValueChanged();
    void on_LayerButtonClicked(Matrix *matrix,const QString label);

signals:
    void showMatrix(Matrix *matrix,const QString label);
    void showStatus(const QString msg);
    void addNetworkGraph(const QString name, const QString layertype, int layernum, int whichlayer);

private slots:
    void on_showallButton_clicked();

private:
    Ui::Network *ui;

    QString filePath = "";
    QString fileName = "";
    QList<NetworkLayer *> * layers = new QList<NetworkLayer *>();
    bool isLoadedFlag = false;
    bool isValueChanged = false;
    int activatedNeurons=0;
    int deactivatedNeurons=0;
    int unknownNeurons=0;
};

#endif // NETWORK_H
