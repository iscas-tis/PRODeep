#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>



class Util
{
public:
    Util();
    static int createFile(const QString filePath, const QString fileName);
    static QJsonObject * parseJsonFile(const QString fileName);

};


#endif // UTIL_H
