#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDebug"

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
    this->m_cameraR->initCamParametersFromYALM("./camRconfig.yml");
    this->m_cameraL->initCamParametersFromYALM("./camLconfig.yml");

    this->m_calibParams_loaded&= this->m_cameraL->initCalibParams("./cameraLmatrix.txt","./cameraRdist.txt");
    this->m_calibParams_loaded&= this->m_cameraR->initCalibParams("./cameraRmatrix.txt","./cameraLdist.txt");

    ui->checkBox_undistort->setEnabled(this->m_calibParams_loaded);
    ui->checkBox_undistort->setChecked(false);

    this->m_timer = new QTimer(this);
    this->m_loadCalibDialog = new LoadCalibParamsDialog(this);

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(frameTimeEvent()));
    connect(this->m_loadCalibDialog,SIGNAL(accepted()),SLOT(acceptedCalibParamsEvent()));

    if(this->m_calibParams_loaded)
    {
        this->m_cameraR->initUndistortMap(Size(MAP_WIDTH,MAP_HIGHT));
        this->m_cameraL->initUndistortMap(Size(MAP_WIDTH,MAP_HIGHT));
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
 * Raise a messagebox warning when the camera doesn't grab an image.
*/
void MainWindow::on_captureButton_clicked()
{
    QImage *qImage;
    bool ret;
    if(ui->checkBox_capture_left->isChecked())
    {
        qImage = this->m_cameraL->single_grab_image(ret);

        if(ret)
        {
            QImage copy;
            if(ui->checkBox_undistort->isChecked())
            {
                copy = this->m_cameraL->undistortMapImage(*qImage, CV_INTER_LINEAR);
                ui->label_display1->setPixmap(QPixmap::fromImage(copy));
                if(ui->checkBox_save->isChecked())
                    this->saveImage(copy);
            }
            else
            {
                ui->label_display1->setPixmap(QPixmap::fromImage(*qImage));                
                if(ui->checkBox_save->isChecked())
                    this->saveImage(*qImage);
            }

            ui->label_display1->show();

            delete[] qImage->bits();
            delete qImage;
            qImage = NULL;
        }
        else
        {
            QMessageBox Msgbox;
            Msgbox.setIcon(Msgbox.Information);
            Msgbox.setText("<big>Warning</big> <p>\n\n No image recieved</p>");
            Msgbox.exec();
        }
    }

    if(ui->checkBox_capture_right->isChecked())
    {
        qImage = this->m_cameraR->single_grab_image(ret);
        if(ret)
        {
            QImage copy;
            if(ui->checkBox_undistort->isChecked())
            {
                copy = this->m_cameraR->undistortMapImage(*qImage, CV_INTER_LINEAR);
                ui->label_display2->setPixmap(QPixmap::fromImage(copy));
                if(ui->checkBox_save->isChecked())
                    this->saveImage(copy);
            }
            else
            {
                ui->label_display2->setPixmap(QPixmap::fromImage(*qImage));
                if(ui->checkBox_save->isChecked())
                    this->saveImage(*qImage);
            }

            ui->label_display2->show();

            delete[] qImage->bits();
            delete qImage;
            qImage = NULL;
        }
        else
        {
            QMessageBox Msgbox;
            Msgbox.setIcon(Msgbox.Information);
            Msgbox.setText("<big>Warning</big> <p>\n\n No image recieved</p>");
            Msgbox.exec();
        }
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
        this->m_cameraR->stopGrabbing();
        this->m_cameraL->stopGrabbing();

        ui->recordingButton->setText("Start");
        this->m_is_recording = false;
        this->m_timer->stop();
        ui->groupBox_capture->setEnabled(true);
        ui->pushButton_Fullscreen->setEnabled(true);
        ui->pushButton_VR->setEnabled(true);
    }
    else
    {
        this->m_cameraR->startGrabbing();
        this->m_cameraL->startGrabbing();

        qDebug() << this->m_cameraL->isGrabbing();

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
 * it grabs and updates the assigned label.
*/
void MainWindow::frameTimeEvent()
{
    QImage *qImage = NULL;
    bool ret;
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
                    "",
                    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
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
    this->m_calibDialog.show();
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

/* Private slot acceptedCalibParamsEvent
 * -------------------------------
 * function called when the load calibration dialog is accepted.
 * it will load the new calibration parameters.
*/
void MainWindow::acceptedCalibParamsEvent()
{
    this->m_cameraL->initCalibParams(this->m_loadCalibDialog->getCamR_matrix_path(),
                                                  this->m_loadCalibDialog->getCamR_distorsion_path());

    this->m_cameraR->initCalibParams(this->m_loadCalibDialog->getCamL_matrix_path(),
                                                  this->m_loadCalibDialog->getCamL_distorsion_path());

    bool initialized = this->m_cameraR->getIsinitUndistort()&&this->m_cameraL->getIsinitUndistort();

    if(initialized)
    {
        this->m_cameraR->initUndistortMap(Size(MAP_WIDTH,MAP_HIGHT));
        this->m_cameraL->initUndistortMap(Size(MAP_WIDTH,MAP_HIGHT));
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
        this->m_screen = new VrFullscreenViewer(this->m_cameraR,this->m_cameraL);
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
}


















