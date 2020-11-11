#include "settingview.h"
#include "ui_settingview.h"
#include <QDebug>
#include <QJsonArray>
#include <QTabBar>

SettingView::SettingView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingView)
{
    ui->setupUi(this);
    this->ui->setting->setTabsClosable(true);

    connect(this->updateThread,SIGNAL(over()),this,SLOT(on_updateNetworkUIOver()));
    connect(this->network,SIGNAL(showMatrix(Matrix *,const QString)),this,SLOT(on_ShowMatrix(Matrix *,const QString)));
    connect(ui->setting,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTab(int)));
    //statusBar Message
    connect(this->network,SIGNAL(showStatus(const QString)),this,SLOT(on_ShowStatus(const QString)));
    connect(this->network,SIGNAL(addNetworkGraph(const QString, const QString, int, int)), this, SLOT(on_addNetworkGraph(const QString, const QString, int, int)));
}

void SettingView::on_addNetworkGraph(const QString name, const QString layertype, int layernum, int layerwhich)
{
    emit SIGNAL_addNetworkGraph(name, layertype, layernum, layerwhich);
}

void SettingView::removeSubTab(int index)
{
    this->ui->setting->removeTab(index);
}

void SettingView::on_ShowStatus(const QString msg)
{
    emit SIGNAL_showstatus(msg);
}

void SettingView::on_ShowMatrix(Matrix *matrix,const QString label)
{
    this->ui->setting->addTab(matrix,label);
    this->ui->setting->setCurrentWidget(matrix);
}


void SettingView::initConfigurationView_S_L_Connection()
{
    connect(this->cfg,SIGNAL(run()),this,SLOT(verify()));
    connect(this->cfg,SIGNAL(run_ERAN(const QString,const QString,const double)),this,SLOT(verify_ERAN(const QString,const QString,const double)));
    connect(this->cfg,SIGNAL(SIGNAL_importNetwork()),this,SLOT(on_importNetwork()));
    connect(this->cfg,SIGNAL(SIGNAL_importInput()),this,SLOT(on_importInput()));
    connect(this->cfg,SIGNAL(SIGNAL_run_with_input(const QString)),this,SLOT(run_with_input(const QString)));
    connect(this->cfg,SIGNAL(initBatch()),this,SLOT(init_Batch()));
    connect(this->cfg,SIGNAL(overBatch()),this,SLOT(over_Batch()));
    connect(this->cfg,SIGNAL(run_ERAN_Batch(const QString,const QString,const double)),this,SLOT(verify_ERAN_Batch(const QString,const QString,const double)));
    connect(this->cfg,SIGNAL(SIGNAL_run_planet(const QString)),this,SLOT(run_planet(const QString)));
}

void SettingView::initDeepGCfgView_S_L_Connection()
{
    connect(this->deepgcfg,SIGNAL(SIGNAL_Run_DeepG(const QString ,const QString)),this,SLOT(on_Run_DeepG(const QString ,const QString)));
    connect(this->deepgcfg,SIGNAL(SIGNAL_Run_ERAN(const QString ,const QString ,const QString ,int ,int)),this,SLOT(on_Run_ERAN(const QString ,const QString ,const QString ,int ,int)));
    connect(this->deepgcfg,SIGNAL(SIGNAL_Run_Deepsymbol(const QString,const QString,const QString,const double)),this,SLOT(verify_with_augs(const QString,const QString,const QString,const double)));
    connect(this->deepgcfg,SIGNAL(SIGNAL_init_batch()),this,SLOT(init_Batch()));
    connect(this->deepgcfg,SIGNAL(SIGNAL_over_batch()),this,SLOT(over_Batch()));
    connect(this->deepgcfg,SIGNAL(SIGNAL_run_with_input(const QString)),this,SLOT(run_with_input_deepgcfg(const QString)));
}

void SettingView::initReluplexCfgView_S_L_Connection()
{
    connect(this->reluplexconfig,SIGNAL(SIGNAL_run_reluplex(int,bool,const QString)),this,SLOT(run_reluplex(int,bool,const QString)));
}

void SettingView::addTool(QString tool,Project * project)
{
    if(tool == TOOL_BASIC)
    {
        if(this->cfg){
            int index=this->ui->setting->addTab(this->cfg,tool);
            this->ui->setting->tabBar()->tabButton(index,QTabBar::RightSide)->hide();
        }
        else
        {
            this->cfg = new ConfigurationView(project);
            initConfigurationView_S_L_Connection();
            int index=this->ui->setting->addTab(this->cfg,tool);
            this->ui->setting->tabBar()->tabButton(index,QTabBar::RightSide)->hide();
        }
    }
    else if(tool == TOOL_GEOMETRY)
    {
        if(this->deepgcfg){
            int index=this->ui->setting->addTab(this->deepgcfg,tool);
            this->ui->setting->tabBar()->tabButton(index,QTabBar::RightSide)->hide();
        }
        else
        {
            this->deepgcfg= new DeepGcfg(project);
            this->initDeepGCfgView_S_L_Connection();
            int index=this->ui->setting->addTab(this->deepgcfg,tool);
            this->ui->setting->tabBar()->tabButton(index,QTabBar::RightSide)->hide();
        }
    }
    else if(tool == TOOL_RELUPLEX)
    {
        if(this->reluplexconfig){
            int index=this->ui->setting->addTab(this->reluplexconfig,tool);
            this->ui->setting->tabBar()->tabButton(index,QTabBar::RightSide)->hide();
        }
        else
        {
            this->reluplexconfig=new reluplexcfg(project);
            this->initReluplexCfgView_S_L_Connection();
            int index=this->ui->setting->addTab(this->reluplexconfig,tool);
            this->ui->setting->tabBar()->tabButton(index,QTabBar::RightSide)->hide();
        }
    }
}
 void SettingView::on_updateNetworkNodeStatus(QJsonObject obj)
 {
     if(!this->network->isLoaded()) return;
     this->network->clearCount();
     this->network->clearBoundOfNode();
     if(obj.contains("layers"))
     {
         QJsonArray layers = obj.value("layers").toArray();
         int size = layers.size();
         for(int i=0;i<size;i++){
             QJsonObject layerStatus = layers.at(i).toObject();
             QString name = layerStatus.value("name").toString();
             QString type = layerStatus.value("type").toString();
             int numOfNeurons = layerStatus.value("number_of_neurons").toInt();
             int index = this->network->layerIndexOf(name);
             if(index == layerStatus.value("index").toString().toInt())
             {
                 QJsonObject::Iterator it;
                 QJsonObject node = layerStatus.value("bound_of_neurons").toObject();
                 for(it=node.begin();it!=node.end();it++)
                 {
                     QJsonObject bound =it.value().toObject();
                     QString nodeName =it.key();
                     this->network->setBoundOfNode(index,nodeName,bound.value("inf").toString(),bound.value("sup").toString());
                 }
            }
             else{
                 qDebug() << " layer index is not correct:"<<index <<layerStatus.value("index").toString().toInt();
             } 

         }
         this->ui->setting->setCurrentWidget(this->network);
     }
     emit SIGNAL_programout(this->network->getNeuronStatus());
 }


 void SettingView::on_networkFileChanged(QString path,QString fileName)
 {
     if(this->cfg)
        this->cfg->changeNetworkFile(path,fileName);

     //this->network->loadNetworkfile(path,fileName);
     //set the network name and path
     this->network->setFileName(fileName);
     this->network->setFilePath(path);
     updateThread->setParameters(path,fileName);
     //this->loadWiget->show();
//     this->loadWiget= new LoadingWidget();
//     this->loadWiget->show();
     updateThread->start();
//     updateThread->run_without_thread();
     //qDebug()<<updateThread->stackSize();
 }

 void SettingView::on_networkFileChangedTextOnly(QString path,QString filename)
 {
     if(this->cfg)this->cfg->changeNetworkFile(path,filename);
 }

 void SettingView::on_inputFileChanged(QString path,QString fileName)
 {
     if(this->cfg)
        this->cfg->changeInputFile(path,fileName);
 }

void SettingView::removeTool(QString tool)
{
    if(tool == TOOL_BASIC){
        if(!this->cfg) return;
        this->ui->setting->removeTab(this->ui->setting->indexOf(this->cfg));
        delete this->cfg;
        this->cfg= nullptr;
       // this->network->reload();
        //this->loadWiget->show();
//        this->loadWiget= new LoadingWidget();
//        this->loadWiget->show();
        //this->updateThread->start();
        this->network->clearBoundOfNode();
    }
    else if(tool == TOOL_GEOMETRY)
    {
        if(!this->deepgcfg)return;
        this->ui->setting->removeTab(this->ui->setting->indexOf(this->deepgcfg));
        delete this->deepgcfg;
        this->deepgcfg=nullptr;
        this->network->clearBoundOfNode();
    }
    else if(tool == TOOL_RELUPLEX)
    {
        if(!this->reluplexconfig)return;
        this->ui->setting->removeTab(this->ui->setting->indexOf(this->reluplexconfig));
        delete this->reluplexconfig;
        this->reluplexconfig=nullptr;
    }
}

void SettingView::on_updateNetworkUIOver()
{
    qDebug() << "update network UI Ok";
    //this->loadWiget->hide();
    this->network->updateUI(this->updateThread->getLayers());
    //if(this->loadWiget)
      //  this->loadWiget->close();
}

void SettingView::on_addTool(QString tool, Project * p)
{
    this->addTool(tool,p);
}
void SettingView::on_removeTool(QString tool)
{
    this->removeTool(tool);
}



void SettingView::openProject(Project * p)
{
    if(p == nullptr)
    {
        qDebug() <<"setting view receive a invalid project";
        return;
    }
    //TODO: add network ....
    //this->network->loadNetworkfile(p->getPath(),p->getNetworkFileName());
   // updateThread->setParameters(p->getPath(),p->getNetworkFileName(),network);
    int index=this->ui->setting->addTab(this->network, "Neurons");
    this->ui->setting->tabBar()->tabButton(index,QTabBar::RightSide)->hide();    

    for(int i=0;i<p->getTools()->size();i++)
    {
        this->addTool(p->getTools()->at(i),p);
    }

}


void SettingView::closeProject()
{

    this->ui->setting->clear();
    if(this->network){
        this->network->saveNetwork();
        delete  this->network;
    }
    if(this->cfg){
        delete this->cfg;
    }

    this->network=new Network();
    this->cfg=nullptr;
    connect(this->network,SIGNAL(showMatrix(Matrix *,const QString)),this,SLOT(on_ShowMatrix(Matrix *,const QString)));
}

double SettingView::getDelta()
{
    if(this->cfg == nullptr) return 0;
    return this->cfg->getDelta();
}
QString SettingView::getRobustnessType()
{
    if(this->cfg == nullptr) return "";
    return this->cfg->getRobustnessType();
}


void SettingView::verify()
{
    this->network->saveNetwork();
    emit SIGNAL_run();
}

void SettingView::verify_with_augs(const QString inputfilename,const QString network,const QString robustness,const double delta)
{
    emit SIGNAL_run_with_augs(inputfilename,network,robustness,delta);
}

void SettingView::verify_ERAN(const QString inputfile,const QString network,const double epsilon)
{
    this->network->saveNetwork();
    emit SIGNAL_Run_ERAN_Raw(inputfile,network,epsilon);
}

void SettingView::verify_ERAN_Batch(const QString inputfolder,const QString network,const double epsilon)
{
    this->network->saveNetwork();
    emit SIGNAL_Run_ERAN_Raw_Batch(inputfolder,network,epsilon);
}

void SettingView::run_with_input(const QString inputfilename)
{
    emit SIGNAL_run_with_input(inputfilename);
}

void SettingView::run_with_input_deepgcfg(const QString inputfilename)
{
    emit SIGNAL_run_with_input_deepgcfg(inputfilename);
}

void SettingView::batchNext()
{
    if(this->cfg)this->cfg->on_batch_run();
}

void SettingView::batchNextDeepgcfg()
{
    qDebug("123");
    if(this->deepgcfg)this->deepgcfg->batch_run();
}

void SettingView::init_Batch()
{
    emit SIGNAL_init_Batch();
}

void SettingView::over_Batch()
{
    emit SIGNAL_over_Batch();
}

void SettingView::on_Run_DeepG(const QString binpath,const QString configdir)
{
    emit SIGNAL_Run_DeepG(binpath,configdir);
}

void SettingView::on_Run_ERAN(const QString network,const QString dataset,const QString configdir,int paramnum,int inputnum)
{
    emit SIGNAL_Run_ERAN(network, dataset, configdir, paramnum, inputnum);
}

void SettingView::run_reluplex(int timeout,bool withDeepsymbol,const QString parameters)
{
    emit SIGNAL_run_reluplex(timeout,withDeepsymbol,parameters);
}

void SettingView::run_planet(const QString args)
{
    emit SIGNAL_run_planet(args);
}

void SettingView::on_importNetwork()
{
    emit this->SIGNAL_importNetwork();
}
void SettingView::on_importInput()
{
    emit this->SIGNAL_importInput();
}
void SettingView::showlayer(int layer)
{
    if(this->network)
    {
        this->network->showlayer(layer);
        this->ui->setting->setCurrentWidget(this->network);
    }
}

SettingView::~SettingView()
{
    delete this->updateThread;
    delete ui;
}
