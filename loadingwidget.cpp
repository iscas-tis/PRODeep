#include "loadingwidget.h"
#include "ui_loadingwidget.h"

LoadingWidget::LoadingWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);//无边框
    setAttribute(Qt::WA_TranslucentBackground);//背景透明

    //屏幕居中显示
    int frmX = width();
    int frmY = height();

    QDesktopWidget w;
    int deskWidth = w.width();
    int deskHeight = w.height();

    QPoint movePoint(deskWidth / 2 - frmX / 2, deskHeight / 2 - frmY / 2);
    move(movePoint);

    //加载gif图片
    QMovie *movie = new QMovie("loading");//new QMovie(":/new/pic/resources/loading");
    ui->label->setMovie(movie);
    movie->start();
}

LoadingWidget::~LoadingWidget()
{
    delete ui;
}
