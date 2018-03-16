/********************************************************************************
** Form generated from reading UI file 'calibdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALIBDIALOG_H
#define UI_CALIBDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_CalibDialog
{
public:
    QGroupBox *groupBox;
    QSpinBox *spinBox_col;
    QSpinBox *spinBox_row;
    QLabel *label;
    QLabel *label_2;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_leftpath;
    QPushButton *pushButton_rightpath;
    QLineEdit *lineEdit_left;
    QLineEdit *lineEdit_right;
    QPushButton *pushButton_calibrate;
    QPushButton *pushButton_Cancel;
    QGroupBox *groupBox_3;
    QLineEdit *lineEdit_python;
    QPushButton *pushButton_pythonexe;
    QPushButton *pushButton_script;
    QLineEdit *lineEdit_script;

    void setupUi(QDialog *CalibDialog)
    {
        if (CalibDialog->objectName().isEmpty())
            CalibDialog->setObjectName(QStringLiteral("CalibDialog"));
        CalibDialog->resize(400, 409);
        groupBox = new QGroupBox(CalibDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(30, 272, 141, 91));
        spinBox_col = new QSpinBox(groupBox);
        spinBox_col->setObjectName(QStringLiteral("spinBox_col"));
        spinBox_col->setGeometry(QRect(84, 49, 42, 22));
        spinBox_col->setMinimum(2);
        spinBox_row = new QSpinBox(groupBox);
        spinBox_row->setObjectName(QStringLiteral("spinBox_row"));
        spinBox_row->setGeometry(QRect(15, 50, 42, 22));
        spinBox_row->setMinimum(2);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(15, 21, 47, 13));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(84, 19, 47, 13));
        groupBox_2 = new QGroupBox(CalibDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(30, 156, 331, 101));
        pushButton_leftpath = new QPushButton(groupBox_2);
        pushButton_leftpath->setObjectName(QStringLiteral("pushButton_leftpath"));
        pushButton_leftpath->setGeometry(QRect(230, 20, 90, 23));
        pushButton_leftpath->setAutoDefault(false);
        pushButton_rightpath = new QPushButton(groupBox_2);
        pushButton_rightpath->setObjectName(QStringLiteral("pushButton_rightpath"));
        pushButton_rightpath->setGeometry(QRect(230, 60, 90, 23));
        pushButton_rightpath->setAutoDefault(false);
        lineEdit_left = new QLineEdit(groupBox_2);
        lineEdit_left->setObjectName(QStringLiteral("lineEdit_left"));
        lineEdit_left->setGeometry(QRect(20, 20, 200, 20));
        lineEdit_right = new QLineEdit(groupBox_2);
        lineEdit_right->setObjectName(QStringLiteral("lineEdit_right"));
        lineEdit_right->setGeometry(QRect(20, 60, 200, 20));
        pushButton_calibrate = new QPushButton(CalibDialog);
        pushButton_calibrate->setObjectName(QStringLiteral("pushButton_calibrate"));
        pushButton_calibrate->setGeometry(QRect(201, 340, 75, 23));
        pushButton_Cancel = new QPushButton(CalibDialog);
        pushButton_Cancel->setObjectName(QStringLiteral("pushButton_Cancel"));
        pushButton_Cancel->setGeometry(QRect(286, 340, 75, 23));
        pushButton_Cancel->setAutoDefault(false);
        groupBox_3 = new QGroupBox(CalibDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(30, 30, 331, 111));
        lineEdit_python = new QLineEdit(groupBox_3);
        lineEdit_python->setObjectName(QStringLiteral("lineEdit_python"));
        lineEdit_python->setGeometry(QRect(20, 30, 200, 20));
        pushButton_pythonexe = new QPushButton(groupBox_3);
        pushButton_pythonexe->setObjectName(QStringLiteral("pushButton_pythonexe"));
        pushButton_pythonexe->setGeometry(QRect(230, 30, 90, 23));
        pushButton_pythonexe->setAutoDefault(false);
        pushButton_script = new QPushButton(groupBox_3);
        pushButton_script->setObjectName(QStringLiteral("pushButton_script"));
        pushButton_script->setGeometry(QRect(230, 70, 90, 23));
        pushButton_script->setAutoDefault(false);
        lineEdit_script = new QLineEdit(groupBox_3);
        lineEdit_script->setObjectName(QStringLiteral("lineEdit_script"));
        lineEdit_script->setGeometry(QRect(20, 70, 200, 20));

        retranslateUi(CalibDialog);

        QMetaObject::connectSlotsByName(CalibDialog);
    } // setupUi

    void retranslateUi(QDialog *CalibDialog)
    {
        CalibDialog->setWindowTitle(QApplication::translate("CalibDialog", "Dialog", nullptr));
        groupBox->setTitle(QApplication::translate("CalibDialog", "Chessboard Pattern", nullptr));
        label->setText(QApplication::translate("CalibDialog", "Rows", nullptr));
        label_2->setText(QApplication::translate("CalibDialog", "Colums", nullptr));
        groupBox_2->setTitle(QApplication::translate("CalibDialog", "Calibration Photos Folder", nullptr));
        pushButton_leftpath->setText(QApplication::translate("CalibDialog", "Camera Left", nullptr));
        pushButton_rightpath->setText(QApplication::translate("CalibDialog", "Camera Right", nullptr));
        pushButton_calibrate->setText(QApplication::translate("CalibDialog", "Calibrate", nullptr));
        pushButton_Cancel->setText(QApplication::translate("CalibDialog", "Cancel", nullptr));
        groupBox_3->setTitle(QApplication::translate("CalibDialog", "Execution Parameters", nullptr));
        pushButton_pythonexe->setText(QApplication::translate("CalibDialog", "Python Path", nullptr));
        pushButton_script->setText(QApplication::translate("CalibDialog", "Script Path", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalibDialog: public Ui_CalibDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBDIALOG_H
