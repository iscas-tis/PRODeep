#include "plugin.h"
#include "ui_plugin.h"

Plugin::Plugin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plugin)
{
    ui->setupUi(this);
}

Plugin::~Plugin()
{
    delete ui;
}
