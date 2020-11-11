#include "mainui.h"

#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDir tmpPath = QDir::tempPath();


    if(!tmpPath.cd("Qt")){
        tmpPath.mkdir("Qt");
        tmpPath.cd("Qt");
    }
    if(!tmpPath.cd("lib")){
        tmpPath.mkdir("lib");
        tmpPath.cd("lib");
    }
     QFileInfo info(tmpPath.absoluteFilePath("deepsymbol"));
     if(!info.isFile()){
         bool copyResult = false;

#if defined(Q_OS_LINUX)
        copyResult= QFile::copy(":/resources/deepsymbol_linux",info.absoluteFilePath());
#elif defined(Q_OS_MAC)
         copyResult= QFile::copy(":/resources/deepsymbol_mac",info.absoluteFilePath());
#elif defined(Q_OS_WIN32)
         copyResult= QFile::copy(":/resources/deepsymbol_windows",info.absoluteFilePath());
#else
        qDebug() << "Unsupported arctecture";
        return -7;
#endif
        if(!copyResult)
        {
            //QMessageBox::warning(this, "Worning", "Failed to access TemporaryFile!");
            qDebug() << "Failed to access TemporaryFile!";
            return -7;
        }
     }
    /*
    QDir dir(qApp->applicationDirPath());
    if(!dir.cd("lib")){
        QMessageBox::warning(&w, "Worning", "does not have \".\\lib\" directory!");
        return -7;
    }
    QFileInfo info(dir.absoluteFilePath("epmc-qmc.jar"));
    if(!info.isFile()){
        QMessageBox::warning(&w, "Worning", "lack of \"epmc-qmc.jar\" in \"\\lib\"!");
        return -7;
    }*/

    MainUI w;
    w.showMaximized();
    return a.exec();
}
