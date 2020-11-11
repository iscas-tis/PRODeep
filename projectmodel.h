#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>
#include "treenode.h"
#include "project.h"

class ProjectModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ProjectModel(QObject *parent = nullptr);
    ~ProjectModel() override;

    void addProject(Project * project);
    void closeProject(int row);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;


    int addFile(ProjectItem * file,QString suffix);
    int addTool(ProjectItem * tool);
    int removeTool(ProjectItem * tool);
    int setActiveProject();
    QString getSelectTreeNodeName(const QModelIndex &index) const;
    int getSelectTreeNodeType(const QModelIndex &index) const;
private:
    TreeNode *rootItem;
    int currentProject=-1;
    QList<Project*> projects;

    TreeNode * getItem(const QModelIndex &index) const;
};

#endif // PROJECTMODEL_H
