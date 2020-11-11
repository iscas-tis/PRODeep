#ifndef RELUPLEXCFG_H
#define RELUPLEXCFG_H

#include <QWidget>
#include "project.h"

namespace Ui {
class reluplexcfg;
}

class reluplexcfg : public QWidget
{
    Q_OBJECT

public:
    explicit reluplexcfg(Project *project,QWidget *parent = nullptr);
    ~reluplexcfg();

signals:
    void SIGNAL_run_reluplex(int timeout,bool withDeepsymbol,const QString parameters);

private slots:
    void on_reluplexNetworkButton_clicked();
    void changeReluplexSpinValue(int value);
    void changeReluplexSliderValue(double value);

    void on_runWithoutSymbol_clicked();

    void on_runWithSymbol_clicked();

private:
    Ui::reluplexcfg *ui;
    Project *project;
};

#endif // RELUPLEXCFG_H
