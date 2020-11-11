#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include<QVariant>
#include<QList>

class ProjectItem
{
private:
    QList<QVariant> * column = new QList<QVariant>();
public:
    ProjectItem();
    ProjectItem(QList<QVariant> colum);
    QVariant get(int column) const;
    int count() const;

    void append( QVariant element);
    void insert(int index, QVariant element);
    void remove(int index);
};

#endif // PROJECTITEM_H
