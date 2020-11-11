#include "projectview.h"
#include "ui_projectview.h"

#include <QFileDialog>
#include <QLineEdit>
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

#include "choose.h"
#include "util.h"
#include "selectdialog.h"

ProjectView::ProjectView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectView)
{
    ui->setupUi(this);
    connect(ui->projectDir,SIGNAL(doubleClicked(const QModelIndex)),this,SLOT(treeViewDoubleClick(const QModelIndex)));

    QTreeView *treeViewDir=ui->projectDir;
    projectModel = new ProjectModel();
    treeViewDir->setModel(projectModel);
    treeViewDir->showColumn(0);
    treeViewDir->hideColumn(1);

}

inline void ProjectView::initProject_S_L_Connection()
{

    connect(this->project,SIGNAL(networkFileChanged(QString,QString)),this, SLOT(on_addNetworkFromProject(QString,QString)));
    connect(this->project,SIGNAL(inputFileChanged(QString,QString)),this, SLOT(on_addInputFromProject(QString,QString)));
    connect(this->project,SIGNAL(addTool(QString,QString,Project*)),this, SLOT(on_addToolFromProject(QString,QString,Project *)));
    connect(this->project,SIGNAL(removeTool(QString,QString)),this, SLOT(on_removeToolFromProject(QString,QString)));
}

void ProjectView::addNetwork()
{
    if(!this->isLoadProject) return;
    if(this->project == nullptr)
    {
        qDebug() << "project is null";
        return;
    }
    QString file = QFileDialog::getOpenFileName(this,tr("Choose Network File"), this->project->getPath());
    if(file != "")
    {
        QString suffix = this->project->getNetworkFileSuffix();
        if(!file.endsWith(suffix)){
            QFileInfo info(file);
            emit SIGNAL_networkChangedTextOnly(info.absolutePath(),info.fileName());
            //QMessageBox::warning(this, "Worning", "Invalid File!\n Please open file with suffix"+suffix);
            return;
        }

        QFileInfo info(file);

        if(info.absolutePath().compare(this->project->getPath()) != 0){
            QDir dir(this->project->getPath());
            QString destFile = dir.absoluteFilePath(info.fileName());
            qDebug() << info.absoluteFilePath() << destFile;
            QFileInfo destInfo(destFile);
            if(destInfo.exists())
                QFile::remove(destFile);
            QFile::copy(info.absoluteFilePath(), destFile);

            this->project->setNetworkFileName(destInfo.fileName());
            return;
        }
        this->project->setNetworkFileName(info.fileName());
    }
   // this->project->setNetworkFileName(fileName);
}
void ProjectView::addInput()
{
    if(!this->isLoadProject) return;
    if(this->project == nullptr)
    {
        qDebug() << "project is null";
        return;
    }
    QString file = QFileDialog::getOpenFileName(this,tr("Choose Input File"), this->project->getPath());
    if(file != "")
    {
        QString suffix = this->project->getInputFileSuffix();
        if(!file.endsWith(suffix)){
            QMessageBox::warning(this, "Worning", "Invalid File!\n Please open file with suffix"+suffix);
            return;
        }

        QFileInfo info(file);
        if(info.absolutePath().compare(this->project->getPath()) != 0){
            QDir dir(this->project->getPath());
            QString destFile = dir.absoluteFilePath(info.fileName());
            QFile::copy(info.absoluteFilePath(), destFile);
            QFileInfo destInfo(destFile);

            this->project->setInputFileName(destInfo.fileName());
            return;

        }

        this->project->setInputFileName(info.fileName());

    }
   // this->project->setInputFileName(fileName);
}
void ProjectView::addTool()
{
    if(!this->isLoadProject) return;
    QStringList CandidateTools = this->getCandidateTools();
    SelectDialog diag("Tools",&CandidateTools);
    if(diag.exec() == QDialog::Accepted)
    {
        QString tool = diag.getResult();
        int index = CandidateTools.indexOf(tool);
        if(index ==-1) return;
        this->project->appendTool(tool,QString::number(index));
        //this->addTool(tool,QString::number(index));
        //this->settingView->addTool(tool);
    }

}
void ProjectView::removeTool()
{
    if(!this->isLoadProject) return;
    QStringList CandidateTools = this->getAddedTools();
    SelectDialog diag("Tools",&CandidateTools);
    if(diag.exec() == QDialog::Accepted)
    {
        QString tool = diag.getResult();
        int index = CandidateTools.indexOf(tool);
        if(index ==-1) return;
        this->project->deleteTool(tool,QString::number(index));
        //this->settingView->removeTool(tool);
    }
}

void ProjectView::on_importNetwork()
{
    this->addNetwork();
}
void ProjectView::on_importInput()
{
    this->addInput();
}

void ProjectView::on_addToolFromProject(QString tool,QString index,Project * project)
{
    this->addToolInModel(tool,index);
    emit this->SIGNAL_addTool(tool,project);
}
void ProjectView::on_removeToolFromProject(QString tool,QString index)
{
    this->removeToolInModel(tool,index);
    emit this->SIGNAL_removeTool(tool);
}

void ProjectView::on_addNetworkFromProject(QString path,QString fileName)
{
    this->addNetworkInModel(path,fileName);
    emit this->SIGNAL_networkChanged(path,fileName);
}
void ProjectView::on_addInputFromProject(QString path,QString fileName)
{
    this->addInputInModel(path,fileName);
    emit this->SIGNAL_inputChanged(path,fileName);
}

void ProjectView::addToolInModel(QString tool,QString index)
{
    this->projectModel->addTool(new ProjectItem({tool, index}));
}
void ProjectView::removeToolInModel(QString tool,QString index)
{
    this->projectModel->removeTool(new ProjectItem({tool, index}));
}

void ProjectView::addNetworkInModel(QString path,QString fileName)
{
    this->projectModel->addFile(new ProjectItem({fileName, path}),this->project->getNetworkFileSuffix());
}
void ProjectView::addInputInModel(QString path,QString fileName)
{
    this->projectModel->addFile(new ProjectItem({fileName, path}),this->project->getInputFileSuffix());
}

QString ProjectView::getProjectParameters()
{
    return this->project->generateParametersList();
}

QString ProjectView::getProjectParametersWithInput(const QString inputfilename)
{
    return  this->project->generateParametersListWithInput(inputfilename);
}
bool ProjectView::isValidProject()
{
    return this->isLoadProject;
}

void ProjectView::treeViewDoubleClick(const QModelIndex & index)
{
    //curtextEdit->setText(dirModel->filePath(index));
    //loadFile(dirModel->filePath(index));
    int type = this->projectModel->getSelectTreeNodeType(index);
    if(type == TOOL_NODE)
    {
        emit this->SIGNAL_addTool(this->projectModel->getSelectTreeNodeName(index),this->project);
    }
}


int ProjectView::openProject()
{
    if(this->isLoadProject)this->closeProject();
    QFileDialog dialog(this);
    //dialog.setAcceptMode(QFileDialog::AcceptOpen);
    //dialog.setViewMode(QFileDialog::List);
   // dialog.setFileMode(QFileDialog::AnyFile);
   // dialog.setWindowTitle(tr("Open Project"));
    //dialog.setDefaultSuffix("pro");
    //dialog.setOption(QFileDialog::DontUseNativeDialog);

    dialog.setWindowModality(Qt::ApplicationModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDirectory(QDir::home());
    QStringList nameFilters;
    nameFilters << "Project Files(*.pro)";
    dialog.setNameFilters(nameFilters);
    if (dialog.exec() != QDialog::Accepted)
        return -1;
    while(this->isLoadProject){
        this->closeProject();
    }
    QString file =  dialog.selectedFiles().first();
    QFileInfo info(file);
    this->project = new Project(info.path(),info.baseName());
    projectModel->addProject(this->project);

    initProject_S_L_Connection();

    this->isLoadProject = true;
    QStringList CandidateTools(TOOLS);
    this->project->open(file,CandidateTools);
//    if(this->project->isValidNetworkFile()){
//        projectModel->addFile(new ProjectItem({this->project->getNetworkFileName(),this->project->getPath()}));
//    }
//    if(this->project->isValidInputFile()){
//        projectModel->addFile(new ProjectItem({this->project->getInputFileName(),this->project->getPath()}));
//    }

    //ui->DirTree->repaint();
   // ui->DirTree->setRootIndex(dirModel->index(p->getPath()));
    emit SIGNAL_OpenProject(this->project);
    return 0;
}

int ProjectView::newProject(){
    ///QInputDialog to get the name of project
    QString dlgTitle="New Project";
    QString txtLabel="Project Name";
    QString defaultInput="NewProject";
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;
    bool ok;
    QString projectName = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,defaultInput, &ok);
    if(! ok) return CREATE_FILE_FAILED;
    Choose * path = new Choose("Choose Project Path",false);
    if(path->exec() ==  QDialog::Accepted){
        while(this->isLoadProject){
            this->closeProject();
        }
        QString projectPath = path->getValue();

        QDir dir(projectPath);
        int result = Util::createFile(dir.absolutePath(),projectName+".pro");
        if(result == CREATE_FILE_FAILED){
            qDebug()<<"exist ";
            return CREATE_FILE_FAILED;
        }
        this->project = new Project(projectPath,projectName);

        initProject_S_L_Connection();

        this->isLoadProject = true;
        projectModel->addProject(this->project);

        //ui->DirTree->setRootIndex(dirModel->index(projectPath));
        emit SIGNAL_OpenProject(this->project);
        return CREATE_FILE_SUCCESS;
    }
    return CREATE_FILE_FAILED;



}
void ProjectView::closeProject(){
    if(! this->isLoadProject) return;
    this->project->save();
    projectModel->closeProject(0);

    emit SIGNAL_CloseProject();
    //close project
    delete this->project;
    delete this->projectModel;
    this->isLoadProject = false;

    QTreeView *treeViewDir=ui->projectDir;
    this->projectModel = new ProjectModel();
    treeViewDir->setModel(this->projectModel);
    treeViewDir->showColumn(0);
    treeViewDir->hideColumn(1);
    this->project=nullptr;

}

QStringList ProjectView::getCandidateTools()
{
    QStringList CandidateTools(TOOLS);
    if(this->project)
    {
       const QStringList * tools = this->project->getTools();
       for(int i=0;i<tools->size();i++)
       {
           CandidateTools.removeOne(tools->at(i));
       }
    }
    return CandidateTools;
}
QStringList ProjectView::getAddedTools()
{
    if(this->project){
        return *this->project->getTools();
    }
    else
        return QStringList();
}
ProjectView::~ProjectView()
{
    delete ui;
}
