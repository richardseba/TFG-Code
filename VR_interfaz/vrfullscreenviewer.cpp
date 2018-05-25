#include "vrfullscreenviewer.h"


/* Function VrFullscreenViewer
 * -------------------------------
 * build in constructor.
*/
VrFullscreenViewer::VrFullscreenViewer()
{
    m_currentUserParam=1;
    m_isDemo = false;

    this->m_timer = new QTimer(this);
    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(frameUpdateEvent()));

    this->m_timer->start();
}

/* Function VrFullscreenViewer
 * -------------------------------
 * overloaded constructor that will set up window and
 * the timer used to update the scene, also calls to the initTimeEvent function
 *
 * NOTE: the setAtributte function is used to set the WA_DeleteOnClose flag on True.
 * This flag will cause the delete of this widget if the close() function is called.
*/
VrFullscreenViewer::VrFullscreenViewer(Camera* cameraL,Camera* cameraR)
{
    QDesktopWidget* desk = QApplication::desktop();
    this->setAttribute(Qt::WA_DeleteOnClose);

    m_mean = 1.0;
    m_currentUserParam = 1;
    m_currentImage = 1;
    m_isDemo = false;

//    this->m_params.offsetLeftX = 0;
//    this->m_params.offsetLeftY = 0;

    this->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    this->setStyleSheet("border: 0px solid black");

    this->m_timer = new QTimer(this);
    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(frameUpdateEvent()));

    this->m_cameraL = cameraL;
    this->m_cameraR = cameraR;

    this->m_useUndistort = false;

    this->initScene();
}

/* Function ~VrFullscreenViewer
 * -------------------------------
 * build in destructor
*/
VrFullscreenViewer::~VrFullscreenViewer()
{
    emit setUpdatingR(false);
    emit setUpdatingL(false);

    imageUpdaterR->deleteLater();
    imageUpdaterL->deleteLater();

    this->m_threadR.quit();
    this->m_threadL.quit();

    this->m_threadR.wait();
    this->m_threadL.wait();

    if(this->m_timer->isActive())
        this->m_timer->stop();
    delete this->m_timer;

    //delete m_rectangle;
    delete m_fpsCounter;
    qDebug() << "fullscreen closed";
}



/* Function initScene
 * -------------------------------
 * function used to initialize the scene, the viewer updater and the threads
 * and also will start the timers for the viewer updaters
*/
void VrFullscreenViewer::initScene()
{
    bool ret;
    QImage *qImageL;
    QImage *qImageR;

    this->m_cameraL->stopGrabbing();
    this->m_cameraL->setROIRect(cv::Rect(0,0,this->m_cameraL->getMaxWidth(), this->m_cameraL->getMaxHeight()));
    this->m_cameraL->startGrabbing();

    this->m_cameraR->stopGrabbing();
    this->m_cameraR->setROIRect(cv::Rect(0,0,this->m_cameraR->getMaxWidth(), this->m_cameraR->getMaxHeight()));
    this->m_cameraR->startGrabbing();

    qImageL = this->m_cameraL->single_grab_image(ret);
    qImageR = this->m_cameraR->single_grab_image(ret);

    this->m_frameR.setPixmap(QPixmap::fromImage(*qImageR));
    this->m_frameL.setPixmap(QPixmap::fromImage(*qImageL));

    //Setting up the scene
    int imageWidth = this->m_frameR.pixmap().width();
    int imageHeight = this->m_frameR.pixmap().height();

    m_leftSensorROI = Rect(0,0,imageWidth,imageHeight);
    m_rightSensorROI = Rect(0,0,imageWidth,imageHeight);

    m_currentUserParam = 1;
    loadUserParameters("./configFiles/UserParam1.yml");

    this->m_scene.setSceneRect(0,0, m_leftSensorROI.width+m_rightSensorROI.width ,
                               max(m_leftSensorROI.height,m_rightSensorROI.height));
    this->m_frameL.setPos(0,0);
    this->m_frameR.setPos(m_leftSensorROI.width,0);

    this->setScene(&this->m_scene);

    m_splitLine.setLine(m_leftSensorROI.width, 0,m_leftSensorROI.width,
                        max(m_leftSensorROI.height,m_rightSensorROI.height));
    m_splitLine.setPen(QPen(Qt::red));

    //adding items to the scene
    this->scene()->addItem(&this->m_frameR);
    this->scene()->addItem(&this->m_frameL);
    this->scene()->addItem(&m_splitLine);

    QFont panelFont("Helvetica [Cronyx]",25,12,false );

    m_fpsCounter = new QGraphicsTextItemVR("## FPS",0,0,0,0,panelFont);

    m_fpsCounter->setPos(600,450);
    m_fpsCounter->setOffset(imageWidth,0);
    //this->scene()->addItem(m_fpsCounter);

    delete[] qImageL->bits();
    delete qImageL;
    qImageL = NULL;
    delete[] qImageR->bits();
    delete qImageR;
    qImageR = NULL;

    //setting up the threads used to grab the images from the camera
    imageUpdaterR = new VRimageUpdater(m_cameraR, &m_timeR, false, this->m_useUndistort);
    imageUpdaterL = new VRimageUpdater(m_cameraL, &m_timeL, false, this->m_useUndistort);

    connect(&m_timeR, SIGNAL (timeout()), imageUpdaterR, SLOT (frameUpdateEvent()));
    connect(&m_timeL, SIGNAL (timeout()), imageUpdaterL, SLOT (frameUpdateEvent()));

    connect(this,SIGNAL(setUpdatingL(bool)),imageUpdaterL,SLOT(setUpdatingEvent(bool)));
    connect(this,SIGNAL(setUpdatingR(bool)),imageUpdaterR,SLOT(setUpdatingEvent(bool)));

    emit setUpdatingR(true);
    emit setUpdatingL(true);

    this->m_timeR.moveToThread(&this->m_threadR);
    this->m_timeL.moveToThread(&this->m_threadL);

    imageUpdaterR->moveToThread(&this->m_threadR);
    imageUpdaterL->moveToThread(&this->m_threadL);

    m_threadR.start();
    m_threadL.start();

    this->m_timer->start();
}

/* Function frameTimeEvent
 * -------------------------------
 * this function will be called every time that m_timer expires.
 * will grab the lastest image from the image updater and will call
 * the scene updater
*/
void VrFullscreenViewer::frameUpdateEvent()
{
    QRect leftrect = QRect::QRect(m_leftSensorROI.x,m_leftSensorROI.y,m_leftSensorROI.width, m_leftSensorROI.height);
    QRect rightrect = QRect::QRect(m_rightSensorROI.x,m_rightSensorROI.y,m_rightSensorROI.width, m_rightSensorROI.height);

    if(!m_isDemo){
        this->m_frameR.setPixmap(QPixmap::fromImage(this->imageUpdaterR->getNextFrame().copy(rightrect)));
        this->m_frameL.setPixmap(QPixmap::fromImage(this->imageUpdaterL->getNextFrame().copy(leftrect)));
    } else {
        this->m_frameL.setPixmap(QPixmap::fromImage(m_imgL.copy(leftrect)));
        this->m_frameR.setPixmap(QPixmap::fromImage(m_imgR.copy(rightrect)));
    }

    m_mean = (this->imageUpdaterL->getCurrentFPS()+this->imageUpdaterR->getCurrentFPS()+m_mean)/3.0;
    this->m_fpsCounter->setText(QString("FPS: ") + QString::number((int)m_mean));

    this->m_scene.setSceneRect(0,0, m_leftSensorROI.width+m_rightSensorROI.width ,
                               max(m_leftSensorROI.height,m_rightSensorROI.height));

    m_splitLine.setLine(m_leftSensorROI.width, 0,m_leftSensorROI.width,
                        max(m_leftSensorROI.height,m_rightSensorROI.height));

    this->fitInView(this->sceneRect(),Qt::KeepAspectRatio);
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

void VrFullscreenViewer::loadUserParameters(QString filename)
{
    FileStorage fs(filename.toStdString(), FileStorage::READ);

    int LX, LY, LW, LH, RX, RY, RW, RH;
    fs["LeftSensorROI_X"] >> LX;
    fs["LeftSensorROI_Y"] >> LY;
    fs["LeftSensorROI_Width"] >> LW;
    fs["LeftSensorROI_Height"] >> LH;
    fs["RightSensorROI_X"] >> RX;
    fs["RightSensorROI_Y"] >> RY;
    fs["RightSensorROI_Width"] >> RW;
    fs["RightSensorROI_Height"] >> RH;

    m_leftSensorROI.x = LX;
    m_leftSensorROI.y = LY;
    m_leftSensorROI.height = LH;
    m_leftSensorROI.width = LW;

    m_rightSensorROI.x = RX;
    m_rightSensorROI.y = RY;
    m_rightSensorROI.height = RH;
    m_rightSensorROI.width = RW;
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

    this->m_frameL.setPos(0,0);
    this->m_frameR.setPos(m_leftSensorROI.width,0);
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

    this->m_frameL.setPos(0,0);
    this->m_frameR.setPos(m_leftSensorROI.width,0);
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
         zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Escape:
        if(this->m_timer->isActive())
            this->m_timer->stop();
        this->close();
        break;
    case Qt::Key_U:
        //Use or not undistort in the images outputed by the camera
        m_useUndistort = !m_useUndistort;
        this->imageUpdaterR->setIsUndistorted(this->m_useUndistort);
        this->imageUpdaterL->setIsUndistorted(this->m_useUndistort);
        break;
    //Key events to move the window of the left camera - WASD keys
    case Qt::Key_W:
        m_leftSensorROI.y -= 6;
        break;
    case Qt::Key_A:
        m_leftSensorROI.x -= 6;
        break;
    case Qt::Key_S:
        m_leftSensorROI.y += 6;
        break;
    case Qt::Key_D:
        m_leftSensorROI.x += 6;
        break;
    //Key events to move the window of the right camera - Arrow Keys
    case Qt::Key_Up:
        m_rightSensorROI.y -= 6;
        break;
    case Qt::Key_Down:
        m_rightSensorROI.y += 6;
        break;
    case Qt::Key_Left:
        m_rightSensorROI.x -= 6;
        break;
    case Qt::Key_Right:
        m_rightSensorROI.x += 6;
        break;
    //Key events to change de user configuration
    case Qt::Key_1:
        if(m_currentUserParam == 1)
            saveUserParameters("./configFiles/UserParam1.yml");
        else
        {
            m_currentUserParam = 1;
            loadUserParameters("./configFiles/UserParam1.yml");
        }
        break;
    case Qt::Key_2:
        if(m_currentUserParam == 2)
            saveUserParameters("./configFiles/UserParam2.yml");
        else
        {
            m_currentUserParam = 2;
            loadUserParameters("./configFiles/UserParam2.yml");
        }
        break;
    case Qt::Key_3:
        if(m_currentUserParam == 3)
            saveUserParameters("./configFiles/UserParam3.yml");
        else
        {
            m_currentUserParam = 3;
            loadUserParameters("./configFiles/UserParam3.yml");
        }
        break;
    case Qt::Key_4:
        if(m_currentUserParam == 4)
            saveUserParameters("./configFiles/UserParam4.yml");
        else
        {
            m_currentUserParam = 4;
            loadUserParameters("./configFiles/UserParam4.yml");
        }
        break;
    case Qt::Key_8:
        if(m_isDemo && m_currentImage == 1) {
            emit setUpdatingR(true);
            emit setUpdatingL(true);
            m_isDemo = false;
        } else {
            emit setUpdatingR(false);
            emit setUpdatingL(false);
            m_isDemo = true;
            m_currentImage = 1;
        }
        m_imgL = QImage::QImage("./demo_images/im1L.png");
        m_imgR = QImage::QImage("./demo_images/im1R.png");
        break;
    case Qt::Key_9:
        qDebug() << (m_isDemo && m_currentImage == 2);
        if(m_isDemo && m_currentImage == 2) {
            emit setUpdatingR(true);
            emit setUpdatingL(true);
            m_isDemo = false;
        } else {
            emit setUpdatingR(false);
            emit setUpdatingL(false);
            m_isDemo = true;
            m_currentImage = 2;
        }
        m_imgL = QImage::QImage("./demo_images/im2L.png");
        m_imgR = QImage::QImage("./demo_images/im2R.png");
        break;
    case Qt::Key_0:
        if(m_isDemo && m_currentImage == 3) {
            emit setUpdatingR(true);
            emit setUpdatingL(true);
            m_isDemo = false;
        } else {
            emit setUpdatingR(false);
            emit setUpdatingL(false);
            m_isDemo = true;
            m_currentImage = 3;
        }
        m_imgL = QImage::QImage("./demo_images/im3L.png");
        m_imgR = QImage::QImage("./demo_images/im3R.png");
        break;
    default:
        break;
    }
}




























