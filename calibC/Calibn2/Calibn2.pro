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

SOURCES += \
    main.cpp \
    utils.cpp \
    undistort_rectify.cpp \
    calib_stereo.cpp \
    calibtemp.cpp \
    cameracalibration.cpp \
    stereocalibration.cpp

HEADERS += \
    utils.h \
    undistort_rectify.h \
    calib_stereo.h \
    calibtemp.h \
    cameracalibration.h \
    stereocalibration.h

win32 {
    INCLUDEPATH += C:\opencv\build\include\
}

CONFIG(debug, debug|release) {
    LIBS += C:\opencv\build\x64\vc12\lib\opencv_core2413d.lib \
            C:\opencv\build\x64\vc12\lib\opencv_imgproc2413d.lib \
            C:\opencv\build\x64\vc12\lib\opencv_highgui2413d.lib \
            C:\opencv\build\x64\vc12\lib\opencv_ml2413d.lib \
             C:\opencv\build\x64\vc12\lib\opencv_calib3d2413d.lib \
    }
    else
    {
    LIBS += C:\opencv\build\x64\vc12\lib\opencv_core2413.lib \
            C:\opencv\build\x64\vc12\lib\opencv_imgproc2413.lib \
            C:\opencv\build\x64\vc12\lib\opencv_highgui2413.lib \
            C:\opencv\build\x64\vc12\lib\opencv_ml2413.lib \
            C:\opencv\build\x64\vc12\lib\opencv_calib3d2413.lib \
    }
