#ifndef TREENODE_H
#define TREENODE_H

#include <QList>
#include "project.h"
#include "projectitem.h"

#define PROJECT_NODE 0
#define FILE_NODE 1
#define TOOL_NODE 2
#define MIDDLE_NODE 3
class TreeNode
{
public:
    explicit TreeNode(const Project *data,int type, TreeNode *parent = nullptr);
    explicit TreeNode(ProjectItem *data,int type,  TreeNode *parent = nullptr);
    ~TreeNode();

    void appendChild(TreeNode *child);
    void removeChild(int row);
    TreeNode *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    bool insertChildren(int position, int count,  QList<ProjectItem *> pI);
    bool insertColumns(int position,  int columns);
    int row() const;
    TreeNode *parent();
    int getType();
public slots:
    void addModel(const QString & fileName);
private:
    QVector<TreeNode*> m_childItems;
    ProjectItem* m_itemData;
    TreeNode *m_parentItem;
    int type;
};

#endif // TREENODE_H
