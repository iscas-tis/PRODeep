#include "file.h"

#include<QDir>

File::File(QString path, QString name)
{
    QDir dir(path);
    this->info = QFileInfo(dir.absoluteFilePath(name));
}
File::File(QFileInfo file){
    this->info = file;
}

QString File::getPath()
{
    return this->info.path();
}
QString File::getName()
{
    return this->info.fileName();
}
QString File::getAbsFileName()
{
    return this->info.absoluteFilePath();
}

