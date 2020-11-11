#include "outview.h"
#include "ui_outview.h"
#include <QScrollBar>

OutView::OutView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutView)
{
    ui->setupUi(this);    
}


void OutView::clear()
{
    this->ui->output->setText("");
    this->ui->output_Program->setText("");
//    this->ui->tabWidget->setCurrentWidget(this->ui->programOut);
    this->ui->tabWidget->setCurrentWidget(this->ui->applicationOut);
}

void OutView::setText(QString content)
{
    this->ui->output->setText(content);
    QScrollBar *scrollbar = this->ui->output->verticalScrollBar();
    if(scrollbar){
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

void OutView::setProgramText(const QString out)
{
    this->ui->output_Program->setText(out);
    QScrollBar *scrollbar = this->ui->output_Program->verticalScrollBar();
    if(scrollbar){
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

void OutView::appendProgramText(const QString out)
{
    this->ui->output_Program->insertPlainText(out);
    QScrollBar *scrollbar = this->ui->output_Program->verticalScrollBar();
    if(scrollbar){
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}


void OutView::appendText(QString content)
{
    this->ui->output->insertPlainText(content);
    QScrollBar *scrollbar = this->ui->output->verticalScrollBar();
    if(scrollbar){
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

OutView::~OutView()
{
    delete ui;
}
