#ifndef MATRIX_H
#define MATRIX_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class Matrix;
}

class Matrix : public QWidget
{
    Q_OBJECT

public:
    explicit Matrix(const QString name, QStringList * rowHeadName = nullptr, QStringList * columnHeadName = nullptr, bool isSetDefaultHeadName=true,QWidget *parent = nullptr);
    ~Matrix();

    int setName(QString name);
    int setDescription(QString description);
    int setData(QString ** vecs, int rowNumber, int columnNumber);
    int insertItem(int row,int column, QString value);
    int insertRow(QString rowName, QString * value = nullptr);
    int insertColumn(QString columnName, QString * value = nullptr);

    void setItemColor(int row,int column,QColor color);

    int getRowNumber();
    int getColumnNumber();

    QStringList * getRowHeadNames();
    QStringList * getColumnHeadNames();

    QString getData(int row,int column);

    void setEditable(bool flag);

    int indexOfRow(QString rowHeadName);
    int indexOfColumn(QString columnHeadName);

    bool isSetRowHeadName();
    bool isSetColumnHeadName();

protected:
    bool eventFilter(QObject *obj, QEvent *event);//事件过滤器
private:
    Ui::Matrix *ui;
    QString name = "matrix_*";
    int rowNumber = 2;
    int columnNumber = 2;
    QStringList * rowHeadName = new QStringList();
    QStringList * columnHeadName = new QStringList();
    QStandardItemModel *model = new QStandardItemModel(this);

    void choseItem(QModelIndex index);

signals:
    void onValueChanged();
private slots:
    void on_matrix_doubleClicked(const QModelIndex &index);
};

#endif // MATRIX_H
