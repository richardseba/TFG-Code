#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera.h"
#include "QTimer"
#include "QImage"
#include "QMessageBox"
#include "QFileDialog"
#include "QProcess"

#include <calibdialog.h>
#include <loadcalibparamsdialog.h>
#include <selectcameraparamsdialog.h>
#include "vrfullscreenviewer.h"

#include "QGraphicsScene"
#include "QGraphicsItem"
#include "QGraphicsRectItem"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <algorithm>

using namespace cv;

const int FRAME_TIMER = 0;

const int MAP_WIDTH = 1200; //Ya se puede setear automaticamente !!!
const int MAP_HIGHT = 1200; //Ya se puede setear automaticamente !!!

namespace Ui {
class MainWindow;
}

/* Class MainWindow
 * -------------------------------
 * This class is the main interface where all the mainwindow.ui
 * interaction is made.
 * uses Q_OBJECT macro
 *
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_recordingButton_clicked();
    void on_captureButton_clicked();
    void frameTimeEvent();
    void acceptedCalibParamsEvent();

    void on_exeButton_clicked();
    void on_pushButton_loadCalibration_clicked();

    void on_pushButton_Fullscreen_clicked();
    void on_pushButton_VR_clicked();

    void fullscreen_closing();

    void on_changeParamsL_pushButton_clicked();
    void acceptedCamParamsLeftEvent();
    void on_changeParamsR_pushButton_clicked();
    void acceptedCamParamsRightEvent();
    void on_switchCamera_pushButton_clicked();

private:
    bool saveImage(QImage qImage);
    void showVRViewer(int screen=0);
    void processDisparity(QImage* Im1,QImage* Im2);

    Camera* m_cameraL;
    Camera* m_cameraR;
    StereoCalibration m_stereoCalib;

    QTimer* m_timer;
    bool m_is_recording;
    bool m_calibParams_loaded;

    SelectCameraParamsDialog m_cameraParamsDialog;
    CalibDialog m_calibDialog;
    LoadCalibParamsDialog* m_loadCalibDialog;
    VrFullscreenViewer* m_screen; //distroyed automatically when exited

    Ui::MainWindow *ui;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
