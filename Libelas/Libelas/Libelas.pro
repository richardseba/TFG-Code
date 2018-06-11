QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 console
CONFIG -= app_bundle


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    descriptor.cpp \
    elas.cpp \
    filter.cpp \
    matrix.cpp \
    triangle.cpp \
    utils.cpp \
    ../../VR_interfaz/cameracalibration.cpp \
    ../../VR_interfaz/stereocalibration.cpp

HEADERS += \
    descriptor.h \
    elas.h \
    filter.h \
    image.h \
    matrix.h \
    timer.h \
    triangle.h \
    utils.h \
    ../../VR_interfaz/cameracalibration.h \
    ../../VR_interfaz/stereocalibration.h

win32 {
    INCLUDEPATH += $$(PYLON_DEV_DIR)/include \
    INCLUDEPATH += C:\opencv3.4\build\include\
}

CONFIG(debug, debug|release) {
    LIBS += C:\opencv3.4\build\x64\vc15\lib\opencv_world341d.lib \
            -L$$(PYLON_DEV_DIR)/lib/x64 \
            -lPylonBase_MD_VC120_v5_0 \
            -lPylonUtility_MD_VC120_v5_0 \
            -lPylonC_MD_VC120 \
    }
    else
    {
    LIBS += C:\opencv3.4\build\x64\vc15\lib\opencv_world341.lib \
            -L$$(PYLON_DEV_DIR)/lib/x64 \
            -lPylonBase_MD_VC120_v5_0 \
            -lPylonUtility_MD_VC120_v5_0 \
            -lPylonC_MD_VC120 \
    }
