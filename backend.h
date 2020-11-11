#ifndef BACKEND_H
#define BACKEND_H

#include<QString>
#include<QProcess>

#define RUN_SUCCESS 0
#define BUSY_STATUS -1
#define IS_NOT_FILE -2

class BackEnd: public QObject
{
    Q_OBJECT
private:
    QString excuteFile;
    bool isBusy = false;
    bool isBatch = false;
    QString currentModel;
    QString currentFormule;
    QString currentParametersList;
    QString out;
    QProcess * cmd;    
public:

    BackEnd(const QString excuteFileName);
    bool getIsBusy();
    QString getOutput();
    int setCurrentModel(const QString modelFile);
    int setCurrentFormule(const QString formuleFile);
    void setCurrentParametersList(const QString parametersList);

    int run(bool is_batch=false,const QString workingdir="");

    ~BackEnd();
signals:
    void sendOut(char * text);
    void finished(int exitCode);
    void batchfinished(int exitCode);

private slots:
    void on_readoutput();
    void on_readerror();
    void on_finished(int exitCode, QProcess::ExitStatus exitStatus);

};

#endif // BACKEND_H
