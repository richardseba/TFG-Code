#include "vrfullscreenviewer.h"
//#include "qtfilter.h"

#include "imageGeneratorSrc/videoimagegenerator.h"
#include "imageGeneratorSrc/cameraimagegenerator.h"
#include "imageGeneratorSrc/imageloadergenerator.h"

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
VrFullscreenViewer::VrFullscreenViewer(Camera* cameraL,Camera* cameraR, StereoCalibration stereoCalib)
{
    QDesktopWidget* desk = QApplication::desktop();
    this->setAttribute(Qt::WA_DeleteOnClose);

    m_mean = 1.0;
    m_currentUserParam = 1;

    m_doTransitions = false;
    m_isProcessing = false;
    m_currentDistance = Distance(2);

    m_cameraL = cameraL;
    m_cameraR = cameraR;

    m_depthProcess = new DepthProcessing(stereoCalib,12,4,10,4);

//    QtFilter* tmpfilter = new QtFilter();
    this->m_timer = new QTimer(this);
//    this->m_timer->installEventFilter(tmpfilter);
//    this->installEventFilter(tmpfilter);

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(frameUpdateEvent()));

    this->m_useUndistort = false;

    this->initScene();
//    this->setRenderHint( QPainter::SmoothPixmapTransform);

    m_imgGeneratorL = new CameraImageGenerator(cameraL, this->m_useUndistort);
    m_imgGeneratorR = new CameraImageGenerator(cameraR, this->m_useUndistort);

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
    delete m_imgGeneratorL;
    delete m_imgGeneratorR;

    delete m_depthProcess;

    if(this->m_timer->isActive())
        this->m_timer->stop();
    delete this->m_timer;

    delete m_fpsCounter;
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

    QFont panelFont("Helvetica [Cronyx]",25,12,false );

    m_fpsCounter = new QGraphicsTextItemVR("## FPS",0,0,0,0,panelFont);

    m_fpsCounter->setPos(600,450);
//    m_fpsCounter->setOffset(imageWidth,0);
//    this->scene()->addItem(m_fpsCounter);
}

/* Function frameTimeEvent
 * -------------------------------
 * this function will be called every time that m_timer expires.
 * will grab the lastest image from the image updater and will call
 * the scene updater
*/
void VrFullscreenViewer::frameUpdateEvent()
{
//    qDebug() << "starting main loop";
//    qDebug() << crono.restart();
    QRect leftrect = QRect(m_leftSensorROI.x,m_leftSensorROI.y,m_leftSensorROI.width, m_leftSensorROI.height);
    QRect rightrect = QRect(m_rightSensorROI.x,m_rightSensorROI.y,m_rightSensorROI.width, m_rightSensorROI.height);

    QImagePair image;
    image.l = this->m_imgGeneratorL->getFrame().copy();
    image.r = this->m_imgGeneratorR->getFrame().copy();

    if(!image.l.isNull() && !image.r.isNull())
    {
        QImagePair cut;
        cut.l = image.l.copy(leftrect);
        cut.r = image.r.copy(rightrect);

        this->m_frameL.setPixmap(QPixmap::fromImage(cut.l));
        this->m_frameR.setPixmap(QPixmap::fromImage(cut.r));

        if(m_isProcessing)
        {
            m_depthProcess->setImages2Process(image, Size(1100,1100));
            Distance newDistance = m_depthProcess->getCurrentDistance();
            if(newDistance != m_currentDistance) {
                m_currentDistance = newDistance;
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

//    qDebug()  << crono.restart();
//    m_mean = (this->m_videoImageGenL->getCurrentFps()+this->m_videoImageGenR->getCurrentFps()+m_mean)/3.0;
//    this->m_fpsCounter->setText(QString("FPS: ") + QString::number((int)m_mean));
//    this->m_fpsCounter->setText(QString("Time: ") + QString::number((int)elapsed) + " " + QString::number(m_currentDistance) );
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
}

void VrFullscreenViewer::loadUserParameters(QString filename,bool transition)
{
    FileStorage fs(filename.toStdString(), FileStorage::READ);

    Rect leftRect, rightRect;
    int LX, LY, LW, LH, RX, RY, RW, RH;
    fs["LeftSensorROI_X"] >> LX;
    fs["LeftSensorROI_Y"] >> LY;
    fs["LeftSensorROI_Width"] >> LW;
    fs["LeftSensorROI_Height"] >> LH;
    fs["RightSensorROI_X"] >> RX;
    fs["RightSensorROI_Y"] >> RY;
    fs["RightSensorROI_Width"] >> RW;
    fs["RightSensorROI_Height"] >> RH;

    leftRect.x = LX;
    leftRect.y = LY;
    leftRect.height = LH;
    leftRect.width = LW;

    rightRect.x = RX;
    rightRect.y = RY;
    rightRect.height = RH;
    rightRect.width = RW;

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
    default:
        break;
    }
    this->m_frameR.setPos(m_leftSensorROI.width,0);
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

void VrFullscreenViewer::setUpCameraVisualization(Camera* camL, Camera* camR)
{
    if(m_mode != CAMERA) {
        m_mode = CAMERA;
    }

    m_imgGeneratorL->stop();
    m_imgGeneratorR->stop();

    if(m_imgGeneratorL != nullptr){
        delete m_imgGeneratorL;
    }
    if(m_imgGeneratorR != nullptr){
        delete m_imgGeneratorR;
    }

    m_imgGeneratorL = new CameraImageGenerator(camL);
    m_imgGeneratorR = new CameraImageGenerator(camR);

    m_imgGeneratorL->start();
    m_imgGeneratorR->start();
}

























