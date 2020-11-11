#ifndef UPDATENETWORKTHREAD_H
#define UPDATENETWORKTHREAD_H

#include <QThread>
#include "networklayerdata.h"
#include <QMutex>
#include <QSharedPointer>

class UpdateNetworkThread :public  QThread
{
    Q_OBJECT
public:
    explicit UpdateNetworkThread(QObject *parent = nullptr);
    void setParameters(QString filePath,QString fileName,bool isFirstLoad=true);
    void run_without_thread();
    QList<NetworkLayerData *> * getLayers();
protected:
    void run();
signals:
    void over();
private:
    QString fileName;
    QString filePath;
    mutable QMutex mutex;
    QList<NetworkLayerData *> *layers = new QList<NetworkLayerData *>();
    bool isFirstLoad;

    int parsingNetWork();

};

#endif // UPDATENETWORKTHREAD_H
