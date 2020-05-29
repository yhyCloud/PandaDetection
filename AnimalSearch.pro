#-------------------------------------------------
#
# Project created by QtCreator 2018-07-06T14:21:36
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT += axcontainer
QT += webenginewidgets
QT+=multimedia multimediawidgets
RC_ICONS = tubiao.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AnimalSearch
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    tabwidget.cpp \
    writethread.cpp \
    progressbarthread.cpp \
    mysqlite.cpp \
    adduserdialog.cpp \
    updateuserdialog.cpp \
    logindialog.cpp \
    DetectionThread.cpp \
    bigmapdialog.cpp \
    copyfilethread.cpp \
    VideodetectThread.cpp \
    customslider.cpp \
    DetectionThread_cmd.cpp \
    Videodetectionthread_cmd.cpp


HEADERS += \
        mainwindow.h \
    tabwidget.h \
    progressbarthread.h \
    mysqlite.h \
    adduserdialog.h \
    updateuserdialog.h \
    logindialog.h \
    DetectionThread.h \
    bigmapdialog.h \
    copyfilethread.h \
    writethread.h \
    VideodetectThread.h \
    customslider.h \
    DetectionThread_cmd.h \
    Videodetectionthread_cmd.h


FORMS += \
        mainwindow.ui \
    logindialog.ui \
    adduserdialog.ui \
    updateuserdialog.ui \
    bigmapdialog.ui


RESOURCES += \
    resources.qrc

DISTFILES += \
    icon/001.png \
    icon/002.png \
    icon/003.png \
    icon/004.png \
    icon/005.png \
    icon/006.png \
    icon/007.png \
    icon/008.png \
    icon/009.png \
    icon/010.png \
    icon/011.png \
    icon/012.png \
    icon/013.png \
    icon/014.png \
    icon/015.png \
    icon/016.png \
    icon/017.png \
    icon/018.png \
    icon/019.png \
    icon/020.png \
    icon/021.png \
    icon/022.png \
    icon/023.png \
    icon/024.png \
    icon/025.png \
    icon/026.png \
    icon/027.png \
    icon/028.png \
    icon/029.png \
    icon/030.png \
    icon/031.png \
    icon/032.png \
    icon/033.png \
    icon/034.png \
    icon/035.png \
    icon/036.png \
    icon/037.png \
    icon/038.png \
    icon/039.png \
    icon/040.png \
    icon/041.png \
    icon/042.png \
    icon/043.png \
    icon/044.png \
    icon/045.png \
    icon/046.png \
    icon/047.png \
    icon/048.png \
    icon/049.png \
    icon/050.png \
    icon/051.png \
    icon/052.png \
    icon/053.png \
    icon/054.png \
    icon/055.png \
    icon/056.png \
    icon/057.png \
    icon/058.png \
    icon/059.png \
    icon/060.png \
    icon/061.png \
    icon/062.png \
    icon/063.png \
    icon/064.png \
    icon/065.png \
    icon/066.png \
    icon/067.png \
    icon/068.png \
    icon/069.png \
    icon/070.png \
    icon/071.png \
    icon/072.png \
    icon/073.png \
    icon/074.png \
    icon/075.png \
    icon/076.png \
    icon/077.png \
    icon/078.png \
    icon/079.png \
    icon/080.png \
    icon/081.png \
    icon/082.png \
    icon/083.png \
    icon/084.png \
    icon/085.png \
    icon/086.png \
    icon/087.png \
    icon/088.png \
    icon/089.png \
    icon/090.png \
    icon/091.png \
    icon/092.png \
    icon/093.png \
    icon/094.png \
    icon/095.png \
    icon/096.png \
    icon/097.png \
    icon/098.png \
    icon/099.png \
    icon/100.png \
    icon/046.png \
    icon/地图信息.png \
    icon/视频1.png \
    icon/视频2.png \
    icon/视频检索.png \
    icon/图像1.png \
    icon/图像3.png \
    icon/图像4.png \
    icon/图像5.png \
    icon/图像检索.png



win32: LIBS += -LD:/Anaconda3/libs/ -lpython36

INCLUDEPATH += D:/Anaconda3/include
DEPENDPATH += D:/Anaconda3/include

win32:!win32-g++: PRE_TARGETDEPS += D:/Anaconda3/libs/python36.lib
else:win32-g++: PRE_TARGETDEPS += D:/Anaconda3/libs/libpython36.a
