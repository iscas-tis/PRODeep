#include "deepgcfg.h"
#include "ui_deepgcfg.h"
#include <QFileDialog>
#include <QtDebug>
#include <QMessageBox>

DeepGcfg::DeepGcfg(Project *project,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeepGcfg)
{
    ui->setupUi(this);
    this->project=project;
}

DeepGcfg::~DeepGcfg()
{
    delete ui;
}

void DeepGcfg::on_ConfigButton_clicked()
{
    if(this->project)
    {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open DeepG Config Directory"),
                                                    this->project->getPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
        if(!dir.isEmpty())this->ui->ConfigLineEdit->setText(dir);
    }
}

void DeepGcfg::on_NetworkButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Choose Network File"), this->project->getPath());
    if(!file.isEmpty())this->ui->NetworkLineEdit->setText(file);
}

void DeepGcfg::on_RunDeepG_clicked()
{
    this->dataset=this->ui->DatasetComboBox->currentText();
    this->inputnum=this->ui->InputNumSpinBox->value();
    this->paramnum=this->ui->ParamNumSpinBox->value();
    this->inputfolder=this->ui->ConfigLineEdit->text();
    if(this->inputfolder.isEmpty())return;
//    this->networkfile=this->ui->NetworkLineEdit->text();
//    if(this->networkfile.isEmpty())return;
    emit SIGNAL_Run_DeepG("~/deepg/code/build/deepg_constraints",this->inputfolder);
}

void DeepGcfg::on_ERANButton_clicked()
{
    this->dataset=this->ui->DatasetComboBox->currentText();
    this->inputnum=this->ui->InputNumSpinBox->value();
    this->paramnum=this->ui->ParamNumSpinBox->value();
    this->inputfolder=this->ui->ConfigLineEdit->text();
    if(this->inputfolder.isEmpty())return;
    this->networkfile=this->ui->NetworkLineEdit->text();
    if(this->networkfile.isEmpty())return;
    emit SIGNAL_Run_ERAN(this->networkfile,this->dataset,this->inputfolder,this->paramnum,this->inputnum);
//    void SIGNAL_Run_ERAN(const QString network,const QString dataset,const QString configdir,int paramnum,int inputnum);
}

//void DeepGcfg::on_runDeepsymbolButton_clicked()
//{
//    this->dataset=this->ui->DatasetComboBox->currentText();
//    this->inputnum=this->ui->InputNumSpinBox->value();
//    this->paramnum=this->ui->ParamNumSpinBox->value();
//    this->inputfolder=this->ui->ConfigLineEdit->text();
//    if(this->inputfolder.isEmpty())return;
//    this->networkfile=this->ui->NetworkLineEdit->text();
//    if(this->networkfile.isEmpty())return;
////    QDir dir(this->ui->ConfigLineEdit->text()+"/Batch_Input_DeepG");
////    if(!dir.exists())
////    {
////        QMessageBox::warning(this,"Warning","Please Run ERAN analyser first");
////    }
//    QFileInfo info(this->ui->ConfigLineEdit->text()+"/Batch_Input_DeepG/in_"+QString::number(this->inputnum-1)+".in");
//    if(!info.exists())
//    {
//        QMessageBox::warning(this,"Warning","DeepG and ERAN needs to be run first.");
//        return;
//    }
//    if(!this->networkfile.endsWith(".rlv"))
//    {
//        QMessageBox::warning(this,"Warning","DeepSymbol needs *.rlv network file.");
//        return;
//    }
//    emit SIGNAL_Run_Deepsymbol(info.absoluteFilePath(),this->networkfile,"CUSTOMBOX",0);
//}

void DeepGcfg::on_runDeepsymbolButton_clicked()
{
    this->dataset=this->ui->DatasetComboBox->currentText();
    this->inputnum=this->ui->InputNumSpinBox->value();
    this->paramnum=this->ui->ParamNumSpinBox->value();
    this->inputfolder=this->ui->ConfigLineEdit->text();
    if(this->inputfolder.isEmpty())return;
    this->networkfile=this->ui->NetworkLineEdit->text();
    if(this->networkfile.isEmpty())return;

    this->queuedFiles.clear();
    QDir dir(this->ui->ConfigLineEdit->text()+"/Batch_Input_DeepG");
    if(!dir.exists())
    {
        QMessageBox::warning(this,"Warning","DeepG and ERAN needs to be run first.");
        return;
    }
    this->queuedDir=this->ui->ConfigLineEdit->text()+"/Batch_Input_DeepG";
    if(!this->networkfile.endsWith(".rlv"))
    {
        QMessageBox::warning(this,"Warning","DeepSymbol needs *.rlv network file.");
        return;
    }
    QStringList nameFilters;
    nameFilters << "*.in";
    this->queuedFiles = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    //qDebug()<<this->queuedFiles;
    emit SIGNAL_init_batch();
    this->batch_run();
}

void DeepGcfg::batch_run()
{
    if(!this->queuedFiles.empty())
    {
        //attention: linux filesystem "/"
        emit SIGNAL_run_with_input(this->queuedDir+"/"+this->queuedFiles.back());
        this->queuedFiles.pop_back();
    }
    else
    {
        emit SIGNAL_over_batch();
    }
}
