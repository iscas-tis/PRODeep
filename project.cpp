#include "project.h"
#include <QDir>
#include <QDebug>
#include "util.h"


Project::Project(const QString & path, const QString & name)
{
    this->path = path;
    this->name = name;

  //  QDir dir(path);
  //  Util::createFile(dir.absolutePath(),"addedplugin.txt");
  //  Util::createFile(dir.absolutePath(),"unaddedplugin.txt");

  //  this->pcf = new PluginConfigure(dir.absolutePath(),"addedplugin.txt","unaddedplugin.txt");

}
Project::Project(const QString & name)
{
    this->path = "";
    this->name = name;
}

void Project::appendTool(QString tool,QString index)
{
    if(this->tools->contains(tool)){
        return;
    }
    this->tools->append(tool);
    emit this->addTool(tool,index,this);
}
void Project::deleteTool(QString tool,QString index)
{
    if(!this->tools->contains(tool)){
        return;
    }
    this->tools->removeOne(tool);
    emit this->removeTool(tool,index);
}

QVariant Project::get(int column) const
{
    if (column < 0 || column >= ELEMENT_NUMBER)
        return QVariant();
    if(column == 0){
        return this->name;
    }
    else if(column == 1){
        return this->path;
    }
    else{
        return QVariant();
    }
}
int Project::count() const
{
    return ELEMENT_NUMBER;
}

void Project::save()
{
    QString project = this->serilize();
    qDebug() << project;
    QDir dir(this->path);
    QFile file(dir.absoluteFilePath(this->name+".pro"));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(project.toUtf8());
    file.close();
    emit saveProject();
}

void Project::open(QString projectFile,QStringList tools)
{

    QFile file(projectFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    this->unserilize(QString(t),tools);
    file.close();
}

QString Project::serilize()
{
    QString result = this->networkFileName+";"+this->inputFileName+";";
    result+="TOOL:";
    for(int i=0;i<this->tools->size();i++)
    {
     result+=this->tools->at(i)+"$";
    }
    result += ";";
    return result;

}

QString Project::getNetworkFileName()
{
    return this->networkFileName;
}
QString Project::getInputFileName(){
    return this->inputFileName;
}



int Project::unserilize(const QString & project,QStringList tools)
{
    QStringList list = project.split(";");
    if(list.size()<3) return UNSERILIZE_FAILED;
    if(list[0].compare("") != 0)
        this->setNetworkFileName(list[0]);
    if(list[1].compare("") != 0)
        this->setInputFileName(list[1]);
    if(list[2].compare("")!=0){
        QStringList toolsPart = list[2].split(":");
        if(toolsPart.size()<2) return UNSERILIZE_FAILED;
        if(toolsPart[0].compare("TOOL")!=0) return UNSERILIZE_FAILED;
        QStringList toolsName = toolsPart[1].split("$");
        for(int i=0;i<toolsName.size();i++){
            int index = tools.indexOf(toolsName.at(i));
            if(index == -1) continue;
            this->appendTool(toolsName.at(i),QString::number(index));
        }
    }
   // bool * ok = new bool;
    //int type = list[2].toInt(ok);
    //if(! *ok) return UNSERILIZE_FAILED;
    //this->setModelType(type);
    return UNSERILIZE_SUCCESS;
}

void Project::setNetworkFileName(const QString & networkFileName)
{
    this->networkFileName = networkFileName;
    this->isSetNetworkFile= true;
    emit networkFileChanged(this->path,this->networkFileName);
}
void Project::setInputFileName(const QString & inputFileName)
{
    this->inputFileName = inputFileName;
    this->isSetInputFile=true;
    emit inputFileChanged(this->path,this->inputFileName);
}
QString Project::getNetworkFileSuffix()
{
    /*(if(this->robustnessType == PRISM_FILE){
        return ".prism";
    }
    else if(this->robustnessType == JANI_FILE){
        return ".jani";
    }
    else {
        return "";
    }*/
    return ".rlv";
}
QString Project::getInputFileSuffix()
{
    return ".in";
}

QString Project::getAbsoluteNetworkFile()
{
    QDir dir(this->path);
    return dir.absoluteFilePath(this->networkFileName);
}
QString Project::getAbsoluteInputFile()
{
    QDir dir(this->path);
    return dir.absoluteFilePath(this->inputFileName);
}

/*void Project::setRobustnessType(QString robustnessType)
{
    this->robustnessType = robustnessType;
    this->isSetRobustnessType = true;
}

QString Project::getRobustnessType()
{
    return this->robustnessType;
}*/

QString Project::getPath()
{
    return this->path;
}
QString Project::getName()
{
    return this->name;
}
const QStringList * Project::getTools()
{
    return this->tools;
}
bool Project::isValidNetworkFile(){
    return this->isSetNetworkFile;
}
bool Project::isValidInputFile(){
    return this->isSetInputFile;
}
/*bool Project::isValidRobustnessType(){
    return this->isSetRobustnessType;
}*/
//todo
QString Project::generateParametersList()
{
    if(!this->isValidNetworkFile()){
        return "";
    }
    if(!this->isValidInputFile()){
        return "";
    }

    return "--network "+this->getAbsoluteNetworkFile()+" --image "+this->getAbsoluteInputFile()+" --box "+this->getAbsoluteInputFile();
}

QString Project::generateParametersListWithInput(const QString inputfilename)
{
    if(!this->isValidNetworkFile()){
        return "";
    }
    if(!this->isValidInputFile()){
        return "";
    }

    return "--network "+this->getAbsoluteNetworkFile()+" --image "+inputfilename+" --box "+inputfilename;
}
