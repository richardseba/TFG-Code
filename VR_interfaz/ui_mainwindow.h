/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_8;
    QGroupBox *groupBox_cameraParams;
    QHBoxLayout *horizontalLayout_11;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *changeParamsL_pushButton;
    QPushButton *changeParamsR_pushButton;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *switchCamera_pushButton;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *exeButton;
    QPushButton *pushButton_loadCalibration;
    QCheckBox *checkBox_undistort;
    QGroupBox *groupBox_capture;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBox_capture_left;
    QCheckBox *checkBox_capture_right;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *checkBox_save;
    QPushButton *captureButton;
    QGroupBox *groupBox_record;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *checkBox_continous_left;
    QCheckBox *checkBox_continous_right;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushButton_Fullscreen;
    QPushButton *pushButton_VR;
    QPushButton *recordingButton;
    QLabel *label_display1;
    QLabel *label_display2;
    QCheckBox *checkBox_saveVideo;
    QRadioButton *radioButton_recordMemory;
    QRadioButton *radioButton_recordDisk;
    QCheckBox *checkBox_colormap;
    QCheckBox *checkBox_getDepthMap;
    QSpinBox *spinBox_downSampling;
    QLabel *label;
    QSpinBox *spinBox_Libelas_setting;
    QLabel *label_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1355, 759);
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(152, 510, 1071, 141));
        horizontalLayout_8 = new QHBoxLayout(layoutWidget);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, 0, 0, 0);
        groupBox_cameraParams = new QGroupBox(layoutWidget);
        groupBox_cameraParams->setObjectName(QStringLiteral("groupBox_cameraParams"));
        horizontalLayout_11 = new QHBoxLayout(groupBox_cameraParams);
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        changeParamsL_pushButton = new QPushButton(groupBox_cameraParams);
        changeParamsL_pushButton->setObjectName(QStringLiteral("changeParamsL_pushButton"));

        horizontalLayout_10->addWidget(changeParamsL_pushButton);

        changeParamsR_pushButton = new QPushButton(groupBox_cameraParams);
        changeParamsR_pushButton->setObjectName(QStringLiteral("changeParamsR_pushButton"));

        horizontalLayout_10->addWidget(changeParamsR_pushButton);


        verticalLayout_8->addLayout(horizontalLayout_10);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        switchCamera_pushButton = new QPushButton(groupBox_cameraParams);
        switchCamera_pushButton->setObjectName(QStringLiteral("switchCamera_pushButton"));
        switchCamera_pushButton->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(switchCamera_pushButton->sizePolicy().hasHeightForWidth());
        switchCamera_pushButton->setSizePolicy(sizePolicy);
        switchCamera_pushButton->setMinimumSize(QSize(2, 1));

        horizontalLayout_16->addWidget(switchCamera_pushButton);


        verticalLayout_8->addLayout(horizontalLayout_16);


        horizontalLayout_11->addLayout(verticalLayout_8);


        horizontalLayout_8->addWidget(groupBox_cameraParams);

        groupBox_3 = new QGroupBox(layoutWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        horizontalLayout_5 = new QHBoxLayout(groupBox_3);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        exeButton = new QPushButton(groupBox_3);
        exeButton->setObjectName(QStringLiteral("exeButton"));

        horizontalLayout->addWidget(exeButton);

        pushButton_loadCalibration = new QPushButton(groupBox_3);
        pushButton_loadCalibration->setObjectName(QStringLiteral("pushButton_loadCalibration"));

        horizontalLayout->addWidget(pushButton_loadCalibration);


        verticalLayout->addLayout(horizontalLayout);

        checkBox_undistort = new QCheckBox(groupBox_3);
        checkBox_undistort->setObjectName(QStringLiteral("checkBox_undistort"));

        verticalLayout->addWidget(checkBox_undistort);


        horizontalLayout_5->addLayout(verticalLayout);


        horizontalLayout_8->addWidget(groupBox_3);

        groupBox_capture = new QGroupBox(layoutWidget);
        groupBox_capture->setObjectName(QStringLiteral("groupBox_capture"));
        horizontalLayout_6 = new QHBoxLayout(groupBox_capture);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        checkBox_capture_left = new QCheckBox(groupBox_capture);
        checkBox_capture_left->setObjectName(QStringLiteral("checkBox_capture_left"));

        horizontalLayout_3->addWidget(checkBox_capture_left);

        checkBox_capture_right = new QCheckBox(groupBox_capture);
        checkBox_capture_right->setObjectName(QStringLiteral("checkBox_capture_right"));

        horizontalLayout_3->addWidget(checkBox_capture_right);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        checkBox_save = new QCheckBox(groupBox_capture);
        checkBox_save->setObjectName(QStringLiteral("checkBox_save"));

        horizontalLayout_2->addWidget(checkBox_save);

        captureButton = new QPushButton(groupBox_capture);
        captureButton->setObjectName(QStringLiteral("captureButton"));
        captureButton->setEnabled(true);

        horizontalLayout_2->addWidget(captureButton);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout_6->addLayout(verticalLayout_2);


        horizontalLayout_8->addWidget(groupBox_capture);

        groupBox_record = new QGroupBox(layoutWidget);
        groupBox_record->setObjectName(QStringLiteral("groupBox_record"));
        horizontalLayout_7 = new QHBoxLayout(groupBox_record);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        checkBox_continous_left = new QCheckBox(groupBox_record);
        checkBox_continous_left->setObjectName(QStringLiteral("checkBox_continous_left"));

        horizontalLayout_4->addWidget(checkBox_continous_left);

        checkBox_continous_right = new QCheckBox(groupBox_record);
        checkBox_continous_right->setObjectName(QStringLiteral("checkBox_continous_right"));

        horizontalLayout_4->addWidget(checkBox_continous_right);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        pushButton_Fullscreen = new QPushButton(groupBox_record);
        pushButton_Fullscreen->setObjectName(QStringLiteral("pushButton_Fullscreen"));
        pushButton_Fullscreen->setAutoDefault(false);

        verticalLayout_5->addWidget(pushButton_Fullscreen);

        pushButton_VR = new QPushButton(groupBox_record);
        pushButton_VR->setObjectName(QStringLiteral("pushButton_VR"));

        verticalLayout_5->addWidget(pushButton_VR);


        horizontalLayout_9->addLayout(verticalLayout_5);

        recordingButton = new QPushButton(groupBox_record);
        recordingButton->setObjectName(QStringLiteral("recordingButton"));

        horizontalLayout_9->addWidget(recordingButton);


        verticalLayout_3->addLayout(horizontalLayout_9);


        horizontalLayout_7->addLayout(verticalLayout_3);


        horizontalLayout_8->addWidget(groupBox_record);

        label_display1 = new QLabel(centralWidget);
        label_display1->setObjectName(QStringLiteral("label_display1"));
        label_display1->setGeometry(QRect(130, 30, 581, 451));
        label_display1->setFrameShape(QFrame::Box);
        label_display1->setMidLineWidth(0);
        label_display1->setScaledContents(true);
        label_display2 = new QLabel(centralWidget);
        label_display2->setObjectName(QStringLiteral("label_display2"));
        label_display2->setGeometry(QRect(714, 30, 581, 451));
        label_display2->setFrameShape(QFrame::Box);
        label_display2->setScaledContents(true);
        checkBox_saveVideo = new QCheckBox(centralWidget);
        checkBox_saveVideo->setObjectName(QStringLiteral("checkBox_saveVideo"));
        checkBox_saveVideo->setGeometry(QRect(600, 690, 121, 31));
        radioButton_recordMemory = new QRadioButton(centralWidget);
        radioButton_recordMemory->setObjectName(QStringLiteral("radioButton_recordMemory"));
        radioButton_recordMemory->setGeometry(QRect(960, 690, 131, 31));
        radioButton_recordDisk = new QRadioButton(centralWidget);
        radioButton_recordDisk->setObjectName(QStringLiteral("radioButton_recordDisk"));
        radioButton_recordDisk->setGeometry(QRect(760, 690, 161, 31));
        checkBox_colormap = new QCheckBox(centralWidget);
        checkBox_colormap->setObjectName(QStringLiteral("checkBox_colormap"));
        checkBox_colormap->setGeometry(QRect(390, 680, 161, 31));
        checkBox_getDepthMap = new QCheckBox(centralWidget);
        checkBox_getDepthMap->setObjectName(QStringLiteral("checkBox_getDepthMap"));
        checkBox_getDepthMap->setGeometry(QRect(190, 680, 181, 31));
        spinBox_downSampling = new QSpinBox(centralWidget);
        spinBox_downSampling->setObjectName(QStringLiteral("spinBox_downSampling"));
        spinBox_downSampling->setGeometry(QRect(100, 690, 42, 22));
        spinBox_downSampling->setMaximum(99);
        spinBox_downSampling->setSingleStep(2);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(100, 660, 121, 31));
        spinBox_Libelas_setting = new QSpinBox(centralWidget);
        spinBox_Libelas_setting->setObjectName(QStringLiteral("spinBox_Libelas_setting"));
        spinBox_Libelas_setting->setGeometry(QRect(20, 600, 42, 22));
        spinBox_Libelas_setting->setMaximum(2);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 570, 121, 31));
        MainWindow->setCentralWidget(centralWidget);
        label_display1->raise();
        label_display2->raise();
        layoutWidget->raise();
        checkBox_saveVideo->raise();
        radioButton_recordMemory->raise();
        radioButton_recordDisk->raise();
        checkBox_colormap->raise();
        checkBox_getDepthMap->raise();
        spinBox_downSampling->raise();
        label->raise();
        spinBox_Libelas_setting->raise();
        label_2->raise();
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "VR Viewer", nullptr));
        groupBox_cameraParams->setTitle(QApplication::translate("MainWindow", "Camera Setup", nullptr));
        changeParamsL_pushButton->setText(QApplication::translate("MainWindow", "Camera Left", nullptr));
        changeParamsR_pushButton->setText(QApplication::translate("MainWindow", "Camera Right", nullptr));
        switchCamera_pushButton->setText(QApplication::translate("MainWindow", "Switch Cameras", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Calibration parameters", nullptr));
        exeButton->setText(QApplication::translate("MainWindow", "Recalibrate", nullptr));
        pushButton_loadCalibration->setText(QApplication::translate("MainWindow", "Load Calibration", nullptr));
        checkBox_undistort->setText(QApplication::translate("MainWindow", "Undistort Images", nullptr));
        groupBox_capture->setTitle(QApplication::translate("MainWindow", "Capture Image", nullptr));
        checkBox_capture_left->setText(QApplication::translate("MainWindow", "Left", nullptr));
        checkBox_capture_right->setText(QApplication::translate("MainWindow", "Right", nullptr));
        checkBox_save->setText(QApplication::translate("MainWindow", "Save", nullptr));
        captureButton->setText(QApplication::translate("MainWindow", "Capture", nullptr));
        groupBox_record->setTitle(QApplication::translate("MainWindow", "Continous Capture", nullptr));
        checkBox_continous_left->setText(QApplication::translate("MainWindow", "Left", nullptr));
        checkBox_continous_right->setText(QApplication::translate("MainWindow", "Right", nullptr));
        pushButton_Fullscreen->setText(QApplication::translate("MainWindow", "Fullscreen", nullptr));
        pushButton_VR->setText(QApplication::translate("MainWindow", "VR", nullptr));
        recordingButton->setText(QApplication::translate("MainWindow", "Start", nullptr));
        label_display1->setText(QString());
        label_display2->setText(QString());
        checkBox_saveVideo->setText(QApplication::translate("MainWindow", "Record", nullptr));
        radioButton_recordMemory->setText(QApplication::translate("MainWindow", "Record in memory", nullptr));
        radioButton_recordDisk->setText(QApplication::translate("MainWindow", "Record in disk", nullptr));
        checkBox_colormap->setText(QApplication::translate("MainWindow", "Color Map", nullptr));
        checkBox_getDepthMap->setText(QApplication::translate("MainWindow", "get Depth map", nullptr));
        label->setText(QApplication::translate("MainWindow", "DownSampling", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Libelas settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
