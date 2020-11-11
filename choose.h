#ifndef CHOOSE_H
#define CHOOSE_H

#include <QDialog>

namespace Ui {
class Choose;
}

class Choose : public QDialog
{
    Q_OBJECT

public:
    explicit Choose(QString title, bool isFile, QWidget *parent = nullptr);
    ~Choose();
    QString getValue();

private slots:
    void on_choose_clicked();

    void on_result_accepted();

    void on_result_rejected();


private:
    Ui::Choose *ui;
    QString value = "";

    QString title;
    bool isFile;
};

#endif // CHOOSE_H
