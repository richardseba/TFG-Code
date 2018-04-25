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
    QSpinBox *spinBox_squareSize;
    QLabel *label_3;
    QSpinBox *spinBox_numImg;
    QLabel *label_4;
    QGroupBox *groupBox_2;
    QPushButton *pushButton_leftpath;
    QPushButton *pushButton_rightpath;
    QLineEdit *lineEdit_left;
    QLineEdit *lineEdit_right;
    QPushButton *pushButton_calibrate;
    QPushButton *pushButton_Cancel;

    void setupUi(QDialog *CalibDialog)
    {
        if (CalibDialog->objectName().isEmpty())
            CalibDialog->setObjectName(QStringLiteral("CalibDialog"));
        CalibDialog->resize(568, 339);
        groupBox = new QGroupBox(CalibDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 210, 411, 111));
        spinBox_col = new QSpinBox(groupBox);
        spinBox_col->setObjectName(QStringLiteral("spinBox_col"));
        spinBox_col->setGeometry(QRect(90, 50, 71, 41));
        spinBox_col->setMinimum(2);
        spinBox_col->setValue(7);
        spinBox_row = new QSpinBox(groupBox);
        spinBox_row->setObjectName(QStringLiteral("spinBox_row"));
        spinBox_row->setGeometry(QRect(10, 50, 71, 41));
        spinBox_row->setMinimum(2);
        spinBox_row->setValue(11);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 71, 31));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(90, 20, 71, 31));
        spinBox_squareSize = new QSpinBox(groupBox);
        spinBox_squareSize->setObjectName(QStringLiteral("spinBox_squareSize"));
        spinBox_squareSize->setGeometry(QRect(190, 50, 71, 41));
        spinBox_squareSize->setMinimum(1);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(190, 20, 111, 31));
        spinBox_numImg = new QSpinBox(groupBox);
        spinBox_numImg->setObjectName(QStringLiteral("spinBox_numImg"));
        spinBox_numImg->setGeometry(QRect(280, 50, 71, 41));
        spinBox_numImg->setMinimum(3);
        spinBox_numImg->setMaximum(999);
        spinBox_numImg->setValue(25);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(280, 20, 131, 31));
        groupBox_2 = new QGroupBox(CalibDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 20, 531, 171));
        pushButton_leftpath = new QPushButton(groupBox_2);
        pushButton_leftpath->setObjectName(QStringLiteral("pushButton_leftpath"));
        pushButton_leftpath->setGeometry(QRect(400, 50, 111, 31));
        pushButton_leftpath->setAutoDefault(false);
        pushButton_rightpath = new QPushButton(groupBox_2);
        pushButton_rightpath->setObjectName(QStringLiteral("pushButton_rightpath"));
        pushButton_rightpath->setGeometry(QRect(400, 120, 111, 31));
        pushButton_rightpath->setAutoDefault(false);
        lineEdit_left = new QLineEdit(groupBox_2);
        lineEdit_left->setObjectName(QStringLiteral("lineEdit_left"));
        lineEdit_left->setGeometry(QRect(20, 50, 361, 31));
        lineEdit_right = new QLineEdit(groupBox_2);
        lineEdit_right->setObjectName(QStringLiteral("lineEdit_right"));
        lineEdit_right->setGeometry(QRect(20, 120, 361, 31));
        pushButton_calibrate = new QPushButton(CalibDialog);
        pushButton_calibrate->setObjectName(QStringLiteral("pushButton_calibrate"));
        pushButton_calibrate->setGeometry(QRect(440, 220, 101, 41));
        pushButton_Cancel = new QPushButton(CalibDialog);
        pushButton_Cancel->setObjectName(QStringLiteral("pushButton_Cancel"));
        pushButton_Cancel->setGeometry(QRect(440, 270, 101, 41));
        pushButton_Cancel->setAutoDefault(false);

        retranslateUi(CalibDialog);

        QMetaObject::connectSlotsByName(CalibDialog);
    } // setupUi

    void retranslateUi(QDialog *CalibDialog)
    {
        CalibDialog->setWindowTitle(QApplication::translate("CalibDialog", "Dialog", nullptr));
        groupBox->setTitle(QApplication::translate("CalibDialog", "Chessboard Pattern", nullptr));
        label->setText(QApplication::translate("CalibDialog", "Rows", nullptr));
        label_2->setText(QApplication::translate("CalibDialog", "Colums", nullptr));
        label_3->setText(QApplication::translate("CalibDialog", "Square Size", nullptr));
        label_4->setText(QApplication::translate("CalibDialog", "Number of images", nullptr));
        groupBox_2->setTitle(QApplication::translate("CalibDialog", "Calibration Photos Folder", nullptr));
        pushButton_leftpath->setText(QApplication::translate("CalibDialog", "Camera Left", nullptr));
        pushButton_rightpath->setText(QApplication::translate("CalibDialog", "Camera Right", nullptr));
        pushButton_calibrate->setText(QApplication::translate("CalibDialog", "Calibrate", nullptr));
        pushButton_Cancel->setText(QApplication::translate("CalibDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CalibDialog: public Ui_CalibDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALIBDIALOG_H
