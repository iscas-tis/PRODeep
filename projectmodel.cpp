#include "projectmodel.h"
#include<QDebug>
#include<QDir>

ProjectModel::ProjectModel( QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeNode(new Project("Project"), PROJECT_NODE);
    //setupModelData(data.split('\n'), rootItem);
}
 ProjectModel::~ProjectModel()
{

}

void ProjectModel::addProject(Project * project)
{
    this->beginInsertRows(QModelIndex(),rootItem->childCount(),rootItem->childCount());
    TreeNode * node = new TreeNode(project,PROJECT_NODE,rootItem);
    rootItem->appendChild(node);
    this->endInsertRows();

    this->beginInsertRows(this->index(this->currentProject,0),0,1);
    node->appendChild(new TreeNode(new ProjectItem({"Files"}),MIDDLE_NODE,node));
    node->appendChild(new TreeNode(new ProjectItem({"Tools"}),MIDDLE_NODE,node));
    this->endInsertRows();

    this->projects.append(project);
    this->currentProject = this->projects.size()-1;
}
void ProjectModel::closeProject(int row)
{
    this->beginRemoveRows(QModelIndex(),row,row);
    rootItem->removeChild(row);
    this->endRemoveRows();
    this->projects.removeAt(row);
    this->currentProject = this->projects.size()-1;
}
int ProjectModel::addFile(ProjectItem * file,QString suffix){
    if(this->currentProject == -1) return -1;
    TreeNode * node = rootItem->child(this->currentProject)->child(0);
    QModelIndex node_index = this->index(0,0,this->index(this->currentProject,0));
//    if(node->childCount() >0){
//        this->beginRemoveRows(this->index(0,0,node_index),0,0);
//        node->removeChild(this->currentProject);
//        this->endRemoveRows();
//    }
    for(int i=0;i<node->childCount();i++)
    {
        TreeNode *child = node->child(i);
        if(child->data(0).toString().endsWith(suffix))
        {
            this->beginRemoveRows(node_index,i,i);
            node->removeChild(i);
            this->endRemoveRows();
            break;
        }
    }

    this->beginInsertRows(node_index,node->childCount(),node->childCount());
    node->appendChild(new TreeNode(file,FILE_NODE,node));
    this->endInsertRows();
    return 0;
}

int ProjectModel::addTool(ProjectItem * tool){
    if(this->currentProject == -1) return -1;
    TreeNode * node = rootItem->child(this->currentProject)->child(1);
    QModelIndex node_index = this->index(1,0,this->index(this->currentProject,0));
//    if(node->childCount() >0) {
//        this->beginRemoveRows(this->index(0,0,node_index),0,0);
//        node->removeChild(this->currentProject);
//        this->endRemoveRows();
//    }
    for(int i=0;i<node->childCount();i++)
    {
        TreeNode *child = node->child(i);
        if(child->data(1).toString().compare(tool->get(1).toString()) == 0)
        {
            this->beginRemoveRows(node_index,i,i);
            node->removeChild(i);
            this->endRemoveRows();
            break;
        }
    }
    this->beginInsertRows(node_index,node->childCount(),node->childCount());
    node->appendChild(new TreeNode(tool,TOOL_NODE,node));
    this->endInsertRows();
    return 0;
}

int ProjectModel::removeTool(ProjectItem * tool)
{
    if(this->currentProject == -1) return -1;
    TreeNode * node = rootItem->child(this->currentProject)->child(1);
    QModelIndex node_index = this->index(1,0,this->index(this->currentProject,0));
    for(int i=0;i<node->childCount();i++)
    {
        if(node->child(i)->data(1) == tool->get(1))
        {
            this->beginRemoveRows(node_index,i,i);
            node->removeChild(i);
            this->endRemoveRows();
            break;
        }
    }
    return 0;
}

TreeNode *ProjectModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}
QString ProjectModel::getSelectTreeNodeName(const QModelIndex &index) const
{
    TreeNode * node = this->getItem(index);
    return node->data(0).toString();

}

int ProjectModel::getSelectTreeNodeType(const QModelIndex &index) const
{
    TreeNode * node = this->getItem(index);
    return node->getType();
}


QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeNode*>(parent.internalPointer());

    TreeNode *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex ProjectModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNode *childItem = static_cast<TreeNode*>(index.internalPointer());
    TreeNode *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectModel::rowCount(const QModelIndex &parent) const
{
    TreeNode *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeNode*>(parent.internalPointer());

    return parentItem->childCount();
}
Qt::ItemFlags ProjectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}
int ProjectModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeNode*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
    // FIXME: Implement me!
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());

    return item->data(index.column());
}

