#ifndef OUTVIEW_H
#define OUTVIEW_H

#include <QWidget>

namespace Ui {
class OutView;
}

class OutView : public QWidget
{
    Q_OBJECT

public:
    explicit OutView(QWidget *parent = nullptr);
    ~OutView();

    void clear();

    void setText(QString content);

    void appendText(QString content);

    void setProgramText(const QString out);

    void appendProgramText(const QString out);

private:
    Ui::OutView *ui;
};

#endif // OUTVIEW_H
