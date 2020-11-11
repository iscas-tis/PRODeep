#include "mainui.h"
#include "ui_mainui.h"
#include <QPushButton>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QDebug>
#include "util.h"
#include <QJsonArray>
#include <QDir>
#include "selectdialog.h"
#include <QDialog>
#include <QLabel>
#include "util.h"

MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    this->initUI();

    QDir dir = QDir::tempPath();
    dir.cd("Qt");
    dir.cd("lib");
    backend = new BackEnd(dir.absoluteFilePath("deepsymbol"));
//    deepgbackend = new BackEnd("/home/deepui/deepg/code/build/deepg_constraints");
    deepgbackend = new BackEnd(DEEPG_BACKEND);
    connect(deepgbackend, SIGNAL(sendOut(char * )), this,SLOT(on_readoutput(char *)) );

    eranbackend = new BackEnd(DEEPG_ERAN_BACKEND);
    connect(eranbackend, SIGNAL(sendOut(char * )), this,SLOT(on_readoutput(char *)) );

    eranrawbackend = new BackEnd(ERAN_BACKEND);
    connect(eranrawbackend,SIGNAL(sendOut(char * )), this, SLOT(on_readoutput(char *)));
    connect(eranrawbackend,SIGNAL(finished(int)),this,SLOT(verifyFinished(int)));

    reluplexbackend = new BackEnd("timeout");
    connect(reluplexbackend,SIGNAL(sendOut(char *)),this,SLOT(on_readoutput(char *)));

    planetbackend = new BackEnd(PLANET_BACKEND);
    connect(planetbackend,SIGNAL(sendOut(char *)),this,SLOT(on_readoutput(char *)));

   // backend = new BackEnd("/home/xuechao/xuechao/work/Lab/DeepAI/deepsymbol/deepsymbol");
    connect(backend, SIGNAL(sendOut(char * )), this,SLOT(on_readoutput(char *)) );
    connect(backend, SIGNAL(finished(int)), this, SLOT(verifyFinished(int)));
    connect(backend, SIGNAL(batchfinished(int)), this, SLOT(batchverifyFinished(int)));
    //this->resultView->parsingJsonFile(JSON_RESULT_FILE);
}

void MainUI::initUI()
{
    this->projectView = new ProjectView();

    this->outView = new OutView();
    this->settingView = new SettingView();
    this->resultView = new ResultView();       

    this->ui->splitterMain->setStretchFactor(0,1);
    this->ui->splitterMain->setStretchFactor(1,10);
    this->ui->splitterMain->setChildrenCollapsible(false);
    this->ui->splitterMain->setMinimumWidth(this->size().width()*2/10);

    this->ui->splitterOut->setStretchFactor(0,30);
    this->ui->splitterOut->setStretchFactor(1,1);
    this->ui->splitterOut->setChildrenCollapsible(false);
    this->ui->splitterOut->setMinimumHeight(this->size().height()*5/10);

    this->ui->splitterResult->setStretchFactor(0,3);
    this->ui->splitterResult->setStretchFactor(1,2);
    this->ui->splitterResult->setChildrenCollapsible(false);
    this->ui->splitterResult->setMinimumWidth(this->size().width()*5/10);

    this->ui->projectView->addWidget(this->projectView);
    this->ui->setView->addWidget(settingView);
    this->ui->resultView->addWidget(resultView);
    this->ui->outView->addWidget(this->outView);

    this->projectView->hide();

    //ui->centralwidget->createWinId()
    //ui->centralwidget->setLayout(mainLayout);
    //setLayout(mainLayout);
    //this->ui->test->addWidget(settingView);


    //this->ui->centralwidget->

    //this->ui->Setting->addTab(nt, "Network");

    connect(this->projectView,SIGNAL(SIGNAL_OpenProject(Project *)),this->settingView, SLOT(openProject(Project *)));
    connect(this->projectView,SIGNAL(SIGNAL_CloseProject()), this->settingView, SLOT(closeProject()));
    connect(this->projectView,SIGNAL(SIGNAL_networkChanged(QString, QString)),this->settingView, SLOT(on_networkFileChanged(QString,QString)));
    connect(this->projectView,SIGNAL(SIGNAL_networkChangedTextOnly(QString, QString)),this->settingView, SLOT(on_networkFileChangedTextOnly(QString,QString)));
    connect(this->projectView,SIGNAL(SIGNAL_inputChanged(QString,QString)), this->settingView, SLOT(on_inputFileChanged(QString,QString)));
    connect(this->projectView,SIGNAL(SIGNAL_addTool(QString,Project *)),this->settingView, SLOT(on_addTool(QString, Project *)));
    connect(this->projectView,SIGNAL(SIGNAL_removeTool(QString)), this->settingView, SLOT(on_removeTool(QString)));


    connect(this->settingView,SIGNAL(SIGNAL_importNetwork()), this->projectView, SLOT(on_importNetwork()));
    connect(this->settingView,SIGNAL(SIGNAL_importInput()),this->projectView,SLOT(on_importInput()));

    connect(this->settingView,SIGNAL(SIGNAL_run()),this,SLOT(on_verify()));
    connect(this->settingView,SIGNAL(SIGNAL_Run_ERAN_Raw(const QString,const QString,const double)),this,SLOT(on_verify_ERAN(const QString,const QString,const double)));
    connect(this->settingView,SIGNAL(SIGNAL_Run_ERAN_Raw_Batch(const QString,const QString,const double)),this,SLOT(on_verify_ERAN_Batch(const QString,const QString,const double)));
    connect(this->settingView,SIGNAL(SIGNAL_run_with_input(const QString)),this,SLOT(verify_with_input(const QString)));
    connect(this->settingView,SIGNAL(SIGNAL_run_with_input_deepgcfg(const QString)),this,SLOT(verify_with_input_deepgcfg(const QString)));
    connect(this->settingView,SIGNAL(SIGNAL_init_Batch()),this,SLOT(init_Batch()));
    connect(this->settingView,SIGNAL(SIGNAL_over_Batch()),this,SLOT(over_Batch()));
    connect(this->settingView,SIGNAL(SIGNAL_addNetworkGraph(const QString, const QString, int, int)),this->resultView,SLOT(on_addNetworkGraph(const QString, const QString, int, int)));    
    connect(this->settingView,SIGNAL(SIGNAL_Run_DeepG(const QString ,const QString)),this,SLOT(on_Run_DeepG(const QString ,const QString)));
    connect(this->settingView,SIGNAL(SIGNAL_Run_ERAN(const QString ,const QString ,const QString ,int ,int)),this,SLOT(on_Run_ERAN(const QString ,const QString ,const QString ,int ,int)));
    connect(this->resultView,SIGNAL(updateNetworkNodeStatus(QJsonObject)),this->settingView,SLOT(on_updateNetworkNodeStatus(QJsonObject)));
    connect(this->resultView,SIGNAL(SIGNAL_showlayer(int)),this->settingView,SLOT(showlayer(int)));    
    connect(this->settingView,SIGNAL(SIGNAL_run_with_augs(const QString,const QString,const QString,const double)),this,SLOT(verify_with_augs(const QString,const QString,const QString,const double)));
    connect(this->settingView,SIGNAL(SIGNAL_run_reluplex(int,bool,const QString)),this,SLOT(run_reluplex(int,bool,const QString)));
    connect(this->settingView,SIGNAL(SIGNAL_run_planet(const QString)),this,SLOT(run_planet(const QString)));
    //program out
    connect(this->settingView,SIGNAL(SIGNAL_programout(const QString)),this,SLOT(on_updateProgramOut(const QString)));
    //status bar out
    connect(this->settingView,SIGNAL(SIGNAL_showstatus(const QString)),this,SLOT(on_showStatusBar(const QString)));

    statusBar()->addWidget(this->statusBarLabel,1);
}
inline QString generateAbsoluteResultJsonFile(QString jsonFile)
{
    QDir dir = QDir::tempPath();
    dir.cd("Qt");
    dir.cd("lib");
    return dir.absoluteFilePath(jsonFile);
}
void MainUI::Run(){
 if(!this->projectView->isValidProject()) return;
 QString projectParameters = this->projectView->getProjectParameters();
 QString othersParameters = " --robust "+this->settingView->getRobustnessType()+
                            " --delta "+ QString::number(this->settingView->getDelta()) +
                            " --dumpJSON "+generateAbsoluteResultJsonFile(JSON_RESULT_FILE);

 backend->setCurrentParametersList(projectParameters+" "+othersParameters);


 this->outView->clear(); 
 backend->run();
}

void MainUI::verify_with_augs(const QString inputfilename,const QString network,const QString robustness,const double delta)
{
    QString parameters = "--box "+inputfilename+
            " --image "+inputfilename+
            " --network "+network+
            " --delta "+QString::number(delta)+
            " --robust "+robustness+
            " --dumpJSON "+generateAbsoluteResultJsonFile(JSON_RESULT_FILE);
    backend->setCurrentParametersList(parameters);
    this->outView->clear();
    backend->run();
}

void MainUI::Run_ERAN(const QString inputfile,const QString network,const double epsilon){
    if(!this->projectView->isValidProject())return;
    QString parameters="--netname "+network+
            " --epsilon "+QString::number(epsilon)+
            " --domain deeppoly"+
            " --dataset mnist"+
            " --input "+inputfile+
            " --relation_diagram "+generateAbsoluteResultJsonFile(JSON_RESULT_FILE);
    //" --input_folder folder"
    qDebug()<<ERAN_BACKEND+parameters;
    eranrawbackend->setCurrentParametersList(parameters);
    this->outView->clear();
    eranrawbackend->run(false,DEEPG_ERAN_DIR);
}

void MainUI::on_verify_ERAN_Batch(const QString inputfolder,const QString network,const double epsilon)
{
    if(!this->projectView->isValidProject())return;
    QString parameters="--netname "+network+
            " --epsilon "+QString::number(epsilon)+
            " --domain deeppoly"+
            " --dataset mnist"+
            " --input_folder "+inputfolder;
    //" --input_folder folder"
    qDebug()<<ERAN_BACKEND+parameters;
    eranrawbackend->setCurrentParametersList(parameters);
    this->outView->clear();
    eranrawbackend->run(false,DEEPG_ERAN_DIR);
}

void MainUI::on_Run_DeepG(const QString binpath,const QString configdir)
{
    deepgbackend->setCurrentParametersList(configdir);
    this->outView->clear();
//    qDebug()<<"!";
    deepgbackend->run(false,DEEPG_DIR);
}

void MainUI::on_Run_ERAN(const QString network,const QString dataset,const QString configdir,int paramnum,int inputnum)
{
    QString parameters=" --net "+network+
            " --dataset "+dataset+
            " --data_dir "+configdir+
            " --num_params "+QString::number(paramnum)+
            " --num_tests "+QString::number(inputnum);
    eranbackend->setCurrentParametersList(parameters);
    this->outView->clear();
    eranbackend->run(false,DEEPG_ERAN_DIR);
}

void MainUI::run_reluplex(int timeout,bool withDeepsymbol,const QString parameters)
{
    QString params=QString(" --foreground ")+
            " --signal=SIGQUIT "+
            " "+QString::number(timeout)+"m ";
    if(withDeepsymbol)params+=QString(" ")+RELUPLEX_WITH_DEEPSYMBOL_BACKEND+" ";else params+=QString(" ")+RELUPLEX_BACKEND+" ";
    params+=parameters;
    //qDebug()<<params;
    reluplexbackend->setCurrentParametersList(params);
    this->outView->clear();
    reluplexbackend->run();
}

void MainUI::run_planet(const QString args)
{
    qDebug()<<args;
    planetbackend->setCurrentParametersList(args);
    this->outView->clear();
    planetbackend->run(false,PLANET_DIR);
}

void MainUI::verifyFinished(int exitCode)
{
    if(exitCode == 0)
    {
        this->resultView->parsingJsonFile(generateAbsoluteResultJsonFile(JSON_RESULT_FILE));
    }
}

void MainUI::parsingbatchout()
{
    const QString jsonFile=generateAbsoluteResultJsonFile(JSON_RESULT_FILE);
    QJsonObject * jsonObj = Util::parseJsonFile(jsonFile);
    if(jsonObj==nullptr) {
        qDebug() << "jsonObj is unvalid";
        return ;
    }

    if(jsonObj->contains("graph"))
    {
        QJsonObject graph = jsonObj->value("graph").toObject();
        int nodes = 0;
        if(graph.contains("number_of_nodes")){
            nodes = graph.value("number_of_nodes").toString().toInt();
        }
        QStringList names;
        QStringList possible;
        if(graph.contains(("names")))
        {
            QJsonValue value = graph.value("names");
            if(value.isArray())
            {
                QJsonArray array = value.toArray();
                int size =array.size();
                for(int i=0;i<size;i++){
                    names.append(array.at(i).toString());
                    possible.append(array.at(i).toString());
                }
            }
        }

        if(graph.contains("edges"))
        {
            QJsonValue value = graph.value("edges");
            if(value.isArray())
            {
                QJsonArray array = value.toArray();
                int size =array.size();
                for(int i=0;i<size;i++){
                    int indexFrom = array.at(i).toObject().value("from").toString().toInt();
                    int indexTo = array.at(i).toObject().value("to").toString().toInt();
                    //qDebug()<<names.at(indexFrom);
                    possible.removeOne(names.at(indexFrom));
                }
            }
        }

        if(possible.size()==1)
        {
            this->outView->appendProgramText("Robust to ");
            this->outView->appendProgramText(possible.first());
            this->outView->appendProgramText("\n");
            this->robustnum++;
        }
        else
        {
            this->outView->appendProgramText("Possible Output: ");
            for (int i=0; i<possible.size(); i++) {
                this->outView->appendProgramText(possible.at(i));
                this->outView->appendProgramText("   ");
            }
            this->outView->appendProgramText("\n");            
        }
    }
    this->verifiednum++;
    this->outView->appendProgramText(QString("Verified:")+QString::number(this->verifiednum)+" Robust:"+QString::number(this->robustnum)+" Robust Percentage:"+
                              QString::number(double(this->robustnum)/this->verifiednum*100,'g',5));
    this->outView->appendProgramText("%\n");
}

void MainUI::batchverifyFinished(int exitCode)
{
    if(exitCode == 0)
    {
        this->parsingbatchout();
    }
    else
    {
        QString errormsg;
        QTextStream(&errormsg)<<"Error Exit Code: "<<exitCode<<"\n";
        this->outView->setProgramText(errormsg);
    }
    qDebug()<<this->curbatch;
    if(this->curbatch==1)this->settingView->batchNext();
    if(this->curbatch==2)this->settingView->batchNextDeepgcfg();
}

void MainUI::init_Batch()
{
    this->robustnum=0;
    this->verifiednum=0;
    this->ui->actionRun->setDisabled(true);
    this->outView->clear();
}

void MainUI::over_Batch()
{
    this->curbatch=0;
    this->ui->actionRun->setEnabled(true);
}

void MainUI::on_readoutput(char * out)
{    
    QString str(out);    
    QList<QString> result = this->filter(str);
//    QList<QString> result = str.split("\n");
    //ui->outText->append(result);
    for(QString line : result){
        this->outView->appendText(line);
    }    
}
QList<QString> MainUI::filter(QString out){
    QList<QString> lines = out.split("\n");
    QList<QString> result;
    QString line;
    int currentLevel = 0;
    for(int i=0;i<lines.size()-1;i++){
    //foreach (line, lines) {
        line = lines.at(i);
        line.replace(" ","   ");
//        line = line.trimmed();
        /*if(line.startsWith("[Info]")){
            currentLevel = 0;
        }
        else if(line.startsWith("[Worning]")){
            currentLevel = 1;

        }
        else if(line.startsWith("[Error]"))
        {
            currentLevel = 2;
        }
        if(currentLevel >= this->filterLevel)*/{
            result.append(line+"\n");
        }
    }
    /*if(currentLevel >= this->filterLevel)*/{
        result.append(lines.at(lines.size()-1));
    }
    return result;
}



MainUI::~MainUI()
{
    delete ui;
}


void MainUI::on_verify()
{
    this->Run();
}
void MainUI::on_verify_ERAN(const QString inputfile,const QString network,const double epsilon)
{
    this->Run_ERAN(inputfile,network,epsilon);
}
void MainUI::verify_with_input(const QString inputfilename)
{
    if(this->curbatch==0 || this->curbatch==1)this->curbatch=1;else return;
    this->outView->appendProgramText(inputfilename);
    this->outView->appendProgramText(":    ");
    if(!this->projectView->isValidProject()) return;
    QString projectParameters = this->projectView->getProjectParametersWithInput(inputfilename);
    QString othersParameters = " --robust "+this->settingView->getRobustnessType()+
                               " --delta "+ QString::number(this->settingView->getDelta()) +
                               " --dumpJSON "+generateAbsoluteResultJsonFile(JSON_RESULT_FILE);

    backend->setCurrentParametersList(projectParameters+" "+othersParameters);


    backend->run(true);
}

void MainUI::verify_with_input_deepgcfg(const QString inputfilename)
{
    if(this->curbatch==0 || this->curbatch==2)this->curbatch=2;else return;
    this->outView->appendProgramText(inputfilename);
    this->outView->appendProgramText(":    ");
    if(!this->projectView->isValidProject()) return;
    QString projectParameters = this->projectView->getProjectParametersWithInput(inputfilename);
    QString othersParameters = QString(" --robust CUSTOMBOX")+
                               " --delta 0"+
                               " --dumpJSON "+generateAbsoluteResultJsonFile(JSON_RESULT_FILE);

    backend->setCurrentParametersList(projectParameters+" "+othersParameters);


    backend->run(true);
}

void MainUI::closeEvent(QCloseEvent *event)
{
    this->projectView->closeProject();
}
void MainUI::on_action_New_Project_triggered()
{
    this->projectView->newProject();
}

void MainUI::on_action_Open_Project_triggered()
{
    this->projectView->openProject();
}

void MainUI::on_actionClose_Project_triggered()
{
    this->projectView->closeProject();
}

void MainUI::on_actionRun_triggered()
{
    this->Run();
}

void MainUI::on_actionAdd_Tool_triggered()
{
    this->projectView->addTool();
}

void MainUI::on_actionRemove_Tool_triggered()
{
    this->projectView->removeTool();
}

void MainUI::on_actionImport_Network_triggered()
{
    this->projectView->addNetwork();
}

void MainUI::on_actionImport_Input_triggered()
{
    this->projectView->addInput();
}

void MainUI::on_updateProgramOut(const QString out)
{
    this->outView->setProgramText(out);
}

void MainUI::on_showStatusBar(const QString msg)
{
    this->statusBarLabel->setText(msg);
}
