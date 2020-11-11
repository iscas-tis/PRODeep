#include "networkfile.h"
#include <QDir>
#include <QDebug>

NetworkFile::NetworkFile(QString filePath, QString fileName)
{
    this->filePath = filePath;
    this->fileName = fileName;

    this->parsingNetWork();
}



int NetworkFile::updateNetworkUI(Network * network)
{
    int length = this->layers->size();
    for(int i=0;i<length;i++){
        NetworkLayerData * layer =  this->layers->at(i);
        QStringList * rowNames = new QStringList();
        QStringList * columnNames = new QStringList();
        layer->getRowHeadNames(rowNames);
        layer->getColumnHeadNames(columnNames);

        if(layer->getLayerType() == LAYER_TYPE_INPUT)
        {
            network->addLayer(layer->getName(),layer->getLayerType(),8,rowNames,columnNames);
            network->setLayerName(i,LAYER_TYPE_INPUT);

        }
        else if (layer->getLayerType() == LAYER_TYPE_LINEAR) {
            columnNames->removeFirst();
            network->addLayer(layer->getName(),layer->getLayerType(),15, columnNames, rowNames);
            network->setLayerName(i,LAYER_TYPE_LINEAR);
            //Bias
            for(int j=0;j<rowNames->size();j++){
               network->setBias(i,j,layer->getData(j,0));
            }
            //Weight
            for(int j=0;j<rowNames->size();j++){
                for(int k=0;k<columnNames->size();k++){

                    network->setWeight(i,j,k,layer->getData(j,k+1));
                }
            }

        }
        else if (layer->getLayerType() == LAYER_TYPE_RELU)
        {
            columnNames->removeFirst();
            network->addLayer(layer->getName(),layer->getLayerType(),15,columnNames,rowNames);
            network->setLayerName(i,LAYER_TYPE_RELU);
            //Bias

            for(int j=0;j<rowNames->size();j++){
               network->setBias(i,j,layer->getData(j,0));
            }
            //Weight
            for(int j=0;j<rowNames->size();j++){
                for(int k=0;k<columnNames->size();k++){
                    network->setWeight(i,j,k,layer->getData(j,k+1));
                }
            }

        }
        else if(layer->getLayerType() == LAYER_TYPE_MAXPOOL)
        {
            network->addLayer(layer->getName(),layer->getLayerType(),11,columnNames,rowNames);
            network->setLayerName(i,LAYER_TYPE_MAXPOOL);

            //Weight
            for(int j=0;j<rowNames->size();j++){
                for(int k=0;k<columnNames->size();k++){
                    network->setWeight(i,j,k,layer->getData(j,k));
                }
            }

        }


    }
    return 0;

}
inline QStringList parsingLine(QByteArray line_byte){
    QString line_str(line_byte);
    return line_str.left(line_str.length()-1).split(" ");
}
int NetworkFile::parsingNetWork()
{
    QDir path(filePath);

    QFile file(path.absoluteFilePath(fileName));
    QFileInfo fileInfo(file);
    if(fileInfo.isDir()) return -1;

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "can not open" << fileInfo.absoluteFilePath();
        return -2;
    }

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
