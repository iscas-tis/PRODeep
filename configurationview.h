#ifndef CONFIGURATIONVIEW_H
#define CONFIGURATIONVIEW_H


#define ROBUSTNESS_TYPE_LINFBALL "LINFBALL"
#define ROBUSTNESS_TYPE_BRIGHTNESS "brightness"
#define ROBUSTNESS_TYPE_CUSTOMBOX "CUSTOMBOX"

#define UI_DELTA_PRECISION 3
#define UI_EPSILON_PRECISION 3
#define UI_UNSMOOTHNESS_PRECISION 3

#include <QWidget>
#include "project.h"

namespace Ui {
class ConfigurationView;
}

class ConfigurationView : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigurationView(Project * pro, QWidget *parent = nullptr);
    ~ConfigurationView();

    QString getRobustnessType();
    double getDelta();
    double getEpsilon();

    void changeNetworkFile(QString path, QString fileName);
    void changeInputFile(QString path, QString fileName);

public slots:
    void on_batch_run();

private slots:
    void on_importNetwork_clicked();


    void on_chooseInput_clicked();

    void on_robustnessType_currentIndexChanged(const QString &arg1);

    void changeSlideValue(double value);
    void changeSpinboxValue(int value);

    void changeEpsilonSlideValue(double value);
    void changeEpsilonSpinboxValue(int value);

    void changeDeltaSlideValue(double value);
    void changeDeltaSpinboxValue(int value);

    void changeUnsmoothnessSlideValue(double value);
    void changeUnsmoothnessSpinboxValue(int value);

    void on_run_clicked();    

    void on_chooseBatchFolder_clicked();

    void on_batchRun_clicked();

    void on_eranRunButton_clicked();

    void on_eranBatchRunButton_clicked();

    void on_runPLANETButton_clicked();

private:
    Ui::ConfigurationView *ui;

    Project * project = nullptr;
    QString roubustnessType = ROBUSTNESS_TYPE_LINFBALL;
    QStringList queuedFiles;

    void importNetwork();
    void importInputFile();

signals:
    void run();
    void run_ERAN(const QString inputfile,const QString inputfilename,const double epsilon);
    void run_ERAN_Batch(const QString inputfolder,const QString network,const double epsilon);
    void batch_run();
    void initBatch();
    void overBatch();
    void SIGNAL_importNetwork();
    void SIGNAL_importInput();
    void SIGNAL_run_with_input(const QString inputfilename);
    void SIGNAL_run_planet(const QString args);

};

#endif // CONFIGURATIONVIEW_H
