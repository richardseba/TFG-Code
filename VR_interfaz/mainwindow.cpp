#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QDebug"

#include <QDateTime>
#include <QProgressBar>
#include <QApplication>

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

    this->m_photosCaptured = 0;

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
    m_classifier = ThresholdClassificator(SIZE_OF_CLASSIFIER_BUFFER,12,5);
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

    if(ui->checkBox_capture_right->isChecked()){
        ImgR = this->m_cameraR->single_grab_image(retR);
    }
    if(retL)
    {
        QImage copy;
        if(ui->checkBox_undistort->isChecked())
        {
            copy = Mat2QImage(this->m_stereoCalib.undistortLeft(QImage2Mat(*ImgL), CV_INTER_LINEAR));
            ui->label_display1->setPixmap(QPixmap::fromImage(copy));
            if(ui->checkBox_save->isChecked())
                this->saveImage(copy);
        } else {
            ui->label_display1->setPixmap(QPixmap::fromImage(*ImgL));
            if(ui->checkBox_save->isChecked())
                this->saveImage(*ImgL);
        }

        ui->label_display1->show();

        delete[] ImgL->bits();
        delete ImgL;
        ImgL = NULL;
    } else {
        qDebug() << "no left image recieved";
    }

    if(retR)
    {
        QImage copy;
        if(ui->checkBox_undistort->isChecked())
        {
            copy = Mat2QImage(this->m_stereoCalib.undistortRight(QImage2Mat(*ImgR), CV_INTER_LINEAR));
            ui->label_display2->setPixmap(QPixmap::fromImage(copy));
            if(ui->checkBox_save->isChecked())
                this->saveImage(copy);
        } else {
            ui->label_display2->setPixmap(QPixmap::fromImage(*ImgR));
            if(ui->checkBox_save->isChecked())
                this->saveImage(*ImgR);
        }

        ui->label_display2->show();

        delete[] ImgR->bits();
        delete ImgR;
        ImgR = NULL;
    } else {
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

        if(ui->radioButton_recordMemory->isChecked())
        {
            QProgressBar progress(this);
            progress.setMaximum(m_vectorVideoL.size()+m_vectorVideoR.size());
            qDebug() << m_vectorVideoL.size() << m_vectorVideoR.size();
            progress.show();
            progress.setFixedSize(250,50);
            qDebug() << "a guardar";
            saveVideoFromMemory(m_vectorVideoL, m_videoL,&progress);
            saveVideoFromMemory(m_vectorVideoR, m_videoR,&progress);
            qDebug() << "a despues";
            m_vectorVideoL.clear();
            m_vectorVideoR.clear();
        }
        if(ui->checkBox_saveVideo->isChecked()){
            m_videoL.release();
            m_videoR.release();
        }

        ui->recordingButton->setText("Start");
        this->m_is_recording = false;
        this->m_timer->stop();
        ui->groupBox_capture->setEnabled(true);
        ui->pushButton_Fullscreen->setEnabled(true);
        ui->pushButton_VR->setEnabled(true);
    } else {
        if(ui->checkBox_saveVideo->isChecked()){
            Rect rectL = m_cameraL->getCurrentROIRect();
            Rect rectR = m_cameraR->getCurrentROIRect();

            QDateTime now;
            now = QDateTime::currentDateTime();
            QString namefile = QString("./videos/") + QString(now.toString("dd_MM_hh_mm_ss"));

#ifdef _WIN32
            m_videoL.open((namefile+QString("L.avi")).toLatin1().data(),-1,FRAME_RATE_SAVE, rectL.size());
            m_videoR.open((namefile+QString("R.avi")).toLatin1().data(),-1,FRAME_RATE_SAVE, rectR.size());
#endif

#ifdef __linux__
            qDebug() << "Linux";
            m_videoL.open((namefile+QString("L.avi")).toLatin1().data(),VideoWriter::fourcc('M','J','P','G'),FRAME_RATE_SAVE, rectL.size());
            m_videoR.open((namefile+QString("R.avi")).toLatin1().data(),VideoWriter::fourcc('M','J','P','G'),FRAME_RATE_SAVE, rectR.size());
#endif
            qDebug() << "Videos are opened " << (m_videoL.isOpened() && m_videoR.isOpened());
        }
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
    QImage qImageL;
    QImage qImageR;

    QImage* temp;

    bool left,right;
    left = ui->checkBox_continous_left->isChecked();
    right = ui->checkBox_continous_right->isChecked();

    if(left){
        temp = this->m_cameraL->grab_image(left);
        qImageL = temp->copy();
        if(temp!=NULL){
            delete[] temp->bits();
            delete temp;
            temp = NULL;
        }
    }
    if(right){
        temp = this->m_cameraR->grab_image(right);
        qImageR = temp->copy();
        if(temp!=NULL){
            delete[] temp->bits();
            delete temp;
       }
    }

    if(left && right)
    {
        if(!ui->checkBox_saveVideo->isChecked() && ui->checkBox_getDepthMap->isChecked())
        {
            //runtime processing
            QImage imLeft = qImageL.convertToFormat(QImage::Format_RGB888);
            QImage imRight= qImageR.convertToFormat(QImage::Format_RGB888);
            Mat undisL,undisR ;
            QImage tempL,tempR;
            int downSampling = ui->spinBox_downSampling->value();
            if(downSampling >0){
                Mat subsampL,subsampR;
                Rect outRect = this->m_cameraL->getCurrentROIRect();
                Size outSize(outRect.width/downSampling,outRect.height/downSampling);

                undisL = this->m_stereoCalib.undistortLeft(QImage2Mat(imLeft),CV_INTER_LINEAR);
                undisR = this->m_stereoCalib.undistortRight(QImage2Mat(imRight),CV_INTER_LINEAR);

                cv::resize(undisL,subsampL,outSize,INTER_LINEAR);
                cv::resize(undisR,subsampR,outSize,INTER_LINEAR);

                tempL = Mat2QImage(subsampL).convertToFormat(QImage::Format_Grayscale8);
                tempR = Mat2QImage(subsampR).convertToFormat(QImage::Format_Grayscale8);
            } else {
                tempL = Mat2QImage(this->m_stereoCalib.undistortLeft(QImage2Mat(imLeft),CV_INTER_LINEAR)).convertToFormat(QImage::Format_Grayscale8);
                tempR = Mat2QImage(this->m_stereoCalib.undistortRight(QImage2Mat(imRight),CV_INTER_LINEAR)).convertToFormat(QImage::Format_Grayscale8);
            }
            MatPair dispOut;
            QImagePair dispImOut;
            dispOut = processDisparity(&tempL,&tempR,Elas::setting(ui->spinBox_Libelas_setting->value()));

            dispImOut = postProcessImages(dispOut,ui->checkBox_colormap->isChecked());

            if(ui->checkBox_overLap->isChecked() && ui->checkBox_colormap->isChecked()){
                Mat upSampL, downSampL = QImage2Mat(dispImOut.l.copy()).clone();
                cv::resize(downSampL,upSampL,Size(undisL.size().width,undisL.size().height),INTER_LINEAR);

                Mat mixL = getColorFrom(undisL.clone(),upSampL);

                Mat upSampR, downSampR = QImage2Mat(dispImOut.r.copy()).clone();
                cv::resize(downSampR,upSampR,Size(undisR.size().width,undisR.size().height),INTER_LINEAR);

                Mat mixR = getColorFrom(undisR.clone(),upSampR);

                dispImOut.l = Mat2QImage(mixL);
                dispImOut.r = Mat2QImage(mixR);
            }
            if(ui->checkBox_dynamicVergence->isChecked()){
                cv::Rect centerROI = calculateCenteredROI(dispOut.l.size(),dispOut.l.size().width/downSampling,dispOut.l.size().height/downSampling);

                Mat cutL = Mat(dispOut.l,centerROI);
                Mat cutR = Mat(dispOut.r,centerROI);
                Distance value = m_classifier.calcClasificationProximity(cutL,cutR);
                if(value == CLOSE)
                    qDebug() << "near";
                else if(value == MEDIUM)
                    qDebug() << "medium";
                else
                    qDebug() << "far";
            }

            qImageL = dispImOut.l.copy();
            qImageR = dispImOut.r.copy();

        }
        if(ui->checkBox_saveVideo->isChecked())
        {
            if(ui->radioButton_recordMemory->isChecked() && ((m_vectorVideoL.size()+m_vectorVideoR.size()) < MAX_FRAME_IN_MEMORY)) {
                m_vectorVideoL.push_back(qImageL.copy());
                m_vectorVideoR.push_back(qImageR.copy());
            }else if(((m_vectorVideoL.size()+m_vectorVideoR.size()) >= MAX_FRAME_IN_MEMORY))
                on_recordingButton_clicked();
            if(ui->radioButton_recordDisk->isChecked()){
                Mat im1 = QImage2Mat(qImageL);
                Mat im2 = QImage2Mat(qImageR);
                m_videoL << im1;
                m_videoR << im2;
            }
        }

        float currentfps = 1000.0/m_time->restart();
        m_meanfps = (currentfps+m_meanfps)/2;
//        qDebug() << m_meanfps;

    }
    if(left && !ui->checkBox_saveVideo->isChecked()) {
        ui->label_display1->setPixmap(QPixmap::fromImage(qImageL));
        ui->label_display1->show();
    }
    if(right && !ui->checkBox_saveVideo->isChecked()){
        ui->label_display2->setPixmap(QPixmap::fromImage(qImageR));
        ui->label_display2->show();
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
    QString filename = QString::number(m_photosCaptured/2+1);
    filename.append(".png");
    QString imagePath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save File"),
                    "C:/Users/rsegovia/Desktop/Dataset/1100x1100 22 june/"+filename,
                    tr("PNG (*.png);;JPEG (*.jpg *.jpeg)" )
                    );
    if(!imagePath.isEmpty() && !imagePath.isNull())
        m_photosCaptured++;
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
        this->m_screen = new VrFullscreenViewer(this->m_cameraL,this->m_cameraR,m_stereoCalib);
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


void MainWindow::saveVideoFromMemory(std::vector<QImage> buffer, VideoWriter video, QProgressBar *progress)
{
    for(int i = 0; i < buffer.size(); i++) {
        video << QImage2Mat(buffer[i]);
        progress->setValue(progress->value()+1);
        qApp->processEvents();
        progress->update();
        this->update();
    }
}











