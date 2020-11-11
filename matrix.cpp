#include "matrix.h"
#include "ui_matrix.h"
#include <QDebug>
#include <QScrollBar>
#include <QToolTip>

Matrix::Matrix(const QString name, QStringList * rowHeadName,QStringList * columnHeadName, bool isSetDefaultHeadName,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Matrix)
{
    ui->setupUi(this);
    this->name = name;
    if(rowHeadName != nullptr)
    {
        this->rowNumber = rowHeadName->size();
        model->setRowCount(this->rowNumber);
        for(int i=0;i<this->rowNumber;i++)
        {
            QString headName = rowHeadName->at(i);
            model->setHeaderData(i,Qt::Vertical,headName);
            this->rowHeadName->append(headName);
        }
    }
    else
    {
        model->setRowCount(this->rowNumber);
        if(isSetDefaultHeadName)
        {
            model->setHeaderData(0,Qt::Vertical,"inf");
            model->setHeaderData(1,Qt::Vertical,"sup");
        }

    }
    if(columnHeadName != nullptr)
    {
        this->columnNumber = columnHeadName->size();
        model->setColumnCount(this->columnNumber);
        for(int i=0;i<this->columnNumber;i++)
        {
            QString headName = columnHeadName->at(i);
            model->setHeaderData(i,Qt::Horizontal,headName);
            this->columnHeadName->append(headName);
        }
    }
    else
    {
       model->setColumnCount(this->columnNumber);
       if(isSetDefaultHeadName){
           model->setHeaderData(0,Qt::Horizontal,"inf");
           model->setHeaderData(1,Qt::Horizontal,"sup");
       }
       else
       {
           model->setColumnCount(1); //Bias has only 1 column
           this->columnNumber = 1;
       }
    }
    ui->matrix->setModel(this->model);

    this->ui->matrixName->setText(this->name);
    this->ui->matrixDimention->setText(QString::number(this->rowNumber)+"*"+ QString::number(this->columnNumber));

    //ui->matrix->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->matrix->setWordWrap(false);
    ui->matrix->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->matrix->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->matrix->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->matrix->verticalScrollBar()->installEventFilter(this);
    ui->matrix->horizontalScrollBar()->installEventFilter(this);

    ui->matrix->viewport()->installEventFilter(this);//安装事件过滤器
    ui->matrix->viewport()->setMouseTracking(true);//true鼠标移动触发 false关闭移动触发
}


bool Matrix::isSetRowHeadName()
{
    return this->rowHeadName->size()>0;
}
bool Matrix::isSetColumnHeadName()
{
    return this->columnHeadName->size()>0;
}

int Matrix::indexOfRow(QString rowHeadName)
{
    return this->rowHeadName->indexOf(rowHeadName);
}
int Matrix::indexOfColumn(QString columnHeadName)
{
    return this->columnHeadName->indexOf(columnHeadName);
}

void Matrix::setEditable(bool flag)
{
    if(flag){
        this->ui->matrix->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::AnyKeyPressed);
    }
    else{
        this->ui->matrix->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

QString Matrix::getData(int row,int column)
{

    return this->model->item(row,column)->text();
}

int Matrix::getRowNumber()
{
    return this->model->rowCount();
}
int Matrix::getColumnNumber()
{
    return this->model->columnCount();
}


QStringList * Matrix::getRowHeadNames()
{
    return this->rowHeadName;
}
QStringList * Matrix::getColumnHeadNames()
{
    return this->columnHeadName;
}

void Matrix::setItemColor(int row, int column, QColor color)
{
    this->model->item(row,column)->setBackground(QBrush(color));
}

int Matrix::setName(QString name)
{
    this->name = name;
    ui->matrixName->setText(name);
    return  0;
}
int Matrix::setDescription(QString description)
{
    ui->matrixDimention->setText(description);
    return 0;
}
int Matrix::setData(QString ** vecs, int rowNumber, int columnNumber)
{
    for(int i=0;i<rowNumber;i++)
    {
        for(int j=0;j<columnNumber;j++){
            this->insertItem(i,j,vecs[i][j]);
        }
    }
    //ui->matrix->resizeColumnsToContents();
    return 0;
}
int Matrix::insertItem(int row,int column, QString value)
{
    QStandardItem *item = new QStandardItem(value);
    model->setItem(row,column,item);    
    return 0;
}

bool Matrix::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->matrix->verticalScrollBar() || obj == ui->matrix->horizontalScrollBar())
    {
        if(event->type() == QEvent::Wheel)
        {

            return true;
        }
    }
    else if(obj == ui->matrix->viewport())
        {
            if(event->type() == QEvent::MouseMove)
            {
                QPoint p = ui->matrix->viewport()->mapFromGlobal(QCursor::pos());
                choseItem(ui->matrix->indexAt(p));
            }
        }
    return QWidget::eventFilter(obj, event);
}
void Matrix::choseItem(QModelIndex index)
{
    if(!index.isValid())
        return;
    QString text = QString("%1").arg(index.data().toString());
    if(text.isEmpty())
        return;
    QToolTip::showText(QCursor::pos(),text);
}
Matrix::~Matrix()
{
    delete ui;
}

void Matrix::on_matrix_doubleClicked(const QModelIndex &index)
{
    emit onValueChanged();
}
