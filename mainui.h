#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>
#include <QSplitter>

#include "settingview.h"
#include "projectview.h"
#include "outview.h"
#include "resultview.h"
#include "backend.h"

#include <QJsonObject>
#include <QLabel>


#define JSON_RESULT_FILE "VerifyResult.json"

#define PYTHON_EXECUTABLE "/home/demo/miniconda3/bin/python"
#define DEEPG_ROOT "/home/demo/Library/deepg"
//#define DEEPG_ERAN_BACKEND "/home/demo/miniconda3/bin/python deepg.py"
#define DEEPG_BACKEND "./build/deepg_constraints"
#define DEEPG_ERAN_BACKEND (std::string(PYTHON_EXECUTABLE) + std::string(" deepg.py")).c_str()
#define ERAN_BACKEND (std::string(PYTHON_EXECUTABLE) + std::string(" . ")).c_str()
//#define ERAN_BACKEND "/home/demo/miniconda3/bin/python . "
#define DEEPG_DIR (std::string(DEEPG_ROOT) + std::string("/code")).c_str()
#define DEEPG_ERAN_DIR (std::string(DEEPG_ROOT) + std::string("/ERAN/tf_verify")).c_str()

#define RELUPLEX_BACKEND "/home/demo/Library/reluplex/ReluplexCav2017-master/check_properties/adversarial/adversarialPure.elf"
#define RELUPLEX_WITH_DEEPSYMBOL_BACKEND "/home/demo/Library/reluplex/ReluplexCav2017-master/check_properties/adversarial/adversarialBOXSym.elf"

#define PLANET_BACKEND "python2 /home/demo/Library/planet/casestudies/MNIST/prodNetwork.py"
#define PLANET_DIR "/home/demo/Library/planet/casestudies/MNIST"

namespace Ui {
class MainUI;
}

class MainUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = nullptr);
    ~MainUI() ;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_action_New_Project_triggered();

    void on_action_Open_Project_triggered();

    void on_actionClose_Project_triggered();

    void on_readoutput(char * out);

    void on_actionRun_triggered();

    void verifyFinished(int exitCode);
    void batchverifyFinished(int exitCode);

    void on_verify();
    void on_verify_ERAN(const QString inputfile,const QString network,const double epsilon);
    void on_verify_ERAN_Batch(const QString inputfolder,const QString network,const double epsilon);
    void verify_with_input(const QString inputfilename);
    void verify_with_input_deepgcfg(const QString inputfilename);
    void verify_with_augs(const QString inputfilename,const QString network,const QString robustness,const double delta);
    void init_Batch();
    void over_Batch();
    void run_reluplex(int timeout,bool withDeepsymbol,const QString parameters);
    void run_planet(const QString args);

    void on_actionAdd_Tool_triggered();

    void on_actionRemove_Tool_triggered();

    void on_actionImport_Network_triggered();

    void on_actionImport_Input_triggered();

    void on_updateProgramOut(const QString out);

    void on_showStatusBar(const QString msg);
    void on_Run_DeepG(const QString binpath,const QString configdir);
    void on_Run_ERAN(const QString network,const QString dataset,const QString configdir,int paramnum,int inputnum);


private:
    Ui::MainUI * ui;
    SettingView * settingView;
    ProjectView * projectView;
    OutView * outView;
    ResultView * resultView;
    QLabel *statusBarLabel=new QLabel();
    int verifiednum,robustnum,curbatch=0;    


    BackEnd * backend,*deepgbackend,*eranbackend,*eranrawbackend,*reluplexbackend,*planetbackend;

    void initUI();
    void parsingbatchout();

    void Run();
    void Run_ERAN(const QString inputfile,const QString network,const double epsilon);
    QList<QString> filter(QString out);
};

#endif // MAINUI_H
