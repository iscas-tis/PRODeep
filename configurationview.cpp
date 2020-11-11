#include "configurationview.h"
#include "ui_configurationview.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QtMath>
ConfigurationView::ConfigurationView(Project * pro, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigurationView)
{
    ui->setupUi(this);
    if(pro == nullptr)
    {
        qDebug() << "project is null";
        return;
    }
    this->project = pro;
    this->changeNetworkFile(this->project->getPath(),this->project->getNetworkFileName());
    this->changeInputFile(this->project->getPath(),this->project->getInputFileName());



    //spin and slide
    ui->deltaSpin->setRange(0,1);
    ui->deltaSlider->setRange(0,(int)pow(10,UI_DELTA_PRECISION));
    ui->deltaSpin->setSingleStep(pow(0.1,UI_DELTA_PRECISION));
    ui->deltaSpin->setDecimals(UI_DELTA_PRECISION);
    connect(ui->deltaSlider, SIGNAL(valueChanged(int)), this, SLOT(changeSpinboxValue(int)));
    connect(ui->deltaSpin, SIGNAL(valueChanged(double)), this, SLOT(changeSlideValue(double)));


    //epsilon spin and slide
    ui->epsilonSpin->setRange(0,1);
    ui->epsilonSlider->setRange(0,(int)pow(10,UI_EPSILON_PRECISION));
    ui->epsilonSpin->setSingleStep(pow(0.1,UI_EPSILON_PRECISION));
    ui->epsilonSpin->setDecimals(UI_EPSILON_PRECISION);
    connect(ui->epsilonSlider, SIGNAL(valueChanged(int)), this, SLOT(changeEpsilonSpinboxValue(int)));
    connect(ui->epsilonSpin, SIGNAL(valueChanged(double)), this, SLOT(changeEpsilonSlideValue(double)));

    //delta and unsmoothness about PLANET
    ui->PLANETDeltaSpin->setRange(0,1);
    ui->PLANETDeltaSlider->setRange(0,(int)pow(10,UI_DELTA_PRECISION));
    ui->PLANETDeltaSpin->setSingleStep(pow(0.1,UI_DELTA_PRECISION));
    ui->PLANETDeltaSpin->setDecimals(UI_DELTA_PRECISION);
    connect(ui->PLANETDeltaSlider, SIGNAL(valueChanged(int)), this, SLOT(changeDeltaSpinboxValue(int)));
    connect(ui->PLANETDeltaSpin, SIGNAL(valueChanged(double)), this, SLOT(changeDeltaSlideValue(double)));

    ui->PLANETUnsmoothnessSpin->setRange(0,1);
    ui->PLANETUnsmoothnessSlider->setRange(0,(int)pow(10,UI_UNSMOOTHNESS_PRECISION));
    ui->PLANETUnsmoothnessSpin->setSingleStep(pow(0.1,UI_UNSMOOTHNESS_PRECISION));
    ui->PLANETUnsmoothnessSpin->setDecimals(UI_UNSMOOTHNESS_PRECISION);
    connect(ui->PLANETUnsmoothnessSlider, SIGNAL(valueChanged(int)), this, SLOT(changeUnsmoothnessSpinboxValue(int)));
    connect(ui->PLANETUnsmoothnessSpin, SIGNAL(valueChanged(double)), this, SLOT(changeUnsmoothnessSlideValue(double)));


    connect(this,SIGNAL(batch_run()),this,SLOT(on_batch_run()));

}

void ConfigurationView::importNetwork()
{
    emit SIGNAL_importNetwork();
}

void ConfigurationView::importInputFile()
{
    emit SIGNAL_importInput();
}

ConfigurationView::~ConfigurationView()
{
    delete ui;
}


QString ConfigurationView::getRobustnessType()
{
    return this->roubustnessType;
}

double ConfigurationView::getDelta()
{
    return this->ui->deltaSpin->value();
}

double ConfigurationView::getEpsilon()
{
    return this->ui->epsilonSpin->value();
}

void ConfigurationView::changeSlideValue(double value)
{
    ui->deltaSlider->setValue((int)(value * pow(10,UI_DELTA_PRECISION)));
}
void ConfigurationView::changeSpinboxValue(int value)
{
    ui->deltaSpin->setValue(value*pow(0.1,UI_DELTA_PRECISION));
}

void ConfigurationView::changeDeltaSlideValue(double value)
{
    ui->PLANETDeltaSlider->setValue((int)(value * pow(10,UI_DELTA_PRECISION)));
}
void ConfigurationView::changeDeltaSpinboxValue(int value)
{
    ui->PLANETDeltaSpin->setValue(value*pow(0.1,UI_DELTA_PRECISION));
}

void ConfigurationView::changeUnsmoothnessSlideValue(double value)
{
    ui->PLANETUnsmoothnessSlider->setValue((int)(value * pow(10,UI_UNSMOOTHNESS_PRECISION)));
}
void ConfigurationView::changeUnsmoothnessSpinboxValue(int value)
{
    ui->PLANETUnsmoothnessSpin->setValue(value*pow(0.1,UI_UNSMOOTHNESS_PRECISION));
}

void ConfigurationView::changeEpsilonSlideValue(double value)
{
    ui->epsilonSlider->setValue((int)(value * pow(10,UI_EPSILON_PRECISION)));
}
void ConfigurationView::changeEpsilonSpinboxValue(int value)
{
    ui->epsilonSpin->setValue(value*pow(0.1,UI_EPSILON_PRECISION));
}


void ConfigurationView::changeNetworkFile(QString path, QString fileName)
{
    QDir dir(path);
    this->ui->networkFile->setText(dir.absoluteFilePath(fileName));
    this->ui->networkFile->setCursorPosition(0);    
}
void ConfigurationView::changeInputFile(QString path, QString fileName)
{
    QDir dir(path);
    this->ui->inputFile->setText(dir.absoluteFilePath(fileName));
    this->ui->inputFile->setCursorPosition(0);
    this->ui->batchFolder->setText(path);
}

void ConfigurationView::on_importNetwork_clicked()
{
    this->importNetwork();
}


void ConfigurationView::on_chooseInput_clicked()
{
    this->importInputFile();
}

void ConfigurationView::on_robustnessType_currentIndexChanged(const QString &arg1)
{
    this->roubustnessType = arg1;
}

void ConfigurationView::on_run_clicked()
{
    if(!this->ui->networkFile->text().endsWith(".rlv"))
    {
        QMessageBox::warning(this,"Warning","Deepsymbol only supports .rlv network file.");
        return;
    }
    if(this->ui->inputFile->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning","No input file.");
        return;
    }
    emit run();
}

void ConfigurationView::on_chooseBatchFolder_clicked()
{
    if(this->project)
    {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Batch Input Directory"),
                                                    this->project->getPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
        this->ui->batchFolder->setText(dir);
    }
}

void ConfigurationView::on_batch_run()
{
    if(!this->queuedFiles.empty())
    {
        //attention: linux filesystem "/"
        emit SIGNAL_run_with_input(this->ui->batchFolder->text()+"/"+this->queuedFiles.back());
        this->queuedFiles.pop_back();
    }
    else
    {
//        this->ui->batchRun->setEnabled(true);
//        this->ui->run->setEnabled(true);
//        this->ui->deltaSpin->setEnabled(true);
//        this->ui->deltaSlider->setEnabled(true);
//        this->ui->batchRun->setEnabled(true);
//        this->ui->importNetwork->setEnabled(true);
//        this->ui->chooseInput->setEnabled(true);
//        this->ui->robustnessType->setEnabled(true);
//        this->ui->chooseBatchFolder->setEnabled(true);
        emit overBatch();
    }
}

void ConfigurationView::on_batchRun_clicked()
{
    if(!this->ui->networkFile->text().endsWith(".rlv"))
    {
        QMessageBox::warning(this,"Warning","Deepsymbol only supports .rlv network file.");
        return;
    }
    if(this->ui->batchFolder->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning","No batch folder.");
        return;
    }
    this->queuedFiles.clear();
    QDir dir(this->ui->batchFolder->text());
    QStringList nameFilters;
    nameFilters << "*.in";
    this->queuedFiles = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    emit initBatch();
//    this->ui->batchRun->setDisabled(false);
//    this->ui->run->setDisabled(true);
//    this->ui->deltaSpin->setDisabled(true);
//    this->ui->deltaSlider->setDisabled(true);
//    this->ui->batchRun->setDisabled(true);
//    this->ui->importNetwork->setDisabled(true);
//    this->ui->chooseInput->setDisabled(true);
//    this->ui->robustnessType->setDisabled(true);
//    this->ui->chooseBatchFolder->setDisabled(true);
    emit batch_run();
    //qDebug()<<this->queuedFiles;
}

void ConfigurationView::on_eranRunButton_clicked()
{
    if(!(this->ui->networkFile->text().endsWith(".tf")||(this->ui->networkFile->text().endsWith(".pyt"))))
    {
        QMessageBox::warning(this,"Warning","ERAN only supports .tf or .pyt network file.");
        return;
    }
    if(this->ui->inputFile->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning","No input file.");
        return;
    }
    emit run_ERAN(this->ui->inputFile->text(),this->ui->networkFile->text(),this->ui->epsilonSpin->value());
}

void ConfigurationView::on_eranBatchRunButton_clicked()
{
    if(!(this->ui->networkFile->text().endsWith(".tf")||(this->ui->networkFile->text().endsWith(".pyt"))))
    {
        QMessageBox::warning(this,"Warning","ERAN only supports .tf or .pyt network file.");
        return;
    }
    if(this->ui->batchFolder->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning","No batch folder.");
        return;
    }
    emit run_ERAN_Batch(this->ui->batchFolder->text(),this->ui->networkFile->text(),this->ui->epsilonSpin->value());
}

void ConfigurationView::on_runPLANETButton_clicked()
{
    if(!this->ui->networkFile->text().endsWith(".rlv"))
    {
        QMessageBox::warning(this,"Warning","PLANET only supports .rlv network file.");
        return;
    }
    if(this->ui->inputFile->text().isEmpty())
    {
        QMessageBox::warning(this,"Warning","No input file.");
        return;
    }
    QString args=" "+this->ui->networkFile->text()+
            " ROBUST "+this->ui->inputFile->text()+
            " "+QString::number(this->ui->PLANETTargetNumberSpin->value())+
            " "+QString::number(this->ui->PLANETDeltaSpin->value())+
            " "+QString::number(this->ui->PLANETUnsmoothnessSpin->value());
    //qDebug()<<args;
    emit SIGNAL_run_planet(args);
}
