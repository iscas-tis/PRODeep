#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QVariant>
#include <QFileInfo>

#define OPERATION_SUCCESS 0
#define FILE_CAN_NOT_OPEN -1

class File
{
private:
    QFileInfo info;


public:
    File(QString path, QString name);
    File(QFileInfo  file);

    QString getPath();
    QString getName();
    QString getAbsFileName();


};

#endif // FILES_H
