/********************************************************************************
** Form generated from reading UI file 'selectcameraparamsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTCAMERAPARAMSDIALOG_H
#define UI_SELECTCAMERAPARAMSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SelectCameraParamsDialog
{
public:
    QPushButton *Load_pushButton;
    QPushButton *Cancel_pushButton;
    QGroupBox *groupBox;
    QSlider *width_slider;
    QSlider *height_slider;
    QLabel *label_5;
    QLabel *label_6;
    QSpinBox *width_spinBox;
    QSpinBox *height_spinBox;
    QGroupBox *groupBox_2;
    QSlider *xOffset_slider;
    QSlider *yOffset_slider;
    QLabel *label_3;
    QLabel *label_4;
    QSpinBox *xOffset_spinBox;
    QSpinBox *yOffset_spinBox;
    QGroupBox *groupBox_3;
    QSlider *vBinning_slider;
    QSlider *hBinning_slider;
    QLabel *label;
    QLabel *label_2;
    QSpinBox *vBinning_spinBox;
    QSpinBox *hBinning_spinBox;
    QPushButton *fromYAML_pushButton;
    QGroupBox *groupBox_4;
    QCheckBox *yFlip_checkBox;
    QCheckBox *xFlip_checkBox;
    QPushButton *save_YAML_pushButton;

    void setupUi(QDialog *SelectCameraParamsDialog)
    {
        if (SelectCameraParamsDialog->objectName().isEmpty())
            SelectCameraParamsDialog->setObjectName(QStringLiteral("SelectCameraParamsDialog"));
        SelectCameraParamsDialog->resize(453, 493);
        Load_pushButton = new QPushButton(SelectCameraParamsDialog);
        Load_pushButton->setObjectName(QStringLiteral("Load_pushButton"));
        Load_pushButton->setGeometry(QRect(330, 390, 91, 31));
        Cancel_pushButton = new QPushButton(SelectCameraParamsDialog);
        Cancel_pushButton->setObjectName(QStringLiteral("Cancel_pushButton"));
        Cancel_pushButton->setGeometry(QRect(330, 430, 91, 31));
        groupBox = new QGroupBox(SelectCameraParamsDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(30, 20, 391, 111));
        width_slider = new QSlider(groupBox);
        width_slider->setObjectName(QStringLiteral("width_slider"));
        width_slider->setGeometry(QRect(200, 30, 181, 31));
        width_slider->setInputMethodHints(Qt::ImhPreferNumbers);
        width_slider->setSingleStep(2);
        width_slider->setOrientation(Qt::Horizontal);
        width_slider->setTickInterval(0);
        height_slider = new QSlider(groupBox);
        height_slider->setObjectName(QStringLiteral("height_slider"));
        height_slider->setGeometry(QRect(200, 70, 181, 31));
        height_slider->setSingleStep(2);
        height_slider->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 30, 51, 31));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 70, 51, 31));
        width_spinBox = new QSpinBox(groupBox);
        width_spinBox->setObjectName(QStringLiteral("width_spinBox"));
        width_spinBox->setGeometry(QRect(120, 30, 71, 31));
        width_spinBox->setSingleStep(2);
        height_spinBox = new QSpinBox(groupBox);
        height_spinBox->setObjectName(QStringLiteral("height_spinBox"));
        height_spinBox->setGeometry(QRect(120, 70, 71, 31));
        height_spinBox->setMaximum(99);
        height_spinBox->setSingleStep(2);
        groupBox_2 = new QGroupBox(SelectCameraParamsDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(30, 140, 391, 111));
        xOffset_slider = new QSlider(groupBox_2);
        xOffset_slider->setObjectName(QStringLiteral("xOffset_slider"));
        xOffset_slider->setGeometry(QRect(200, 30, 181, 31));
        xOffset_slider->setSingleStep(2);
        xOffset_slider->setOrientation(Qt::Horizontal);
        xOffset_slider->setTickPosition(QSlider::NoTicks);
        xOffset_slider->setTickInterval(0);
        yOffset_slider = new QSlider(groupBox_2);
        yOffset_slider->setObjectName(QStringLiteral("yOffset_slider"));
        yOffset_slider->setGeometry(QRect(200, 70, 181, 31));
        yOffset_slider->setSingleStep(2);
        yOffset_slider->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 30, 47, 31));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 70, 81, 31));
        xOffset_spinBox = new QSpinBox(groupBox_2);
        xOffset_spinBox->setObjectName(QStringLiteral("xOffset_spinBox"));
        xOffset_spinBox->setGeometry(QRect(120, 30, 71, 31));
        xOffset_spinBox->setSingleStep(2);
        yOffset_spinBox = new QSpinBox(groupBox_2);
        yOffset_spinBox->setObjectName(QStringLiteral("yOffset_spinBox"));
        yOffset_spinBox->setGeometry(QRect(120, 70, 71, 31));
        yOffset_spinBox->setSingleStep(2);
        groupBox_3 = new QGroupBox(SelectCameraParamsDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(30, 260, 391, 111));
        vBinning_slider = new QSlider(groupBox_3);
        vBinning_slider->setObjectName(QStringLiteral("vBinning_slider"));
        vBinning_slider->setGeometry(QRect(200, 30, 181, 31));
        vBinning_slider->setMinimum(1);
        vBinning_slider->setMaximum(4);
        vBinning_slider->setSingleStep(2);
        vBinning_slider->setOrientation(Qt::Horizontal);
        hBinning_slider = new QSlider(groupBox_3);
        hBinning_slider->setObjectName(QStringLiteral("hBinning_slider"));
        hBinning_slider->setGeometry(QRect(200, 70, 181, 31));
        hBinning_slider->setMinimum(1);
        hBinning_slider->setMaximum(4);
        hBinning_slider->setSingleStep(2);
        hBinning_slider->setOrientation(Qt::Horizontal);
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 30, 81, 31));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 70, 91, 31));
        vBinning_spinBox = new QSpinBox(groupBox_3);
        vBinning_spinBox->setObjectName(QStringLiteral("vBinning_spinBox"));
        vBinning_spinBox->setGeometry(QRect(120, 30, 71, 31));
        vBinning_spinBox->setMinimum(1);
        vBinning_spinBox->setMaximum(4);
        hBinning_spinBox = new QSpinBox(groupBox_3);
        hBinning_spinBox->setObjectName(QStringLiteral("hBinning_spinBox"));
        hBinning_spinBox->setGeometry(QRect(120, 70, 71, 31));
        hBinning_spinBox->setMinimum(1);
        hBinning_spinBox->setMaximum(4);
        fromYAML_pushButton = new QPushButton(SelectCameraParamsDialog);
        fromYAML_pushButton->setObjectName(QStringLiteral("fromYAML_pushButton"));
        fromYAML_pushButton->setGeometry(QRect(230, 390, 91, 31));
        groupBox_4 = new QGroupBox(SelectCameraParamsDialog);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(30, 380, 191, 81));
        yFlip_checkBox = new QCheckBox(groupBox_4);
        yFlip_checkBox->setObjectName(QStringLiteral("yFlip_checkBox"));
        yFlip_checkBox->setGeometry(QRect(110, 26, 70, 41));
        xFlip_checkBox = new QCheckBox(groupBox_4);
        xFlip_checkBox->setObjectName(QStringLiteral("xFlip_checkBox"));
        xFlip_checkBox->setGeometry(QRect(20, 26, 70, 41));
        save_YAML_pushButton = new QPushButton(SelectCameraParamsDialog);
        save_YAML_pushButton->setObjectName(QStringLiteral("save_YAML_pushButton"));
        save_YAML_pushButton->setGeometry(QRect(230, 430, 91, 31));

        retranslateUi(SelectCameraParamsDialog);

        QMetaObject::connectSlotsByName(SelectCameraParamsDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectCameraParamsDialog)
    {
        SelectCameraParamsDialog->setWindowTitle(QApplication::translate("SelectCameraParamsDialog", "Dialog", nullptr));
        Load_pushButton->setText(QApplication::translate("SelectCameraParamsDialog", "Load", nullptr));
        Cancel_pushButton->setText(QApplication::translate("SelectCameraParamsDialog", "Cancel", nullptr));
        groupBox->setTitle(QApplication::translate("SelectCameraParamsDialog", "Resolution", nullptr));
        label_5->setText(QApplication::translate("SelectCameraParamsDialog", "Width", nullptr));
        label_6->setText(QApplication::translate("SelectCameraParamsDialog", "Height", nullptr));
        groupBox_2->setTitle(QApplication::translate("SelectCameraParamsDialog", "ROI Offset", nullptr));
        label_3->setText(QApplication::translate("SelectCameraParamsDialog", "X", nullptr));
        label_4->setText(QApplication::translate("SelectCameraParamsDialog", "Y", nullptr));
        groupBox_3->setTitle(QApplication::translate("SelectCameraParamsDialog", "Binning", nullptr));
        label->setText(QApplication::translate("SelectCameraParamsDialog", "Vertical", nullptr));
        label_2->setText(QApplication::translate("SelectCameraParamsDialog", "Horizontal", nullptr));
        fromYAML_pushButton->setText(QApplication::translate("SelectCameraParamsDialog", "From YAML", nullptr));
        groupBox_4->setTitle(QApplication::translate("SelectCameraParamsDialog", "Flip Camera", nullptr));
        yFlip_checkBox->setText(QApplication::translate("SelectCameraParamsDialog", "Flip Y", nullptr));
        xFlip_checkBox->setText(QApplication::translate("SelectCameraParamsDialog", "Flip X", nullptr));
        save_YAML_pushButton->setText(QApplication::translate("SelectCameraParamsDialog", "Save YAML", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectCameraParamsDialog: public Ui_SelectCameraParamsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTCAMERAPARAMSDIALOG_H
