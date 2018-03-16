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
    ../Libelas/Libelas/descriptor.cpp \
    ../Libelas/Libelas/elas.cpp \
    ../Libelas/Libelas/filter.cpp \
    ../Libelas/Libelas/matrix.cpp \
    ../Libelas/Libelas/triangle.cpp

HEADERS  += mainwindow.h \
    camera.h \
    calibdialog.h \
    loadcalibparamsdialog.h \
    vrfullscreenviewer.h \
    vrimageupdater.h \
    selectcameraparamsdialog.h \
    vrui.h \
    qgraphicstextitemvr.h \
    ../Libelas/Libelas/descriptor.h \
    ../Libelas/Libelas/elas.h \
    ../Libelas/Libelas/filter.h \
    ../Libelas/Libelas/matrix.h \
    ../Libelas/Libelas/timer.h \
    ../Libelas/Libelas/triangle.h

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
    LIBS += C:\opencv\build\x86\vc12\lib\opencv_core2413d.lib \
            C:\opencv\build\x86\vc12\lib\opencv_imgproc2413d.lib \
            C:\opencv\build\x86\vc12\lib\opencv_highgui2413d.lib \
            C:\opencv\build\x86\vc12\lib\opencv_ml2413d.lib \
             C:\opencv\build\x86\vc12\lib\opencv_calib3d2413d.lib \
            -L$$(PYLON_DEV_DIR)/lib/Win32 \
            -lPylonBase_MD_VC120_v5_0 \
            -lPylonUtility_MD_VC120_v5_0 \
            -lPylonC_MD_VC120 \
    }
    else
    {
    LIBS += C:\opencv\build\x86\vc12\lib\opencv_core2413.lib \
            C:\opencv\build\x86\vc12\lib\opencv_imgproc2413.lib \
            C:\opencv\build\x86\vc12\lib\opencv_highgui2413.lib \
            C:\opencv\build\x86\vc12\lib\opencv_ml2413.lib \
            C:\opencv\build\x86\vc12\lib\opencv_calib3d2413.lib \
            -L$$(PYLON_DEV_DIR)/lib/Win32 \
            -lPylonBase_MD_VC120_v5_0 \
            -lPylonUtility_MD_VC120_v5_0 \
            -lPylonC_MD_VC120 \
    }
