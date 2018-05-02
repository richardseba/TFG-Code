#-------------------------------------------------
#
# Project created by QtCreator 2017-07-12T13:08:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VR_interfaz
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    calibdialog.cpp \
    loadcalibparamsdialog.cpp \
    vrfullscreenviewer.cpp \
    vrimageupdater.cpp \
    selectcameraparamsdialog.cpp \
    vrui.cpp \
    qgraphicstextitemvr.cpp \
    utils.cpp \
    cameracalibration.cpp \
    descriptor.cpp \
    elas.cpp \
    filter.cpp \
    stereocalibration.cpp \
    triangle.cpp \
    matrix.cpp

HEADERS  += mainwindow.h \
    camera.h \
    calibdialog.h \
    loadcalibparamsdialog.h \
    vrfullscreenviewer.h \
    vrimageupdater.h \
    selectcameraparamsdialog.h \
    vrui.h \
    qgraphicstextitemvr.h \
    utils.h \
    cameracalibration.h \
    descriptor.h \
    elas.h \
    filter.h \
    image.h \
    stereocalibration.h \
    timer.h \
    triangle.h \
    matrix.h

FORMS    += mainwindow.ui \
    calibdialog.ui \
    loadcalibparamsdialog.ui \
    selectcameraparamsdialog.ui

RESOURCES += \
    Logos/logosr.qrc

win32 {
    INCLUDEPATH += $$(PYLON_DEV_DIR)/include \
    INCLUDEPATH += C:\opencv\build\include\
}

CONFIG(debug, debug|release) {
    LIBS += C:\opencv\build\x64\vc12\lib\opencv_core2413d.lib \
            C:\opencv\build\x64\vc12\lib\opencv_imgproc2413d.lib \
            C:\opencv\build\x64\vc12\lib\opencv_highgui2413d.lib \
            C:\opencv\build\x64\vc12\lib\opencv_ml2413d.lib \
            C:\opencv\build\x64\vc12\lib\opencv_features2d2413d.lib \
             C:\opencv\build\x64\vc12\lib\opencv_calib3d2413d.lib \
            -L$$(PYLON_DEV_DIR)/lib/x64 \
            -lPylonBase_MD_VC120_v5_0 \
            -lPylonUtility_MD_VC120_v5_0 \
            -lPylonC_MD_VC120 \
    }
    else
    {
    LIBS += C:\opencv\build\x64\vc12\lib\opencv_core2413.lib \
            C:\opencv\build\x64\vc12\lib\opencv_imgproc2413.lib \
            C:\opencv\build\x64\vc12\lib\opencv_highgui2413.lib \
            C:\opencv\build\x64\vc12\lib\opencv_ml2413.lib \
            C:\opencv\build\x64\vc12\lib\opencv_features2d2413.lib\
            C:\opencv\build\x64\vc12\lib\opencv_calib3d2413.lib \
            -L$$(PYLON_DEV_DIR)/lib/x64 \
            -lPylonBase_MD_VC120_v5_0 \
            -lPylonUtility_MD_VC120_v5_0 \
            -lPylonC_MD_VC120 \
    }
