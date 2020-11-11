#ifndef SETTINGVIEW_H
#define SETTINGVIEW_H

#include <QWidget>
#include "network.h"
#include "configurationview.h"
#include <QJsonObject>
#include "updatenetworkthread.h"
#include "loadingwidget.h"
#include "deepgcfg.h"
#include "reluplexcfg.h"


namespace Ui {
class SettingView;
}

class SettingView : public QWidget
{
    Q_OBJECT

public:
    explicit SettingView(QWidget *parent = nullptr);
    ~SettingView();

    double getDelta();
    QString getRobustnessType();
    void batchNext();
    void batchNextDeepgcfg();

public slots:
    void openProject(Project * p);
    void closeProject();
    void verify();
    void verify_with_augs(const QString inputfilename,const QString network,const QString robustness,const double delta);
    void verify_ERAN(const QString inputfile,const QString network,const double epsilon);
    void verify_ERAN_Batch(const QString inputfolder,const QString network,const double epsilon);
    void on_addTool(QString tool, Project* project);
    void on_removeTool(QString tool);
    void on_updateNetworkNodeStatus(QJsonObject obj);
    void on_networkFileChanged(QString path,QString fileName);//TODO
    void on_networkFileChangedTextOnly(QString path,QString filename);
    void on_inputFileChanged(QString path,QString fileName);//TODO
    void on_importNetwork();
    void on_importInput();
    void on_Run_DeepG(const QString binpath,const QString configdir);
    void on_Run_ERAN(const QString network,const QString dataset,const QString configdir,int paramnum,int inputnum);
    void run_reluplex(int timeout,bool withDeepsymbol,const QString parameters);
    void run_planet(const QString args);

    void on_updateNetworkUIOver();
    void on_ShowMatrix(Matrix *matrix,const QString label);
    void removeSubTab(int index);
    void on_ShowStatus(const QString msg);
    void run_with_input(const QString inputfilename);
    void run_with_input_deepgcfg(const QString inputfilename);
    void init_Batch();
    void over_Batch();
    void on_addNetworkGraph(const QString name, const QString layertype, int layernum, int layerwhich);
    void showlayer(int layer);

private:
    Ui::SettingView *ui;

     Network * network = new Network();
    ConfigurationView * cfg = nullptr;    
    DeepGcfg *deepgcfg = nullptr;
    reluplexcfg *reluplexconfig = nullptr;
    UpdateNetworkThread * updateThread = new UpdateNetworkThread();
    LoadingWidget * loadWiget;
    void initConfigurationView_S_L_Connection();
    void initDeepGCfgView_S_L_Connection();
    void initReluplexCfgView_S_L_Connection();
    void addTool(QString tool,Project *p);
    void removeTool(QString tool);    

signals:
    void SIGNAL_run();
    void SIGNAL_run_with_augs(const QString inputfilename,const QString network,const QString robustness,const double delta);
    void SIGNAL_init_Batch();
    void SIGNAL_over_Batch();
    void SIGNAL_showstatus(const QString msg);
    void SIGNAL_importNetwork();
    void SIGNAL_importInput();
    void SIGNAL_programout(const QString out);
    void SIGNAL_run_with_input(const QString inputfilename);
    void SIGNAL_run_with_input_deepgcfg(const QString inputfilename);
    void SIGNAL_addNetworkGraph(const QString name, const QString layertype, int layernumm, int layerwhich);
    void SIGNAL_Run_DeepG(const QString binpath,const QString configdir);
    void SIGNAL_Run_ERAN(const QString network,const QString dataset,const QString configdir,int paramnum,int inputnum);
    void SIGNAL_Run_ERAN_Raw(const QString inputfile,const QString network,const double epsilon);
    void SIGNAL_Run_ERAN_Raw_Batch(const QString inputfolder,const QString network,const double epsilon);
    void SIGNAL_run_reluplex(int timeout,bool withDeepsymbol,const QString parameters);
    void SIGNAL_run_planet(const QString args);


};

#endif // SETTINGVIEW_H
