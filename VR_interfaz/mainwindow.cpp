#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDebug"
#include "../Libelas/Libelas/elas.h"
#include "../Libelas/Libelas/image.h"

#include <pylon/PylonIncludes.h>
#include <pylon/usb/BaslerUsbInstantCamera.h>
typedef Pylon::CBaslerUsbInstantCamera Camera_t;
using namespace Basler_UsbCameraParams;
using namespace Pylon;

/* Function MainWindow
 * -------------------------------
 * overloaded constructor that sets up the cameras, the calibration parameters
 * the timer and the events.
 *
 * parent : parent widget.
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_is_recording = false;
    this->m_calibParams_loaded = true;

    this->m_cameraR = new Camera(0);
    this->m_cameraL = new Camera(1);

    //Loading the yaml is optional
    this->m_cameraR->initCamParametersFromYALM("./configFiles/camRconfig.yml");
    this->m_cameraL->initCamParametersFromYALM("./configFiles/camLconfig.yml");

    this->m_calibParams_loaded&= this->m_cameraL->initCalibParams("./configFiles/calibLeft.yml");
    this->m_calibParams_loaded&= this->m_cameraR->initCalibParams("./configFiles/calibRight.yml");

    char stereoCalibFile[] = "./configFiles/calibStereo.yml";
    this->m_stereoCalib.calibrateStereoFromFile(m_cameraL->getCalibration(),m_cameraR->getCalibration(),stereoCalibFile);
    this->m_calibParams_loaded&=m_stereoCalib.isCalibrated();

    ui->checkBox_undistort->setEnabled(this->m_calibParams_loaded);
    ui->checkBox_undistort->setChecked(false);

    this->m_timer = new QTimer(this);
    this->m_time = new QTime();
    this->m_loadCalibDialog = new LoadCalibParamsDialog(this);
    this->m_calibDialog = new CalibDialog(this);

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(frameTimeEvent()));
    connect(this->m_loadCalibDialog,SIGNAL(accepted()),SLOT(acceptedCalibParamsEvent()));
    connect(this->m_calibDialog,SIGNAL(accepted()),SLOT(on_acceptedCalibrationFromFile()));

    if(m_calibParams_loaded)
    {
        Rect roi = this->m_cameraL->getCurrentROIRect();
        this->m_cameraR->initUndistortMap(Size(roi.width,roi.height));
        this->m_cameraL->initUndistortMap(Size(roi.width,roi.height));
        this->m_stereoCalib.initUndistortImage(Size(roi.width,roi.height));
    }

}

/* Function ~MainWindow
 * -------------------------------
 * build-in destructor.
*/
MainWindow::~MainWindow()
{
    this->m_cameraR->stopGrabbing();
    this->m_cameraL->stopGrabbing();

    delete m_cameraR;
    delete m_cameraL;
    delete this->m_loadCalibDialog;
    delete this->m_timer;
    PylonTerminate();
    delete ui;
}

/* Private slot on_captureButton_clicked
 * -------------------------------
 * function called when the capture button is clicked, grabs and show an image
 * of the selected cameras.
 * Prints a warning when the camera doesn't grab an image.
*/
void MainWindow::on_captureButton_clicked()
{
    QImage *ImgR,*ImgL;
    bool retL = false,retR = false;

    if(ui->checkBox_capture_left->isChecked())
        ImgL = this->m_cameraL->single_grab_image(retL);

    if(ui->checkBox_capture_right->isChecked())
        ImgR = this->m_cameraR->single_grab_image(retR);

    if(retL)
    {
        QImage copy;
        if(ui->checkBox_undistort->isChecked())
        {
            //copy = this->m_cameraL->undistortMapImage(*ImgL, CV_INTER_LINEAR);
            copy = Mat2QImage(this->m_stereoCalib.undistortLeft(QImage2Mat(*ImgL), CV_INTER_LINEAR));
            ui->label_display1->setPixmap(QPixmap::fromImage(copy));
            if(ui->checkBox_save->isChecked())
                this->saveImage(copy);
        }
        else
        {
            ui->label_display1->setPixmap(QPixmap::fromImage(*ImgL));
            if(ui->checkBox_save->isChecked())
                this->saveImage(*ImgL);
        }

        ui->label_display1->show();

        delete[] ImgL->bits();
        delete ImgL;
        ImgL = NULL;
    }
    else
    {
        qDebug() << "no left image recieved";
    }

    if(retR)
    {
        QImage copy;
        if(ui->checkBox_undistort->isChecked())
        {
            //copy = this->m_cameraR->undistortMapImage(*ImgR, CV_INTER_LINEAR);
            copy = Mat2QImage(this->m_stereoCalib.undistortRight(QImage2Mat(*ImgR), CV_INTER_LINEAR));
            ui->label_display2->setPixmap(QPixmap::fromImage(copy));
            if(ui->checkBox_save->isChecked())
                this->saveImage(copy);
        }
        else
        {
            ui->label_display2->setPixmap(QPixmap::fromImage(*ImgR));
            if(ui->checkBox_save->isChecked())
                this->saveImage(*ImgR);
        }

        ui->label_display2->show();

        delete[] ImgR->bits();
        delete ImgR;
        ImgR = NULL;
    }
    else
    {
        qDebug() << "no right image recieved";
    }
}

/* Private slot on_recordingButton_clicked
 * -------------------------------
 * function called when the recording button is pressed.
 * It will either start or finish the timer (m_timer) that fires the event that updates
 * the image display on the window.
*/
void MainWindow::on_recordingButton_clicked()
{
    if(this->m_is_recording)
    {   
        m_time->restart();
        this->m_cameraR->stopGrabbing();
        this->m_cameraL->stopGrabbing();

        qDebug() << "Ending video";
        m_video.release();


        ui->recordingButton->setText("Start");
        this->m_is_recording = false;
        this->m_timer->stop();
        ui->groupBox_capture->setEnabled(true);
        ui->pushButton_Fullscreen->setEnabled(true);
        ui->pushButton_VR->setEnabled(true);
    }
    else
    {
        qDebug() << "start" << m_video.open("./out1.avi",-1,14, Size(1100,1100));

        qDebug() << m_video.isOpened();
        this->m_cameraR->startGrabbing();
        this->m_cameraL->startGrabbing();

        m_time->restart();

        ui->recordingButton->setText("Stop");
        ui->pushButton_Fullscreen->setEnabled(false);
        ui->pushButton_VR->setEnabled(false);
        ui->groupBox_capture->setEnabled(false);
        this->m_is_recording = true;
        this->m_timer->start(FRAME_TIMER);
    }
}


/* Private slot frameTimeEvent
 * -------------------------------
 * function called every time that the m_timer timesout.
 * it grabs and updates the assigned pixmap.
*/
void MainWindow::frameTimeEvent()
{
    QImage *qImage = NULL;
    bool ret;

    bool left,right;
    left = ui->checkBox_continous_left->isChecked();
    right = ui->checkBox_continous_right->isChecked();

    if(left && right)
    {
        QImage *qImageL = this->m_cameraL->grab_image(left);
        QImage *qImageR = this->m_cameraR->grab_image(right);
        if(left && right && !ui->checkBox_saveVideo->isChecked())
        {

            QImage qImageLU = Mat2QImage(m_stereoCalib.undistortLeft(QImage2Mat(*qImageL),CV_INTER_LINEAR));
            QImage qImageRU = Mat2QImage(m_stereoCalib.undistortRight(QImage2Mat(*qImageR),CV_INTER_LINEAR));

            this->processDisparity(&qImageLU,&qImageRU);
        }
        if(left && right && ui->checkBox_saveVideo->isChecked())
        {

            Mat im1 = QImage2Mat(*qImageL);
            Mat im2 = QImage2Mat(*qImageR);

            m_video << im1;

        }

        delete[] qImageL->bits();
        delete qImageL;
        qImageL = NULL;
        delete[] qImageR->bits();
        delete qImageR;
        qImageR = NULL;

        float currentfps = 1000.0/m_time->restart();
        m_meanfps = (currentfps+m_meanfps)/2;
        qDebug() << m_meanfps;

    }
    else
    {
        if(ui->checkBox_continous_left->isChecked())
        {
            qImage = this->m_cameraL->grab_image(ret);

            if(ret)
            {
                ui->label_display1->setPixmap(QPixmap::fromImage(*qImage));
                ui->label_display1->show();

                delete[] qImage->bits();
                delete qImage;
                qImage = NULL;
            }
        }

        if(ui->checkBox_continous_right->isChecked())
        {
            qImage = this->m_cameraR->grab_image(ret);
            if(ret)
            {
                ui->label_display2->setPixmap(QPixmap::fromImage(*qImage));
                ui->label_display2->show();

                delete[] qImage->bits();
                delete qImage;
                qImage = NULL;
            }
        }
    }


}

/* Function saveImage
 * -------------------------------
 * function used to save images.
 * it will open a fileDialog to select which path is desired.
 *
 * qImage : image wanted to be saved.
 *
 * return -> boolean indicates if the image have been saved
*/
bool MainWindow::saveImage(QImage qImage)
{
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save File"),
                    "C:/Users/rsegovia/Desktop/Dataset/1100x1100/1.png",
                    tr("PNG (*.png);;JPEG (*.jpg *.jpeg)" )
                    );
    return qImage.save(imagePath);
}

/* Private slot on_exeButton_clicked
 * -------------------------------
 * function called when the recalibrate button is pressed.
 * it will show the calibration dialog.
*/
void MainWindow::on_exeButton_clicked()
{
    this->m_calibDialog->show();
}

/* Private slot on_pushButton_loadCalibration_clicked
 * -------------------------------
 * function called when the load calibration button is pressed.
 * it will show the load calibration dialog.
*/
void MainWindow::on_pushButton_loadCalibration_clicked()
{
    this->m_loadCalibDialog->show();
}

void MainWindow::on_acceptedCalibrationFromFile()
{
    m_cameraL->setCalibration(m_calibDialog->getLeftCalibration());
    m_cameraR->setCalibration(m_calibDialog->getRightCalibration());

    m_stereoCalib = m_calibDialog->getStereoCalibration();

    bool initialized = m_cameraL->isCalibrated()&&m_cameraR->isCalibrated()&&m_stereoCalib.isCalibrated();

    if(initialized)
    {
        Rect roi = m_cameraL->getCurrentROIRect();
        this->m_cameraR->initUndistortMap(Size(roi.width,roi.height));
        this->m_cameraL->initUndistortMap(Size(roi.width,roi.height));
        this->m_stereoCalib.initUndistortImage(Size(roi.width,roi.height));
    }

    this->m_calibParams_loaded = initialized;
    this->ui->checkBox_undistort->setEnabled(initialized);
    this->ui->checkBox_undistort->setChecked(ui->checkBox_undistort->isChecked()&&initialized);
}

/* Private slot acceptedCalibParamsEvent
 * -------------------------------
 * function called when the load calibration dialog is accepted.
 * it will load the new calibration parameters.
*/
void MainWindow::acceptedCalibParamsEvent()
{
    CameraCalibration camLeft(this->m_loadCalibDialog->getCamLConfigPath().toLatin1().data());
    CameraCalibration camRight(this->m_loadCalibDialog->getCamRConfigPath().toLatin1().data());

    m_cameraL->setCalibration(camLeft);
    m_cameraR->setCalibration(camRight);

    m_stereoCalib.calibrateStereoFromFile(camLeft,camRight,this->m_loadCalibDialog->getStereoConfigPath().toLatin1().data());

    bool initialized = camLeft.isCalibrated()&&camRight.isCalibrated()&&m_stereoCalib.isCalibrated();

    if(initialized)
    {
        Rect roi = m_cameraL->getCurrentROIRect();
        this->m_cameraR->initUndistortMap(Size(roi.width,roi.height));
        this->m_cameraL->initUndistortMap(Size(roi.width,roi.height));
        this->m_stereoCalib.initUndistortImage(Size(roi.width,roi.height));
    }

    this->m_calibParams_loaded = initialized;
    this->ui->checkBox_undistort->setEnabled(initialized);
    this->ui->checkBox_undistort->setChecked(ui->checkBox_undistort->isChecked()&&initialized);
}

/* Private slot on_pushButton_Fullscreen_clicked
 * -------------------------------
 * function called when the fullscreen button is pressed or when the key f11 is
 * pressed.
 * calls showVRViewer()
*/
void MainWindow::on_pushButton_Fullscreen_clicked()
{
    showVRViewer();
}

/* Private slot on_pushButton_VR_clicked
 * -------------------------------
 * function called when the VR button is pressed.
 * calls showVRViewer()
*/
void MainWindow::on_pushButton_VR_clicked()
{
    showVRViewer(1);
}

/* Function showVRViewer
 * -------------------------------
 * function called when the fullscreen button is pressed or when the key f11 is
 * pressed.
 * this function will initialize the vrFullscreenViewer and will show it.
*/
void MainWindow::showVRViewer(int screen)
{
    //To avoid executing if the main window is updating any pixmap
    if(!this->m_is_recording)
    {
        ui->groupBox_capture->setEnabled(false);
        ui->groupBox_record->setEnabled(false);
        ui->groupBox_3->setEnabled(false);
        ui->groupBox_cameraParams->setEnabled(false);

        this->m_timer->stop();
        this->m_screen = new VrFullscreenViewer(this->m_cameraL,this->m_cameraR);
        connect(this->m_screen,SIGNAL(destroyed(QObject*)),SLOT(fullscreen_closing()));
        this->m_screen->showFullScreen(screen);
    }
}

/* Private slot fullscreen_closing
 * -------------------------------
 * slot called when the fullscreenviwer is closed.
 * this function will initialize the vrFullscreenViewer and will show it.
*/
void MainWindow::fullscreen_closing()
{
    ui->groupBox_capture->setEnabled(true);
    ui->groupBox_record->setEnabled(true);
    ui->groupBox_3->setEnabled(true);
    ui->groupBox_cameraParams->setEnabled(true);
}

/* Function keyPressEvent
 * -------------------------------
 * function called when a key is pressed
 * if the key f11 is press, the on_pushButton_Fullscreen_clicked function
 * will be called.
*/
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F11:
        this->on_pushButton_Fullscreen_clicked();
    default:
        break;
    }
}

/* Private slot on_changeParamsL_pushButton_clicked
 * -------------------------------
 * slot called when the change params for the left camera is clicked
 * shows the camara params dialog.
*/
void MainWindow::on_changeParamsR_pushButton_clicked()
{
    Rect cameraRect = this->m_cameraR->getCurrentROIRect();
    m_cameraParamsDialog.setDialog(m_cameraR->getMaxWidth(),m_cameraR->getMaxHeight(),cameraRect,m_cameraR->getVBinning(),
                                   m_cameraR->getHBinning(),
                                   m_cameraR->getflipX(),m_cameraR->getflipY());
    connect(&this->m_cameraParamsDialog,SIGNAL(accepted()),SLOT(acceptedCamParamsRightEvent()));
    m_cameraParamsDialog.setWindowTitle("Camera Right");
    m_cameraParamsDialog.show();
}

/* Private slot acceptedCamParamsLeftEvent
 * -------------------------------
 * slot called when the camera params dialog is accepted
 * this function will change the parameters to the returned by the dialog
*/
void MainWindow::acceptedCamParamsRightEvent()
{
    this->m_cameraR->stopGrabbing();
    this->thread()->sleep(1);
    //this->m_cameraR->startGrabbing(GrabStrategy_LatestImageOnly);

    Rect cameraRect = m_cameraParamsDialog.getcurrentRect();

    this->m_cameraR->setROIRect(cameraRect);
    this->m_cameraR->setBinning(m_cameraParamsDialog.getHBinning(),m_cameraParamsDialog.getVBinning());
    this->m_cameraR->setflipXOutput(m_cameraParamsDialog.getXFlip());
    this->m_cameraR->setflipYOutput(m_cameraParamsDialog.getYFlip());
    this->m_cameraR->startGrabbing();

    this->m_cameraParamsDialog.disconnect();

    if(m_calibParams_loaded)
    {
        Rect roiR = m_cameraR->getCurrentROIRect();
        this->m_cameraR->initUndistortMap(Size(roiR.width,roiR.height));

        Rect roiL = m_cameraL->getCurrentROIRect();
        this->m_cameraL->initUndistortMap(Size(roiL.width,roiL.height));

        this->m_stereoCalib.initUndistortImage(Size(roiR.width,roiR.height));
    }
}

/* Private slot on_changeParamsR_pushButton_clicked
 * -------------------------------
 * slot called when the change params for the left camera is clicked
 * shows the camara params dialog.
*/
void MainWindow::on_changeParamsL_pushButton_clicked()
{
    Rect cameraRect = this->m_cameraL->getCurrentROIRect();
    m_cameraParamsDialog.setDialog(m_cameraL->getMaxWidth(),m_cameraL->getMaxHeight(),cameraRect,m_cameraL->getVBinning(),
                                   m_cameraL->getHBinning(),
                                   m_cameraL->getflipX(),m_cameraL->getflipY());
    connect(&this->m_cameraParamsDialog,SIGNAL(accepted()),SLOT(acceptedCamParamsLeftEvent()));
    m_cameraParamsDialog.setWindowTitle("Camera Left");
    m_cameraParamsDialog.show();
}

/* Private slot acceptedCamParamsRightEvent
 * -------------------------------
 * slot called when the camera params dialog is accepted
 * this function will change the parameters to the returned by the dialog
*/
void MainWindow::acceptedCamParamsLeftEvent()
{
    this->m_cameraL->stopGrabbing();
    Rect cameraRect = m_cameraParamsDialog.getcurrentRect();

    this->m_cameraL->setROIRect(cameraRect);
    this->m_cameraL->setBinning(m_cameraParamsDialog.getHBinning(),m_cameraParamsDialog.getVBinning());
    this->m_cameraL->setflipXOutput(m_cameraParamsDialog.getXFlip());
    this->m_cameraL->setflipYOutput(m_cameraParamsDialog.getYFlip());

    this->m_cameraL->startGrabbing();

    this->m_cameraParamsDialog.disconnect();

    if(m_calibParams_loaded)
    {
        Rect roiR = m_cameraR->getCurrentROIRect();
        this->m_cameraR->initUndistortMap(Size(roiR.width,roiR.height));

        Rect roiL = m_cameraL->getCurrentROIRect();
        this->m_cameraL->initUndistortMap(Size(roiL.width,roiL.height));

        this->m_stereoCalib.initUndistortImage(Size(roiL.width,roiL.height));
    }
}


/* Private slot on_switchCamera_pushButton_clicked
 * -------------------------------
 * slot called when the user wants to interchange the cameras
 * cameraL -> cameraR
 * cameraR -> cameraL
*/
void MainWindow::on_switchCamera_pushButton_clicked()
{
    Camera* temp = NULL;
    temp = m_cameraL;
    m_cameraL = m_cameraR;
    m_cameraR = temp;

    qDebug() <<"WARNING! Stereo undistort will not work!\n";
}

void MainWindow::processDisparity(QImage* Im1,QImage* Im2)
{
    QImage I1 = Im1->convertToFormat(QImage::Format_Grayscale8);
    QImage I2 = Im2->convertToFormat(QImage::Format_Grayscale8);

    // check for correct size
     if (I1.width()<=0 || I1.height() <=0 || I2.width()<=0 || I2.height() <=0 ||
         I1.width()!=I2.width() || I1.height()!=I2.height()) {
       qDebug() << "ERROR: Images must be of same size, but" << endl;
       qDebug() << "       I1: " << I1.width() <<  " x " << I1.height() <<
                    ", I2: " << I2.width() <<  " x " << I2.height() << endl;
       return;
     }

    // get image width and height
     int32_t width  = I1.width();
     int32_t height = I1.height();


    // allocate memory for disparity images
    const int32_t dims[3] = {width,height,width}; // bytes per line = width
    float* D1_data = (float*)malloc(width*height*sizeof(float));
    float* D2_data = (float*)malloc(width*height*sizeof(float));

    // process

    Elas::parameters param;
    param.postprocess_only_left = true;
    //Elas elas(param);

    Elas elas(Elas::setting::MIDDLEBURY);
    elas.process(I1.bits(),I2.bits(),D1_data,D2_data,dims);

    // find maximum disparity for scaling output disparity images to [0..255]
    float disp_max = 0;
    for (int32_t i=0; i<width*height; i++) {
      if (D1_data[i]>disp_max) disp_max = D1_data[i];
      if (D2_data[i]>disp_max) disp_max = D2_data[i];
    }

    QImage D1((int)width,(int)height,QImage::Format_Grayscale8);
    QImage D2((int)width,(int)height,QImage::Format_Grayscale8);

    for (int32_t i=0; i<width*height; i++) {
      D1.bits()[i] = (uint8_t)max(255.0*D1_data[i]/disp_max,0.0);
      D2.bits()[i] = (uint8_t)max(255.0*D2_data[i]/disp_max,0.0);
    }

    ui->label_display1->setPixmap(QPixmap::fromImage(D1));
    ui->label_display1->show();
    ui->label_display2->setPixmap(QPixmap::fromImage(D2));
    ui->label_display2->show();

    free(D1_data);
    free(D2_data);
}

void saveVideo(Mat im1,Mat im2)
{


}



























