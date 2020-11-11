#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H

#define CREATE_FILE_SUCCESS 0
#define CREATE_FILE_FAILED -1

#include <QWidget>

#include "projectmodel.h"
#include "project.h"

namespace Ui {
class ProjectView;
}

class ProjectView : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectView(QWidget *parent = nullptr);
    ~ProjectView();

    int openProject();
    int newProject();
    void closeProject();

    bool isValidProject();

    QString getProjectParameters();
    QString getProjectParametersWithInput(const QString inputfilename);

    void addNetwork();
    void addInput();
    void addTool();
    void removeTool();

    QStringList getCandidateTools();
    QStringList getAddedTools();


private slots:

    void treeViewDoubleClick(const QModelIndex & index);
    void on_addNetworkFromProject(QString path,QString fileName);
    void on_addInputFromProject(QString path,QString fileName);
    void on_addToolFromProject(QString tool,QString index,Project * project);
    void on_removeToolFromProject(QString tool,QString index);

    void on_importNetwork();
    void on_importInput();



private:
    Ui::ProjectView *ui;
    ProjectModel * projectModel;
    Project * project = nullptr;
    bool isLoadProject = false;

    void addNetworkInModel(QString path,QString fileName);
    void addInputInModel(QString path,QString fileName);
    void addToolInModel(QString tool,QString index);
    void removeToolInModel(QString tool,QString index);

    inline void initProject_S_L_Connection();

signals:
    void SIGNAL_CloseProject();
    void SIGNAL_OpenProject(Project *p);
    void SIGNAL_networkChanged(QString path, QString fileName);
    void SIGNAL_inputChanged(QString path, QString fileName);
    void SIGNAL_addTool(QString name,Project * project);
    void SIGNAL_removeTool(QString name);
    void SIGNAL_networkChangedTextOnly(QString path,QString filename);

};

#endif // PROJECTVIEW_H
