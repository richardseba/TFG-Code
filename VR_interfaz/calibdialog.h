#ifndef CALIBDIALOG_H
#define CALIBDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDebug>
#include "QMessageBox"

#include "cameracalibration.h"
#include "stereocalibration.h"

namespace Ui {
class CalibDialog;
}

/* Class CalibDialog
 * -------------------------------
 * This class is used to set an easy to use interface that
 * allows users create new calibration parameters files
 *
 * call reject if the loading params is cancelled
 * call accept if the loading params is accepted
*/
class CalibDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibDialog(QWidget *parent = 0);
    ~CalibDialog();

    CameraCalibration getLeftCalibration();
    CameraCalibration getRightCalibration();
    StereoCalibration getStereoCalibration();

private slots:
    void on_pushButton_leftpath_clicked();
    void on_pushButton_rightpath_clicked();

    void on_pushButton_calibrate_clicked();
    void on_pushButton_Cancel_clicked();

private:
    CameraCalibration m_leftCam;
    CameraCalibration m_rightCam;
    StereoCalibration m_stereoCalib;

    QString m_leftPath;
    QString m_rightPath;
    Ui::CalibDialog *ui;
};

#endif // CALIBDIALOG_H
