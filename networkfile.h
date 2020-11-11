#ifndef NETWORKFILE_H
#define NETWORKFILE_H

#include <QString>
#include "networklayerdata.h"
#include "network.h"
class NetworkFile
{
public:
    NetworkFile(QString filePath, QString fileName);

    int parsingNetWork();

    int updateNetworkUI(Network * network);


private:
    QString fileName;
    QString filePath;
    QList<NetworkLayerData *> *layers = new QList<NetworkLayerData *>();

};

#endif // NETWORKFILE_H
