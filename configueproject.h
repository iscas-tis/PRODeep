#ifndef CONFIGUEPROJECT_H
#define CONFIGUEPROJECT_H

#include <QWidget>

namespace Ui {
class ConfigueProject;
}

class ConfigueProject : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigueProject(QWidget *parent = nullptr);
    ~ConfigueProject();

private:
    Ui::ConfigueProject *ui;
};

#endif // CONFIGUEPROJECT_H
