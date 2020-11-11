#include "selectdialog.h"
#include "ui_selectdialog.h"
#include <QStringListModel>

SelectDialog::SelectDialog(QString name, QStringList * items,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDialog)
{
    ui->setupUi(this);

    QStringListModel *model = new QStringListModel(this);

    model->setStringList(*items);
    this->ui->items->setModel(model);
    this->ui->name->setText(name);
    connect(ui->items, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(choose(const QModelIndex &)));
}

SelectDialog::~SelectDialog()
{
    delete ui;
}

QString SelectDialog::getResult()
{
    return this->result;
}


void SelectDialog::choose(const QModelIndex & index)
{
    this->result = ui->items->model()->data(index).toString();
    this->accept();

}
void SelectDialog::on_button_accepted()
{
    QModelIndex  index;
    index=ui->items->currentIndex(); //获取当前 modelIndex
    this->result = ui->items->model()->data(index).toString();
}
