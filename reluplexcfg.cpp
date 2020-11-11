#include "reluplexcfg.h"
#include "ui_reluplexcfg.h"
#include <QFileDialog>
#include <QDebug>

reluplexcfg::reluplexcfg(Project *project,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::reluplexcfg)
{
    ui->setupUi(this);
    this->project=project;

    connect(ui->reluplexSlider, SIGNAL(valueChanged(int)), this, SLOT(changeReluplexSpinValue(int)));
    connect(ui->reluplexSpin, SIGNAL(valueChanged(double)), this, SLOT(changeReluplexSliderValue(double)));
}

reluplexcfg::~reluplexcfg()
{
    delete ui;
}

void reluplexcfg::on_reluplexNetworkButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Choose Network File"), this->project->getPath());
    if(!file.isEmpty())this->ui->reluplexNetwork->setText(file);
}

void reluplexcfg::changeReluplexSpinValue(int value)
{
    this->ui->reluplexSpin->setValue(double(value)/1000);
}

void reluplexcfg::changeReluplexSliderValue(double value)
{
    this->ui->reluplexSlider->setValue(int(value*1000));
}

void reluplexcfg::on_runWithoutSymbol_clicked()
{
    if(this->ui->reluplexNetwork->text().isEmpty())return;
    QString parameters=" /tmp/Qt/lib/adversarial_summary.txt "+this->ui->reluplexNetwork->text()+" "+QString::number(this->ui->testpointSpin->value())+" "+QString::number(this->ui->reluplexSpin->value())+" ";
    //qDebug()<<QString::number(this->ui->timeoutSpin->value())<<parameters;
    emit SIGNAL_run_reluplex(this->ui->timeoutSpin->value(),false,parameters);
}


void reluplexcfg::on_runWithSymbol_clicked()
{
    if(this->ui->reluplexNetwork->text().isEmpty())return;
    QString parameters=" /tmp/Qt/lib/adversarial_summary.txt "+this->ui->reluplexNetwork->text()+" "+QString::number(this->ui->testpointSpin->value())+" "+QString::number(this->ui->reluplexSpin->value())+" ";
    //qDebug()<<QString::number(this->ui->timeoutSpin->value())<<parameters;
    emit SIGNAL_run_reluplex(this->ui->timeoutSpin->value(),true,parameters);
}
