#include "treenode.h"


TreeNode::TreeNode(const Project *data,int type, TreeNode *parent)
    : m_parentItem(parent)
{
    ProjectItem * item = new ProjectItem();
    for(int i=0;i< ELEMENT_NUMBER;i++)
    {
        item->append(data->get(i));
    }
    m_itemData = item;
    this->type = type;

}
TreeNode::TreeNode(ProjectItem *data,int type, TreeNode *parent)
    :m_itemData(data), m_parentItem(parent)
{
    for(int i=m_itemData->count();i<ELEMENT_NUMBER;i++){
        m_itemData->append("");
    }
    this->type = type;
}

TreeNode::~TreeNode()
{
    qDeleteAll(m_childItems);
}
int TreeNode::getType()
{
    return this->type;
}
void TreeNode::appendChild(TreeNode *item)
{
    m_childItems.append(item);
}

void TreeNode::removeChild(int row)
{
    if(row<0 || row>=m_childItems.size()) return;
    m_childItems.remove(row);
}

bool TreeNode::insertChildren(int position, int count, QList<ProjectItem *> pI)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        TreeNode *item = new TreeNode(pI.at(row),PROJECT_NODE, this);
        m_childItems.insert(position, item);
    }

    return true;
}

bool TreeNode::insertColumns(int position, int columns)
{
    if (position < 0 || position > m_itemData->count())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData->insert(position, QVariant());

    for (TreeNode *child : qAsConst(m_childItems))
        child->insertColumns(position, columns);

    return true;
}

bool TreeNode::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

bool TreeNode::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_itemData->count())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData->remove(position);

    for (TreeNode *child : qAsConst(m_childItems))
        child->removeColumns(position, columns);

    return true;
}


TreeNode *TreeNode::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int TreeNode::childCount() const
{
    return m_childItems.count();
}

int TreeNode::columnCount() const
{
    return m_itemData->count();
}

QVariant TreeNode::data(int column) const
{
    return m_itemData->get(column);
}

TreeNode *TreeNode::parent()
{
    return m_parentItem;
}

int TreeNode::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeNode*>(this));

    return 0;
}

