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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_10;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_display1;
    QLabel *label_display2;
    QVBoxLayout *verticalLayout_9;
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
    QHBoxLayout *horizontalLayout_15;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_17;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_13;
    QCheckBox *checkBox_dynamicVergence;
    QCheckBox *checkBox_overLap;
    QLabel *label;
    QSpinBox *spinBox_downSampling;
    QHBoxLayout *horizontalLayout_14;
    QCheckBox *checkBox_colormap;
    QCheckBox *checkBox_getDepthMap;
    QLabel *label_2;
    QSpinBox *spinBox_Libelas_setting;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_18;
    QHBoxLayout *horizontalLayout_12;
    QCheckBox *checkBox_saveVideo;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *radioButton_recordDisk;
    QRadioButton *radioButton_recordMemory;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1430, 825);
        MainWindow->setContextMenuPolicy(Qt::NoContextMenu);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalSpacer = new QSpacerItem(20, 66, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        label_display1 = new QLabel(centralWidget);
        label_display1->setObjectName(QStringLiteral("label_display1"));
        label_display1->setMinimumSize(QSize(600, 450));
        label_display1->setFrameShape(QFrame::Box);
        label_display1->setMidLineWidth(0);
        label_display1->setScaledContents(true);

        horizontalLayout_19->addWidget(label_display1);

        label_display2 = new QLabel(centralWidget);
        label_display2->setObjectName(QStringLiteral("label_display2"));
        label_display2->setMinimumSize(QSize(600, 450));
        label_display2->setFrameShape(QFrame::Box);
        label_display2->setScaledContents(true);

        horizontalLayout_19->addWidget(label_display2);


        verticalLayout_10->addLayout(horizontalLayout_19);

        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        groupBox_cameraParams = new QGroupBox(centralWidget);
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

        groupBox_3 = new QGroupBox(centralWidget);
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

        groupBox_capture = new QGroupBox(centralWidget);
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

        groupBox_record = new QGroupBox(centralWidget);
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
        checkBox_continous_left->setChecked(true);

        horizontalLayout_4->addWidget(checkBox_continous_left);

        checkBox_continous_right = new QCheckBox(groupBox_record);
        checkBox_continous_right->setObjectName(QStringLiteral("checkBox_continous_right"));
        checkBox_continous_right->setChecked(true);

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


        verticalLayout_9->addLayout(horizontalLayout_8);

        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_17 = new QHBoxLayout(groupBox_2);
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        checkBox_dynamicVergence = new QCheckBox(groupBox_2);
        checkBox_dynamicVergence->setObjectName(QStringLiteral("checkBox_dynamicVergence"));
        checkBox_dynamicVergence->setMinimumSize(QSize(160, 0));

        horizontalLayout_13->addWidget(checkBox_dynamicVergence);

        checkBox_overLap = new QCheckBox(groupBox_2);
        checkBox_overLap->setObjectName(QStringLiteral("checkBox_overLap"));
        checkBox_overLap->setMinimumSize(QSize(200, 0));

        horizontalLayout_13->addWidget(checkBox_overLap);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(150, 0));

        horizontalLayout_13->addWidget(label);

        spinBox_downSampling = new QSpinBox(groupBox_2);
        spinBox_downSampling->setObjectName(QStringLiteral("spinBox_downSampling"));
        spinBox_downSampling->setMinimumSize(QSize(40, 0));
        spinBox_downSampling->setMaximum(99);
        spinBox_downSampling->setSingleStep(2);
        spinBox_downSampling->setValue(2);

        horizontalLayout_13->addWidget(spinBox_downSampling);


        verticalLayout_7->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        checkBox_colormap = new QCheckBox(groupBox_2);
        checkBox_colormap->setObjectName(QStringLiteral("checkBox_colormap"));
        checkBox_colormap->setMinimumSize(QSize(160, 0));

        horizontalLayout_14->addWidget(checkBox_colormap);

        checkBox_getDepthMap = new QCheckBox(groupBox_2);
        checkBox_getDepthMap->setObjectName(QStringLiteral("checkBox_getDepthMap"));
        checkBox_getDepthMap->setMinimumSize(QSize(200, 0));

        horizontalLayout_14->addWidget(checkBox_getDepthMap);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(150, 0));

        horizontalLayout_14->addWidget(label_2);

        spinBox_Libelas_setting = new QSpinBox(groupBox_2);
        spinBox_Libelas_setting->setObjectName(QStringLiteral("spinBox_Libelas_setting"));
        spinBox_Libelas_setting->setMinimumSize(QSize(40, 0));
        spinBox_Libelas_setting->setMaximum(2);
        spinBox_Libelas_setting->setValue(2);

        horizontalLayout_14->addWidget(spinBox_Libelas_setting);


        verticalLayout_7->addLayout(horizontalLayout_14);


        horizontalLayout_17->addLayout(verticalLayout_7);


        horizontalLayout_15->addWidget(groupBox_2);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_18 = new QHBoxLayout(groupBox);
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        checkBox_saveVideo = new QCheckBox(groupBox);
        checkBox_saveVideo->setObjectName(QStringLiteral("checkBox_saveVideo"));

        horizontalLayout_12->addWidget(checkBox_saveVideo);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        radioButton_recordDisk = new QRadioButton(groupBox);
        radioButton_recordDisk->setObjectName(QStringLiteral("radioButton_recordDisk"));
        radioButton_recordDisk->setMinimumSize(QSize(150, 0));

        verticalLayout_4->addWidget(radioButton_recordDisk);

        radioButton_recordMemory = new QRadioButton(groupBox);
        radioButton_recordMemory->setObjectName(QStringLiteral("radioButton_recordMemory"));
        radioButton_recordMemory->setMinimumSize(QSize(150, 0));

        verticalLayout_4->addWidget(radioButton_recordMemory);


        horizontalLayout_12->addLayout(verticalLayout_4);


        horizontalLayout_18->addLayout(horizontalLayout_12);


        horizontalLayout_15->addWidget(groupBox);


        verticalLayout_9->addLayout(horizontalLayout_15);


        verticalLayout_10->addLayout(verticalLayout_9);


        gridLayout->addLayout(verticalLayout_10, 1, 1, 2, 1);

        horizontalSpacer_2 = new QSpacerItem(98, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 2, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 65, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 3, 1, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "VR Viewer", nullptr));
        label_display1->setText(QString());
        label_display2->setText(QString());
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
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Depth map", nullptr));
        checkBox_dynamicVergence->setText(QApplication::translate("MainWindow", "Dynamic Vergence", nullptr));
        checkBox_overLap->setText(QApplication::translate("MainWindow", "Overlap over image", nullptr));
        label->setText(QApplication::translate("MainWindow", "DownSampling", nullptr));
        checkBox_colormap->setText(QApplication::translate("MainWindow", "Color Map", nullptr));
        checkBox_getDepthMap->setText(QApplication::translate("MainWindow", "get Depth map", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Libelas settings", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "Video Recording", nullptr));
        checkBox_saveVideo->setText(QApplication::translate("MainWindow", "Record", nullptr));
        radioButton_recordDisk->setText(QApplication::translate("MainWindow", "Record in disk", nullptr));
        radioButton_recordMemory->setText(QApplication::translate("MainWindow", "Record in memory", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
