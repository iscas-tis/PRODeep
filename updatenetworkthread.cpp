#include "updatenetworkthread.h"
#include <QDir>
#include <QDebug>

UpdateNetworkThread::UpdateNetworkThread(QObject *parent) : QThread(parent)
{
}

void UpdateNetworkThread::run()
{
    if(this->isFirstLoad)
    {        
        this->parsingNetWork();
        this->isFirstLoad = false;
    }
    emit over();
}
QList<NetworkLayerData *> * UpdateNetworkThread::getLayers()
{
    return this->layers;
}
void UpdateNetworkThread::setParameters(QString filePath,QString fileName, bool isFirstLoad)
{
    this->fileName = fileName;
    this->filePath = filePath;
    this->isFirstLoad = isFirstLoad;
}

inline QStringList parsingLine(QByteArray line_byte){
    QString line_str(line_byte);
    return line_str.left(line_str.length()-1).split(" ");
}
int UpdateNetworkThread::parsingNetWork()
{
    QDir path(filePath);

    QFile file(path.absoluteFilePath(fileName));
    QFileInfo fileInfo(file);
    if(fileInfo.isDir()) return -1;

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "can not open" << fileInfo.absoluteFilePath();
        return -2;
    }

    layers->clear();

    while (!file.atEnd()) {
        QStringList line_list = parsingLine(file.readLine());
        if(line_list.size() == 0) continue;
        if (line_list[0] == "#"){
             NetworkLayerData * new_layer = new NetworkLayerData(line_list[5],line_list[4]);
            layers->append(new_layer);
            int length = line_list[3].toInt();
            if(line_list[4] == LAYER_TYPE_INPUT)
            {
                for(int i=0;i<length;i++){
                    QStringList lines = parsingLine(file.readLine());
                    if(lines[0] == LAYER_TYPE_INPUT)
                    {
                        QString rowName = lines[1];
                        if(! new_layer->containsRowHeadName(rowName)){
                            new_layer->insertRowHeadName(rowName);
                        }
                    }
                    else
                    {
                        qDebug() << LAYER_TYPE_INPUT<<"Type not conststent";
                    }
                }
            }
            else if(line_list[4] == LAYER_TYPE_LINEAR)
            {
                for(int i=0;i<length;i++){
                    QStringList lines = parsingLine(file.readLine());
                    if(lines[0] == LAYER_TYPE_LINEAR)
                    {
                        QString rowName = lines[1];
                        if(! new_layer->containsRowHeadName(rowName)){
                            new_layer->insertRowHeadName(rowName);
                        }
                        int row = new_layer->getRowSize()-1;
                        int column = new_layer->indexOfColumnHead(LAYER_BIAS_NAME);

                        if(column == -1)
                        {
                            new_layer->insertColumnHeadName(LAYER_BIAS_NAME);
                            column = new_layer->getColumnSize()-1;
                        }
                        new_layer->setData(row,column,lines[2]);

                        int lineSize = lines.size();
                        for(int j=3;j<lineSize;j+=2)
                        {
                            QString value = lines[j];
                            QString name = lines[j+1];
                            column = new_layer->indexOfColumnHead(name);
                            if(column == -1)
                            {
                                new_layer->insertColumnHeadName(name);
                                column = new_layer->getColumnSize()-1;
                            }

                             new_layer->setData(row,column,value);
                        }
                    }
                    else
                    {
                        qDebug()<<LAYER_TYPE_LINEAR<<"Type not conststent";
                    }
                }
            }
            else if(line_list[4] == LAYER_TYPE_MAXPOOL)
            {
                for(int i=0;i<length;i++){
                    QStringList lines = parsingLine(file.readLine());
                    if(lines[0] == LAYER_TYPE_MAXPOOL)
                    {
                        QString rowName = lines[1];
                        if(! new_layer->containsRowHeadName(rowName)){
                            new_layer->insertRowHeadName(rowName);
                        }
                        int row = new_layer->getRowSize()-1;

                        int lineSize = lines.size();
                        for(int j=2;j<lineSize;j++)
                        {
                            QString value = "1";
                            QString name = lines[j];
                            int column = new_layer->indexOfColumnHead(name);
                            if(column == -1)
                            {
                                new_layer->insertColumnHeadName(name);
                                column = new_layer->getColumnSize()-1;
                            }

                             new_layer->setData(row,column,value);
                        }
                    }
                    else
                    {
                        qDebug()<<LAYER_TYPE_MAXPOOL<<"Type not conststent";
                    }
                }
            }
            else if(line_list[4] == LAYER_TYPE_RELU)
            {
                for(int i=0;i<length;i++){
                    QStringList lines = parsingLine(file.readLine());
                    if(lines[0] == LAYER_TYPE_RELU)
                    {
                        QString rowName = lines[1];
                        if(! new_layer->containsRowHeadName(rowName)){
                            new_layer->insertRowHeadName(rowName);
                        }
                        int row = new_layer->getRowSize()-1;
                        int column = new_layer->indexOfColumnHead(LAYER_BIAS_NAME);

                        if(column == -1)
                        {
                            new_layer->insertColumnHeadName(LAYER_BIAS_NAME);
                            column = new_layer->getColumnSize()-1;
                        }
                        new_layer->setData(row,column,lines[2]);

                        int lineSize = lines.size();
                        for(int j=3;j<lineSize;j+=2)
                        {
                            QString value = lines[j];
                            QString name = lines[j+1];
                            column = new_layer->indexOfColumnHead(name);
                            if(column == -1)
                            {
                                new_layer->insertColumnHeadName(name);
                                column = new_layer->getColumnSize()-1;
                            }

                             new_layer->setData(row,column,value);
                        }
                    }
                    else
                    {
                        qDebug()<<LAYER_TYPE_RELU<<"Type not conststent";
                    }
                }
            }
            else
            {
                qDebug()<< "unknown layer type:" << line_list[4];
            }


        }
    }

    file.close();
    return 0;
}
