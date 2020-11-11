QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_LFLAGS += -no-pie

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backend.cpp \
    choose.cpp \
    comparerelation.cpp \
    configueproject.cpp \
    configurationview.cpp \
    deepgcfg.cpp \
    edge.cpp \
    file.cpp \
    graphwidget.cpp \
    highlighter.cpp \
    loadingwidget.cpp \
    main.cpp \
    mainui.cpp \
    matrix.cpp \
    network.cpp \
    networkfile.cpp \
    networkgraph.cpp \
    networklayer.cpp \
    networklayerdata.cpp \
    node.cpp \
    outview.cpp \
    project.cpp \
    projectitem.cpp \
    projectmodel.cpp \
    projectview.cpp \
    reluplexcfg.cpp \
    resultview.cpp \
    selectdialog.cpp \
    settingview.cpp \
    textedit.cpp \
    treenode.cpp \
    updatenetworkthread.cpp \
    util.cpp

HEADERS += \
    backend.h \
    choose.h \
    comparerelation.h \
    configueproject.h \
    configurationview.h \
    deepgcfg.h \
    edge.h \
    file.h \
    graphwidget.h \
    highlighter.h \
    loadingwidget.h \
    mainui.h \
    matrix.h \
    network.h \
    networkfile.h \
    networkgraph.h \
    networklayer.h \
    networklayerdata.h \
    node.h \
    outview.h \
    project.h \
    projectitem.h \
    projectmodel.h \
    projectview.h \
    reluplexcfg.h \
    resultview.h \
    selectdialog.h \
    settingview.h \
    textedit.h \
    treenode.h \
    updatenetworkthread.h \
    util.h

FORMS += \
    choose.ui \
    comparerelation.ui \
    configueproject.ui \
    configurationview.ui \
    deepgcfg.ui \
    loadingwidget.ui \
    mainui.ui \
    matrix.ui \
    network.ui \
    networkgraph.ui \
    networklayer.ui \
    outview.ui \
    projectview.ui \
    reluplexcfg.ui \
    resultview.ui \
    selectdialog.ui \
    settingview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    file.qrc \
    icon.qrc
