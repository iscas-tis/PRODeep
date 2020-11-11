#include "projectitem.h"

ProjectItem::ProjectItem()
{

}
ProjectItem::ProjectItem(QList<QVariant> column)
{
    this->column->append(column);
}
QVariant ProjectItem::get(int column) const
{
    return this->column->at(column);
}
int ProjectItem::count() const
{
    return this->column->size();
}

void ProjectItem::append(QVariant element){
    this->column->append(element);
}

void ProjectItem::insert(int index, QVariant element)
{
    this->column->insert(index,element);
}
void ProjectItem::remove(int index)
{
    if(index <0 || index >= this->column->size()) return;
    this->column->removeAt(index);
}
