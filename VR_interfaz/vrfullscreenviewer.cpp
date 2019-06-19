#include "vrfullscreenviewer.h"

#include "imageGeneratorSrc/videoimagegenerator.h"
#include "imageGeneratorSrc/cameraimagegenerator.h"
#include "imageGeneratorSrc/imageloadergenerator.h"
#include "histogramimage.h"

#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>

/* Function VrFullscreenViewer
 * -------------------------------
 * build in constructor.
*/
VrFullscreenViewer::VrFullscreenViewer()
{
    m_currentUserParam=1;

    m_mode = NONE;

    m_doTransitions = false;
    m_isProcessing = false;
    m_currentDistance = Distance(2);

    this->m_timer = new QTimer(this);
    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(frameUpdateEvent()));

    this->m_timer->start(0);
}

/* Function VrFullscreenViewer
 * -------------------------------
 * overloaded constructor that will set up window and
 * the timer used to update the scene, also calls to the initTimeEvent function
 *
 * NOTE: the setAtributte function is used to set the WA_DeleteOnClose flag on True.
 * This flag will cause the delete of this widget if the close() function is called.
*/
VrFullscreenViewer::VrFullscreenViewer(Camera* cameraL,Camera* cameraR, StereoCalibration stereoCalib, Camera* cameraC)
{
    QDesktopWidget* desk = QApplication::desktop();
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setInteractive(false);
    this->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    this->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    this->setViewport(new QOpenGLWidget);

    m_mean = 1.0;
    m_currentUserParam = 1;

    m_doTransitions = false;
    m_isProcessing = false;
    m_currentDistance = Distance(2);

    m_cameraL = cameraL;
    m_cameraR = cameraR;
    m_cameraC = cameraC;
    if(m_cameraC != nullptr && m_cameraC->isCameraConnected())
        m_cameraCTest = 0; //by default is -1 (will disable the functionality)

    m_depthProcess = new DepthProcessing(stereoCalib,12,4,10,4);

//    QtFilter* tmpfilter = new QtFilter();
    this->m_timer = new QTimer(this);
//    this->m_timer->installEventFilter(tmpfilter);
//    this->installEventFilter(tmpfilter);

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(frameUpdateEvent()));

    this->m_useUndistort = false;

    this->initScene();
//    this->setRenderHint( QPainter::SmoothPixmapTransform);

    m_imgGeneratorL = new CameraImageGenerator(m_cameraL, this->m_useUndistort);
    m_imgGeneratorR = new CameraImageGenerator(m_cameraR, this->m_useUndistort);
    if(cameraC != nullptr && m_cameraC->isCameraConnected()) {
        m_imgGeneratorC = new CameraImageGenerator(m_cameraC);
        m_IRworker = new IRimageGenerator(m_imgGeneratorC);
    }
    m_mode = CAMERA;

    m_imgGeneratorL->start();
    m_imgGeneratorR->start();

    this->m_timer->start();
}


/* Function ~VrFullscreenViewer
 * -------------------------------
 * build-in destructor
*/
VrFullscreenViewer::~VrFullscreenViewer()
{
    m_imgGeneratorL->stop();
    m_imgGeneratorR->stop();

    delete m_imgGeneratorL;
    delete m_imgGeneratorR;

    if(m_cameraC != nullptr && m_cameraC->isCameraConnected()) {
        m_IRworker->stop();
        m_imgGeneratorC->stop();
        delete m_IRworker;
        delete m_imgGeneratorC;
    }

    delete m_depthProcess;

    if(this->m_timer->isActive())
        this->m_timer->stop();
    delete this->m_timer;
}


/* Function initScene
 * -------------------------------
 * function used to initialize the scene. Set up the scene objects
 * positions and the transitions modules
*/
void VrFullscreenViewer::initScene()
{
    this->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    this->setStyleSheet("border: 0px solid black");

    m_currentUserParam = 1;
    loadUserParameters("./configFiles/UserParam1.yml");

    this->m_scene.setSceneRect(0,0, m_leftSensorROI.width+m_rightSensorROI.width ,
                               max(m_leftSensorROI.height,m_rightSensorROI.height));
    this->m_frameL.setPos(0,0);
    this->m_frameR.setPos(m_leftSensorROI.width,0);

    this->setScene(&this->m_scene);

    //adding items to the scene
    this->scene()->addItem(&this->m_frameR);
    this->scene()->addItem(&this->m_frameL);
    this->scene()->addItem(m_rightChart);
    this->scene()->addItem(m_leftChart);

    m_rightChart->setGeometry(1000,0,400,300);
    m_leftChart->setGeometry(0,0,400,300);
    m_rightChart->legend()->hide();
    m_leftChart->legend()->hide();
    m_rightChart->setTheme(QChart::ChartThemeDark);
    m_leftChart->setTheme(QChart::ChartThemeDark);
    m_rightChart->setBackgroundVisible(false);
    m_leftChart->setBackgroundVisible(false);
    m_rightChart->setMargins(QMargins(0,5,0,5));
    m_leftChart->setMargins(QMargins(0,5,0,5));
}

/* Function frameTimeEvent
 * -------------------------------
 * this function will be called every time that m_timer expires.
 * will grab the lastest image from the image updater and will call
 * the scene updater
*/
void VrFullscreenViewer::frameUpdateEvent()
{
    m_crono.restart();
    //launching two parallel task to retrieve the images from the worker
    std::future<QImage> futureImageL( std::async([](ImageGenerator* igGen) { return igGen->getFrame();} , m_imgGeneratorL));
    std::future<QImage> futureImageR( std::async([](ImageGenerator* igGen) { return igGen->getFrame();} , m_imgGeneratorR));

    QRect leftrect = QRect(m_leftSensorROI.x,m_leftSensorROI.y,m_leftSensorROI.width, m_leftSensorROI.height);
    QRect rightrect = QRect(m_rightSensorROI.x,m_rightSensorROI.y,m_rightSensorROI.width, m_rightSensorROI.height);

    QImagePair image;
    image.l = futureImageL.get();
    image.r = futureImageR.get();

    if(!image.l.isNull() && !image.r.isNull())
    {
        std::future<QImage> futureCutL( std::async([](QImage* img, QRect rect) { return img->copy(rect);} , &image.l, leftrect));
        std::future<QImage> futureCutR( std::async([](QImage* img, QRect rect) { return img->copy(rect);} , &image.r, rightrect));

        QImagePair cut;
        cut.l = futureCutL.get();
        cut.r = futureCutR.get();

        if(m_histogramOn) {
            HistogramImage::updateHistogram(m_rightChart,cut.r, 64);
            HistogramImage::updateHistogram(m_leftChart,cut.l, 64);
            cut.l = cut.l.rgbSwapped();
            cut.r = cut.r.rgbSwapped();
        }

        if(m_thirdCameraMix){

            QImage maskC = m_IRworker->getFrame();

            QPoint positionL = QPoint(m_centerImageOverL.x()-leftrect.x(),m_centerImageOverL.y()-leftrect.y());
            QPoint positionR = QPoint(m_centerImageOverR.x()-rightrect.x(),m_centerImageOverR.y()-rightrect.y());

            std::future<void> mixL = std::async(launch::async,mixImages,&cut.l,maskC,positionL);
            std::future<void> mixR = std::async(launch::async,mixImages,&cut.r,maskC,positionR);

            mixL.wait();
            mixR.wait();
        }

//        std::future<void> futureSetFrameL( std::async([&]() { m_frameL.setPixmap(QPixmap::fromImage(cut.l));}));
//        std::future<void> futureSetFrameR( std::async([&]() { m_frameR.setPixmap(QPixmap::fromImage(cut.r));}));

//        futureSetFrameL.wait();
//        futureSetFrameR.wait();

        std::future<QPixmap> futureGetPixmapL( std::async([](QImage img) { return QPixmap::fromImage(img);} , cut.l));
        std::future<QPixmap> futureGetPixmapR( std::async([](QImage img) { return QPixmap::fromImage(img);} , cut.r));

        this->m_frameL.setPixmap(futureGetPixmapL.get());
        this->m_frameR.setPixmap(futureGetPixmapR.get());

//        this->m_frameL.setPixmap(QPixmap::fromImage(cut.l));
//        this->m_frameR.setPixmap(QPixmap::fromImage(cut.r));

        if(m_isProcessing)
        {
            m_depthProcess->setImages2Process(image, Size(1100,1100));
            Distance newDistance = m_depthProcess->getCurrentDistance();
            if(newDistance != m_currentDistance) {
                m_currentDistance = newDistance;
                switchDistance();
            }
        }

    }

    //update the movement in the ROI, if any.
    this->m_transitionLeft.step();
    this->m_transitionRight.step();

    this->m_frameL.setPos(0,0);
    this->m_frameR.setPos(m_leftSensorROI.width,0);

    this->m_scene.setSceneRect(0,0, m_leftSensorROI.width+m_rightSensorROI.width ,
                               max(m_leftSensorROI.height,m_rightSensorROI.height));

    this->fitInView(this->sceneRect(),Qt::KeepAspectRatio);

    qDebug()  << m_crono.restart();
}





void VrFullscreenViewer::switchDistance()
{
    switch (m_currentDistance)
    {
    case 2 :
        m_currentUserParam = 1;
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        loadUserParameters("./configFiles/UserParam1.yml",m_doTransitions);
        break;
    case Distance::MEDIUM:
        m_currentUserParam = 2;
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        loadUserParameters("./configFiles/UserParam2.yml",m_doTransitions);
        break;
    case Distance::CLOSE:
        m_currentUserParam = 3;
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        loadUserParameters("./configFiles/UserParam3.yml",m_doTransitions);
        break;
    }
}

/* Function showFullScreen
 * -------------------------------
 * this function overloads the inherited showFullScreen method to allow
 * us to select the screen where the widget is going to be displayed.
*/
void VrFullscreenViewer::showFullScreen(int screenSelector)
{
    QDesktopWidget* desk = QApplication::desktop();
    QRect geometry = desk->availableGeometry(screenSelector);
    move(geometry.topLeft());

    QGraphicsView::showFullScreen();

}

void VrFullscreenViewer::saveUserParameters(QString filename, QString nameSufix)
{
    filename += nameSufix;
    FileStorage fs (filename.toStdString(), FileStorage::WRITE);
    fs << "LeftSensorROI_X" << m_leftSensorROI.x;
    fs << "LeftSensorROI_Y" << m_leftSensorROI.y;
    fs << "LeftSensorROI_Width" << m_leftSensorROI.width;
    fs << "LeftSensorROI_Height" << m_leftSensorROI.height;

    fs << "RightSensorROI_X" << m_rightSensorROI.x;
    fs << "RightSensorROI_Y" << m_rightSensorROI.y;
    fs << "RightSensorROI_Width" << m_rightSensorROI.width;
    fs << "RightSensorROI_Height" << m_rightSensorROI.height;

    fs << "IRimageoverL_X" << m_centerImageOverL.x();
    fs << "IRimageoverL_Y" << m_centerImageOverL.y();

    fs << "IRimageoverR_X" << m_centerImageOverR.x();
    fs << "IRimageoverR_Y" << m_centerImageOverR.y();
}

void VrFullscreenViewer::loadUserParameters(QString filename,bool transition)
{
    FileStorage fs(filename.toStdString(), FileStorage::READ);

    Rect leftRect, rightRect;
    int LX, LY, LW, LH, RX, RY, RW, RH, IR_imgL_X, IR_imgL_Y, IR_imgR_X, IR_imgR_Y;
    fs["LeftSensorROI_X"] >> LX;
    fs["LeftSensorROI_Y"] >> LY;
    fs["LeftSensorROI_Width"] >> LW;
    fs["LeftSensorROI_Height"] >> LH;
    fs["RightSensorROI_X"] >> RX;
    fs["RightSensorROI_Y"] >> RY;
    fs["RightSensorROI_Width"] >> RW;
    fs["RightSensorROI_Height"] >> RH;

    fs["IRimageoverL_X"] >> IR_imgL_X;
    fs["IRimageoverL_Y"] >> IR_imgL_Y;

    fs["IRimageoverR_X"] >> IR_imgR_X;
    fs["IRimageoverR_Y"] >> IR_imgR_Y;

    leftRect.x = LX;
    leftRect.y = LY;
    leftRect.height = LH;
    leftRect.width = LW;

    rightRect.x = RX;
    rightRect.y = RY;
    rightRect.height = RH;
    rightRect.width = RW;

    m_centerImageOverL.setX(IR_imgL_X);
    m_centerImageOverL.setY(IR_imgL_Y);
    m_centerImageOverR.setX(IR_imgR_X);
    m_centerImageOverR.setY(IR_imgR_Y);


    if(!transition)
    {
        m_leftSensorROI = leftRect;
        m_rightSensorROI = rightRect;
    } else {
        m_transitionLeft = ROITransition(&m_leftSensorROI);
        m_transitionRight = ROITransition(&m_rightSensorROI);
        m_transitionLeft.setTarget(leftRect,STEPS_IN_TRANSITION);
        m_transitionRight.setTarget(rightRect,STEPS_IN_TRANSITION);
    }
}

void VrFullscreenViewer::zoomIn()
{
    m_leftSensorROI.x += 9;
    m_leftSensorROI.y += 8;

    m_leftSensorROI.height -= 18;
    m_leftSensorROI.width -= 16;

    m_rightSensorROI.x += 9;
    m_rightSensorROI.y += 8;

    m_rightSensorROI.height -= 18;
    m_rightSensorROI.width -= 16;

//    this->m_frameL.setPos(0,0);
//    this->m_frameR.setPos(m_leftSensorROI.width,0);
}
void VrFullscreenViewer::zoomOut()
{
    m_leftSensorROI.x -= 9;
    m_leftSensorROI.y -= 8;

    m_leftSensorROI.height += 18;
    m_leftSensorROI.width += 16;

    m_rightSensorROI.x -= 9;
    m_rightSensorROI.y -= 8;

    m_rightSensorROI.height += 18;
    m_rightSensorROI.width += 16;

//    this->m_frameL.setPos(0,0);
//    this->m_frameR.setPos(m_leftSensorROI.width,0);
}

/* Function keyPressEvent
 * -------------------------------
 * function called when a key is pressed
 * if the plus key is press, the images of the viewer will zoom in
 * if the minus key is press, the images of the viewer will zoom out
 * if the key esc is press, the timer will stop and the widget will be closed.
 * if the key U is press, the undistort bool will change to the negate state.
 * the WASD key let you move the right frame
 * the arrow keys let you move the left interface
*/
void VrFullscreenViewer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Plus:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        zoomIn();
        break;
    case Qt::Key_Minus:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        zoomOut();
        break;
    case Qt::Key_Escape:
        if(this->m_timer->isActive())
            this->m_timer->stop();
        this->close();
        break;
    case Qt::Key_U:
        //TODO: Enable the use of this feature after the changes done to the classes
        //Use or not undistort in the images outputed by the camera
        m_useUndistort = !m_useUndistort;
//        this->m_camImageGenL->setUndistortImages(this->m_useUndistort);
//        this->m_camImageGenR->setUndistortImages(this->m_useUndistort);
        break;
    //Key events to move the window of the left camera - WASD keys
    case Qt::Key_W:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        m_leftSensorROI.y -= 6;
        break;
    case Qt::Key_A:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        m_leftSensorROI.x -= 6;
        break;
    case Qt::Key_S:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        m_leftSensorROI.y += 6;
        break;
    case Qt::Key_D:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        m_leftSensorROI.x += 6;
        break;
    //Key events to move the window of the right camera - Arrow Keys
    case Qt::Key_Up:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        m_rightSensorROI.y -= 6;
        break;
    case Qt::Key_Down:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        m_rightSensorROI.y += 6;
        break;
    case Qt::Key_Left:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        m_rightSensorROI.x -= 6;
        break;
    case Qt::Key_Right:
        m_transitionLeft.cancelTransition();
        m_transitionRight.cancelTransition();
        m_rightSensorROI.x += 6;
        break;
    case Qt::Key_T:
        m_doTransitions = !m_doTransitions;
        break;
    case Qt::Key_P:
        if(!m_isProcessing)
            m_depthProcess->start();
        else
            m_depthProcess->stop();
        m_isProcessing = !m_isProcessing;
        break;
    case Qt::Key_C:
        if(m_cameraCTest >= 0) rotateCameraVisualization();
        break;
    case Qt::Key_H:
        m_histogramOn = !m_histogramOn;
        break;
    case Qt::Key_I:
        if(m_cameraC != nullptr && m_cameraC->isCameraConnected()){
            m_thirdCameraMix = !m_thirdCameraMix;
            if(m_thirdCameraMix)
                m_IRworker->start();
            else
                m_IRworker->stop();
        }
        break;
    //Key events to change de user configuration
    case Qt::Key_1:
        if(m_currentUserParam == 1){
            saveUserParameters("./configFiles/UserParam1.yml");
            qDebug() << "saved 1";
        } else {
            m_currentUserParam = 1;
            m_transitionLeft.cancelTransition();
            m_transitionRight.cancelTransition();
            loadUserParameters("./configFiles/UserParam1.yml",m_doTransitions);
            qDebug() << "loaded 1";
        }
        break;
    case Qt::Key_2:
        if(m_currentUserParam == 2) {
            saveUserParameters("./configFiles/UserParam2.yml");
            qDebug() << "saved 2 ";
        } else {
            m_currentUserParam = 2;
            m_transitionLeft.cancelTransition();
            m_transitionRight.cancelTransition();
            loadUserParameters("./configFiles/UserParam2.yml",m_doTransitions);
            qDebug() << "loaded 2";
        }
        break;
    case Qt::Key_3:
        if(m_currentUserParam == 3) {
            saveUserParameters("./configFiles/UserParam3.yml");
            qDebug() << "saved 3";
        } else {
            m_currentUserParam = 3;
            m_transitionLeft.cancelTransition();
            m_transitionRight.cancelTransition();
            loadUserParameters("./configFiles/UserParam3.yml",m_doTransitions);
            qDebug() << "loaded 3";
        }
        break;
    case Qt::Key_4:
        if(m_currentUserParam == 4)
            saveUserParameters("./configFiles/UserParam4.yml");
        else
        {
            m_currentUserParam = 4;
            m_transitionLeft.cancelTransition();
            m_transitionRight.cancelTransition();
            loadUserParameters("./configFiles/UserParam4.yml",m_doTransitions);
        }
        break;
    case Qt::Key_6:
        setUpVideo((char*)"./videos/1L.avi",(char*)"./videos/1R.avi");
//        Sleep(5000);
        break;
    case Qt::Key_7:
        setUpCameraVisualization(m_cameraL,m_cameraR);
        break;
    case Qt::Key_8:
        setUpStillImages((char*)"./demo_images/im1L.png",(char*)"./demo_images/im1R.png");
        break;
    case Qt::Key_9:
        setUpStillImages((char*)"./demo_images/im2L.png",(char*)"./demo_images/im2R.png");
        break;
    case Qt::Key_0:
        setUpStillImages((char*)"./demo_images/im3L.png",(char*)"./demo_images/im3R.png");
        break;
    case Qt::Key_O:
        if(m_centerImagePointMode)
            m_centerImageOverL.setY(m_centerImageOverL.y()-6);
        else
            m_centerImageOverR.setY(m_centerImageOverR.y()-6);
        break;
    case Qt::Key_Ntilde:
        if(m_centerImagePointMode)
            m_centerImageOverL.setX(m_centerImageOverL.x()+6);
        else
            m_centerImageOverR.setX(m_centerImageOverR.x()+6);
        break;
    case Qt::Key_K:
        if(m_centerImagePointMode)
            m_centerImageOverL.setX(m_centerImageOverL.x()-6);
        else
            m_centerImageOverR.setX(m_centerImageOverR.x()-6);
        break;
    case Qt::Key_L:
        if(m_centerImagePointMode)
            m_centerImageOverL.setY(m_centerImageOverL.y()+6);
        else
            m_centerImageOverR.setY(m_centerImageOverR.y()+6);
        break;
    case Qt::Key_Shift:
        m_centerImagePointMode = !m_centerImagePointMode;
        break;
    default:
        break;
    }
    this->m_frameR.setPos(m_leftSensorROI.width,0);
}

void VrFullscreenViewer::rotateCameraVisualization(){
    m_cameraCTest = (m_cameraCTest+1)%3;
    switch (m_cameraCTest)
    {
        case 0:
            setUpCameraVisualization(m_cameraL,m_cameraR);
        break;
        case 1:
            setUpCameraVisualization(m_cameraC,m_cameraR);
        break;
        case 2:
            setUpCameraVisualization(m_cameraL,m_cameraC);
        break;
    }
}

void VrFullscreenViewer::setUpStillImages(char* nameFileL,char* nameFileR)
{
    if(m_mode != STILL_IMG) {
        m_mode = STILL_IMG;
    }

    m_imgGeneratorL->stop();
    m_imgGeneratorR->stop();

    if(m_imgGeneratorL != nullptr){
        delete m_imgGeneratorL;
    }
    if(m_imgGeneratorR != nullptr){
        delete m_imgGeneratorR;
    }

    m_imgGeneratorL = new ImageLoaderGenerator(nameFileL);
    m_imgGeneratorR = new ImageLoaderGenerator(nameFileR);

    m_imgGeneratorL->start();
    m_imgGeneratorR->start();
}

void VrFullscreenViewer::setUpVideo(char* nameFileL,char* nameFileR)
{
    if(m_mode != VIDEO) {
        m_mode = VIDEO;
    }

    m_imgGeneratorL->stop();
    m_imgGeneratorR->stop();

    if(m_imgGeneratorL != nullptr){
        delete m_imgGeneratorL;
    }
    if(m_imgGeneratorR != nullptr){
        delete m_imgGeneratorR;
    }

    m_imgGeneratorL = new VideoImageGenerator(nameFileL);
    m_imgGeneratorR = new VideoImageGenerator(nameFileR);

    m_imgGeneratorL->start();
    m_imgGeneratorR->start();
}

void VrFullscreenViewer::setUpCameraVisualization(Camera* t_cam1, Camera* t_cam2)
{
    if(m_mode != CAMERA) {
        m_mode = CAMERA;
    }
    qDebug() << "before stop";
    m_imgGeneratorL->stop();
    m_imgGeneratorR->stop();

    qDebug() << "after stop";
    if(m_imgGeneratorL != nullptr){
        delete m_imgGeneratorL;
    }
    if(m_imgGeneratorR != nullptr){
        delete m_imgGeneratorR;
    }
    qDebug() << "after delete";

    m_imgGeneratorL = new CameraImageGenerator(t_cam1);
    m_imgGeneratorR = new CameraImageGenerator(t_cam2);

    qDebug() << "after new";

    m_imgGeneratorL->start();
    m_imgGeneratorR->start();
}

























