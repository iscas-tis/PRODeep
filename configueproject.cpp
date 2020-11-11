#include "configueproject.h"
#include "ui_configueproject.h"

ConfigueProject::ConfigueProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigueProject)
{
    ui->setupUi(this);
}

ConfigueProject::~ConfigueProject()
{
    delete ui;
}
