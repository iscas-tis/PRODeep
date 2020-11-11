#include "choose.h"
#include "ui_choose.h"
#include "QString"
#include "QDir"
#include "QFileDialog"
#include "QDateTime"

Choose::Choose(QString title, bool isFile, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Choose)
{
    ui->setupUi(this);
    this->title = title;
    this->isFile = isFile;
    this->setWindowTitle(this->title);
    if(this->isFile){
        ui->label->setText("Files:");
    }
}

Choose::~Choose()
{
    delete ui;
}

void Choose::on_choose_clicked()
{
    if(this->isFile){
        QStringList fileList = QFileDialog::getOpenFileNames(this,tr("Open File"), QDir::home().path());
        QString file="";
        for(int i=0;i<fileList.size();i++){
            file += fileList.at(i)+";";
        }
        if(file != "")
            ui->text->setText(file);
    }
    else
    {
        //QDir dir;
        QString PathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     QDir::home().path(),
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);
        if(PathName != "")
            ui->text->setText(PathName);    //文件名称显示
        //dir.mkdir(PathName);    //在目录下创建新文件夹
    }
}


void Choose::on_result_accepted()
{
    value = ui->text->text();
    this->accept();
}

void Choose::on_result_rejected()
{
    this->reject();
}
QString Choose::getValue(){
    return this->value;
}
