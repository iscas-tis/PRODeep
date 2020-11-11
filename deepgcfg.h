#ifndef DEEPGCFG_H
#define DEEPGCFG_H

#include <QWidget>
#include "project.h"

namespace Ui {
class DeepGcfg;
}

class DeepGcfg : public QWidget
{
    Q_OBJECT

public:
    explicit DeepGcfg(Project *project,QWidget *parent = nullptr);
    void batch_run();
    ~DeepGcfg();

signals:
    void SIGNAL_Run_DeepG(const QString binpath,const QString configdir);
    void SIGNAL_Run_ERAN(const QString network,const QString dataset,const QString configdir,int paramnum,int inputnum);
    void SIGNAL_Run_Deepsymbol(const QString inputfilename,const QString network,const QString robustness,const double delta);
    void SIGNAL_run_with_input(const QString inputfilename);
    void SIGNAL_init_batch();
    void SIGNAL_over_batch();

private slots:
    void on_ConfigButton_clicked();

    void on_NetworkButton_clicked();

    void on_RunDeepG_clicked();

    void on_ERANButton_clicked();    

    void on_runDeepsymbolButton_clicked();

private:
    Ui::DeepGcfg *ui;
    Project *project;

    QString dataset,inputfolder,networkfile,queuedDir;
    QStringList queuedFiles;
    int paramnum,inputnum;
};

#endif // DEEPGCFG_H
