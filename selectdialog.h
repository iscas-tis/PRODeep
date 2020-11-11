#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QDialog>

namespace Ui {
class SelectDialog;
}

class SelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDialog(QString name, QStringList * items,QWidget *parent = nullptr);
    ~SelectDialog();
    QString getResult();

private slots:
    void on_button_accepted();
    void choose(const QModelIndex & index);

private:
    Ui::SelectDialog *ui;
    QString result = "";
};

#endif // SELECTDIALOG_H
