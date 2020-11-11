#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include <QWidget>
#include "comparerelation.h"
#include "networkgraph.h"
#include <QJsonObject>

namespace Ui {
class ResultView;
}

class ResultView : public QWidget
{
    Q_OBJECT

public:
    explicit ResultView(QWidget *parent = nullptr);
    ~ResultView();
    void parsingJsonFile(QString jsonFile);
    void showCompareRelation();
    void showNetworkGraph();
public slots:
    void removeSubTab(int index);
    void on_addNetworkGraph(const QString name, const QString layertype, int layernum, int layerwhich);
    void showlayer(int layer);

signals:
    void updateNetworkNodeStatus(QJsonObject obj);
    void SIGNAL_showlayer(int layer);
private:
    Ui::ResultView *ui;

    CompareRelation * resultGraph = nullptr;
    NetworkGraph *networkGraph = nullptr;
    QString lastLayer;
};

#endif // RESULTVIEW_H
