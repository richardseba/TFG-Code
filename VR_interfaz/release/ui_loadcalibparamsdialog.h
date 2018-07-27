/********************************************************************************
** Form generated from reading UI file 'loadcalibparamsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADCALIBPARAMSDIALOG_H
#define UI_LOADCALIBPARAMSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_LoadCalibParamsDialog
{
public:
    QPushButton *pushButton_load;
    QPushButton *pushButton_cancel;
    QGroupBox *groupBox_3;
    QLineEdit *lineEdit_matrixL;
    QPushButton *pushButton_matrixL;
    QGroupBox *groupBox_4;
    QLineEdit *lineEdit_matrixR;
    QPushButton *pushButton_matrixR;
    QGroupBox *groupBox;
    QLineEdit *lineEdit_stereoCalib;
    QPushButton *pushButton_stereo;

    void setupUi(QDialog *LoadCalibParamsDialog)
    {
        if (LoadCalibParamsDialog->objectName().isEmpty())
            LoadCalibParamsDialog->setObjectName(QStringLiteral("LoadCalibParamsDialog"));
        LoadCalibParamsDialog->resize(520, 366);
        pushButton_load = new QPushButton(LoadCalibParamsDialog);
        pushButton_load->setObjectName(QStringLiteral("pushButton_load"));
        pushButton_load->setGeometry(QRect(310, 310, 91, 41));
        pushButton_cancel = new QPushButton(LoadCalibParamsDialog);
        pushButton_cancel->setObjectName(QStringLiteral("pushButton_cancel"));
        pushButton_cancel->setGeometry(QRect(410, 310, 91, 41));
        groupBox_3 = new QGroupBox(LoadCalibParamsDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(30, 30, 471, 81));
        lineEdit_matrixL = new QLineEdit(groupBox_3);
        lineEdit_matrixL->setObjectName(QStringLiteral("lineEdit_matrixL"));
        lineEdit_matrixL->setGeometry(QRect(20, 30, 331, 31));
        pushButton_matrixL = new QPushButton(groupBox_3);
        pushButton_matrixL->setObjectName(QStringLiteral("pushButton_matrixL"));
        pushButton_matrixL->setGeometry(QRect(359, 30, 101, 31));
        groupBox_4 = new QGroupBox(LoadCalibParamsDialog);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(30, 120, 471, 81));
        lineEdit_matrixR = new QLineEdit(groupBox_4);
        lineEdit_matrixR->setObjectName(QStringLiteral("lineEdit_matrixR"));
        lineEdit_matrixR->setGeometry(QRect(20, 30, 331, 31));
        pushButton_matrixR = new QPushButton(groupBox_4);
        pushButton_matrixR->setObjectName(QStringLiteral("pushButton_matrixR"));
        pushButton_matrixR->setGeometry(QRect(360, 30, 101, 31));
        groupBox = new QGroupBox(LoadCalibParamsDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(30, 210, 471, 81));
        lineEdit_stereoCalib = new QLineEdit(groupBox);
        lineEdit_stereoCalib->setObjectName(QStringLiteral("lineEdit_stereoCalib"));
        lineEdit_stereoCalib->setGeometry(QRect(20, 29, 331, 31));
        pushButton_stereo = new QPushButton(groupBox);
        pushButton_stereo->setObjectName(QStringLiteral("pushButton_stereo"));
        pushButton_stereo->setGeometry(QRect(360, 30, 101, 31));

        retranslateUi(LoadCalibParamsDialog);

        QMetaObject::connectSlotsByName(LoadCalibParamsDialog);
    } // setupUi

    void retranslateUi(QDialog *LoadCalibParamsDialog)
    {
        LoadCalibParamsDialog->setWindowTitle(QApplication::translate("LoadCalibParamsDialog", "Dialog", nullptr));
        pushButton_load->setText(QApplication::translate("LoadCalibParamsDialog", "Load", nullptr));
        pushButton_cancel->setText(QApplication::translate("LoadCalibParamsDialog", "Cancel", nullptr));
        groupBox_3->setTitle(QApplication::translate("LoadCalibParamsDialog", "Camera Left", nullptr));
        pushButton_matrixL->setText(QApplication::translate("LoadCalibParamsDialog", "Load YAML", nullptr));
        groupBox_4->setTitle(QApplication::translate("LoadCalibParamsDialog", "Camera Right", nullptr));
        pushButton_matrixR->setText(QApplication::translate("LoadCalibParamsDialog", "Load YAML", nullptr));
        groupBox->setTitle(QApplication::translate("LoadCalibParamsDialog", "Stereo Parameters", nullptr));
        pushButton_stereo->setText(QApplication::translate("LoadCalibParamsDialog", "Load YAML", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoadCalibParamsDialog: public Ui_LoadCalibParamsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADCALIBPARAMSDIALOG_H
