#-------------------------------------------------
#
# Project created by QtCreator 2017-07-12T13:08:00
#
#-------------------------------------------------

QT       += core gui widgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VR_interfaz
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    calibdialog.cpp \
    loadcalibparamsdialog.cpp \
    vrfullscreenviewer.cpp \
    selectcameraparamsdialog.cpp \
    utils.cpp \
    cameracalibration.cpp \
    stereocalibration.cpp \
    roitransition.cpp \
    temporalmean.cpp \
    processingimages.cpp \
    libelasSrc/elas.cpp \
    libelasSrc/descriptor.cpp \
    libelasSrc/filter.cpp \
    libelasSrc/matrix.cpp \
    libelasSrc/triangle.cpp \
    VR_UI_Src/qgraphicstextitemvr.cpp \
    VR_UI_Src/vrui.cpp \
    classificatorsSrc/thresholdclassificator.cpp \
    depthprocessing.cpp \
    threadedloopsevents.cpp \
    imageGeneratorSrc/imagegenerator.cpp \
    imageGeneratorSrc/videoimagegenerator.cpp \
    imageGeneratorSrc/cameraimagegenerator.cpp \
    imageGeneratorSrc/imageloadergenerator.cpp \
    histogramimage.cpp

HEADERS  += mainwindow.h \
    camera.h \
    calibdialog.h \
    loadcalibparamsdialog.h \
    vrfullscreenviewer.h \
    selectcameraparamsdialog.h \
    utils.h \
    cameracalibration.h \
    stereocalibration.h \
    roitransition.h \
    temporalmean.h \
    processingimages.h \
    libelasSrc/descriptor.h \
    libelasSrc/elas.h \
    libelasSrc/filter.h \
    libelasSrc/matrix.h \
    libelasSrc/triangle.h \
    VR_UI_Src/qgraphicstextitemvr.h \
    VR_UI_Src/vrui.h \
    classificatorsSrc/presetclassificator.h \
    classificatorsSrc/thresholdclassificator.h \
    depthprocessing.h \
    qtfilter.h \
    threadedloopsevents.h \
    imageGeneratorSrc/imagegenerator.h \
    imageGeneratorSrc/videoimagegenerator.h \
    imageGeneratorSrc/cameraimagegenerator.h \
    imageGeneratorSrc/imageloadergenerator.h \
    histogramimage.h

FORMS    += mainwindow.ui \
    calibdialog.ui \
    loadcalibparamsdialog.ui \
    selectcameraparamsdialog.ui

RESOURCES +=

win32 {
    INCLUDEPATH += $$(PYLON_DEV_DIR)/include \
    INCLUDEPATH += C:\opencv3.4\build\include\

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
}

unix{
    INCLUDEPATH += /opt/pylon5/include
    INCLUDEPATH += /usr/local/include/opencv

    LIBS += -L/opt/pylon5/lib64
    LIBS += -lNodeMapData_gcc_v3_0_Basler_pylon_v5_0 -lXmlParser_gcc_v3_0_Basler_pylon_v5_0 -lMathParser_gcc_v3_0_Basler_pylon_v5_0 -lpylonbase -lpylonc -lpylonutility -lGCBase_gcc_v3_0_Basler_pylon_v5_0 -lGenApi_gcc_v3_0_Basler_pylon_v5_0 -lLog_gcc_v3_0_Basler_pylon_v5_0

    LIBS += `pkg-config opencv --libs`
}

