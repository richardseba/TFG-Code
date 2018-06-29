#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera.h"
#include "QTimer"
#include "QImage"
#include "QMessageBox"
#include "QFileDialog"
#include "QProcess"
#include <QProgressBar>

#include <calibdialog.h>
#include <loadcalibparamsdialog.h>
#include <selectcameraparamsdialog.h>
#include "vrfullscreenviewer.h"
#include "./libelasSrc/elas.h"
#include "processingimages.h".
#include "./classificatorsSrc/presetclassificator.h"
#include "./classificatorsSrc/thresholdclassificator.h"

#include "QGraphicsScene"
#include "QGraphicsItem"
#include "QGraphicsRectItem"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <algorithm>

using namespace cv;

const int FRAME_TIMER = 0;
const int SIZE_OF_CLASSIFIER_BUFFER = 10;

const int FRAME_RATE_SAVE = 33;
const int MAX_FRAME_IN_MEMORY = 350; //64 bits
//const int MAX_FRAME_IN_MEMORY = 450; //32 bits

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
    void on_acceptedCalibrationFromFile();
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
    void saveVideo(Mat im1,Mat im2);
    void showVRViewer(int screen=0);
//    QImagePair processDisparity(QImage* Im1, QImage* Im2, bool colormap, Elas::setting elasSetting);
//    Mat getColorFrom(Mat backgroundSrc, Mat colorSrc);
    void saveVideoFromMemory(std::vector<QImage> buffer, VideoWriter video, QProgressBar* progress);


    Camera* m_cameraL;
    Camera* m_cameraR;
    StereoCalibration m_stereoCalib;

    QTimer* m_timer;
    QTime* m_time;
    float m_meanfps;
    bool m_is_recording;
    bool m_calibParams_loaded;
    bool m_savingOn;

    SelectCameraParamsDialog m_cameraParamsDialog;
    CalibDialog* m_calibDialog;
    LoadCalibParamsDialog* m_loadCalibDialog;
    VrFullscreenViewer* m_screen; //distroyed automatically when exited

    int m_photosCaptured;

    ThresholdClassificator m_classifier;

    //video things

    VideoWriter m_videoL;
    VideoWriter m_videoR;

    std::vector<QImage> m_vectorVideoL;
    std::vector<QImage> m_vectorVideoR;

    Ui::MainWindow *ui;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
