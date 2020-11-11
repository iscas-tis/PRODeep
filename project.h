#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QVariant>

//#include "pluginconfigure.h"
#include "util.h"

#define ROBUSTNESS_INF 0
#define ROBUSTNESS_BRI 1
#define ROBUSTNESS_CUS 2

#define UNSERILIZE_FAILED -1
#define UNSERILIZE_SUCCESS 0

#define TOOL_BASIC "Basic"
#define TOOL_GEOMETRY "Geometry"
#define TOOL_DEEPSYMBOL "DeepSymbol"
#define TOOL_DEEPG "DeepG"
#define TOOL_DIFFAI "DiffAI"
#define TOOL_RELUPLEX "Reluplex"
#define TOOL_PLANET "PLANET"
#define TOOLS {TOOL_BASIC,TOOL_GEOMETRY,TOOL_RELUPLEX}
//#define TOOLS {TOOL_DEEPSYMBOL,TOOL_DIFFAI,TOOL_DEEPG}


#define ELEMENT_NUMBER 2


class Project: public QObject
{
    Q_OBJECT
private:
    QString path;
    QString networkFileName;
    QString inputFileName;
   // QString robustnessType = ROBUSTNESS_TYPE_LINFBALL;
    QString name;

    bool isSetNetworkFile = false;
    bool isSetInputFile = false;
   // bool isSetRobustnessType = false;

    QString serilize();
    int unserilize(const QString & project,QStringList tools);

    QStringList * tools = new QStringList();

    //PluginConfigure * pcf;

signals:
    void closeFile();
    void saveProject();
    void networkFileChanged(QString path, QString fileName);
    void inputFileChanged(QString path, QString fileName);
    void addTool(QString tool,QString index,Project * project);
    void removeTool(QString tool,QString index);

public:
    Project(const QString & path, const QString & name);
    Project( const QString & name);
    void setNetworkFileName(const QString & modelFileName);
    void setInputFileName(const QString & formuleFileName);
   // void setRobustnessType(QString robustnessType);

    QString getNetworkFileSuffix();
    QString getInputFileSuffix();

    //QString getRobustnessType();
    QString getAbsoluteNetworkFile();
    QString getAbsoluteInputFile();
    QString getNetworkFileName();
    QString getInputFileName();
    QString getPath();
    QString getName();
    QString generateParametersList();
    QString generateParametersListWithInput(const QString inputfilename);
    const QStringList * getTools();

    bool isValidNetworkFile();
    bool isValidInputFile();
    //bool isValidRobustnessType();

    void save();
    void open(QString projectFile,QStringList tools);
    QVariant get(int column) const;
    int count() const;

    void appendTool(QString tool,QString index);
    void deleteTool(QString tool,QString index);


};

#endif // PROJECT_H
