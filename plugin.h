#ifndef PLUGIN_H
#define PLUGIN_H

#include <QWidget>

namespace Ui {
class Plugin;
}

class Plugin : public QWidget
{
    Q_OBJECT

public:
    explicit Plugin(QWidget *parent = nullptr);
    ~Plugin();

private:
    Ui::Plugin *ui;
};

#endif // PLUGIN_H
