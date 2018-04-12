#ifndef LOADCALIBPARAMSDIALOG_H
#define LOADCALIBPARAMSDIALOG_H

#include <QDialog>
#include "QFileDialog"

namespace Ui {
class LoadCalibParamsDialog;
}

/* Class CalibDialog
 * -------------------------------
 * This class is used to set an easy to use interface that
 * allows users load other calibration parameters
 * uses Q_OBJECT macro
 *
 * call reject if the loading params is cancelled
 * call accept if the loading params is accepted
*/
class LoadCalibParamsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadCalibParamsDialog(QWidget *parent = 0);
    ~LoadCalibParamsDialog();

    QString getCamLConfigPath();
    QString getCamRConfigPath();
    QString getStereoConfigPath();

private slots:
    void on_pushButton_load_clicked();
    void on_pushButton_cancel_clicked();

    void on_pushButton_matrixR_clicked();
    void on_pushButton_matrixL_clicked();
    void on_pushButton_stereo_clicked();

private:
    Ui::LoadCalibParamsDialog *ui;
};

#endif // LOADCALIBPARAMSDIALOG_H
