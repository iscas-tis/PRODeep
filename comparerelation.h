#ifndef COMPARERELATION_H
#define COMPARERELATION_H

#include <QWidget>
#include "graphwidget.h"

namespace Ui {
class CompareRelation;
}

class CompareRelation : public QWidget
{
    Q_OBJECT

public:
    explicit CompareRelation(QWidget *parent = nullptr);
    ~CompareRelation();
    void addNode(QString name);
    void addEdge(QString name1,QString name2);
    void setpos(const QString name,qreal x,qreal y);
    void clear();

private:
    Ui::CompareRelation *ui;
    GraphWidget * graph;
};

#endif // COMPARERELATION_H
