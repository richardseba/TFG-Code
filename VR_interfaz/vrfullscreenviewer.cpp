#include "vrfullscreenviewer.h"

/* Function VrFullscreenViewer
 * -------------------------------
 * build in constructor.
*/
VrFullscreenViewer::VrFullscreenViewer()
{
    this->m_params.offsetLeftX = 0;
    this->m_params.offsetLeftY = 0;

    this->m_params.offsetRightX = 0;
    this->m_params.offsetRightY = 0;

    this->m_params.screenWidth = 0;
    this->m_params.screenHeight = 0;

    m_currentUserParam=1;

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

    this->m_params.offsetLeftX = 0;
    this->m_params.offsetLeftY = 0;

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
    qImageL = this->m_cameraL->single_grab_image(ret);
    qImageR = this->m_cameraR->single_grab_image(ret);

    this->m_frameR.setPixmap(QPixmap::fromImage(*qImageR));
    this->m_frameL.setPixmap(QPixmap::fromImage(*qImageL));

    m_currentUserParam = 1;
    loadUserParameters("./configFiles/UserParam1.yml");

    //Setting up the scene
    int imageWidth = this->m_frameR.pixmap().width();
    int imageHeight = this->m_frameR.pixmap().height();

    this->m_scene.setSceneRect(0,0,2 * imageWidth+m_params.screenWidth,imageHeight+m_params.screenHeight);
    updateUserParamInFrame();

    this->setScene(&this->m_scene);

    //adding items to the scene
    this->scene()->addItem(&this->m_frameR);
    this->scene()->addItem(&this->m_frameL);

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

    connect(this,SIGNAL(zoomIn(float)),imageUpdaterL,SLOT(zoomIn(float)));
    connect(this,SIGNAL(zoomOut(float)),imageUpdaterL,SLOT(zoomOut(float)));
    connect(this,SIGNAL(zoomIn(float)),imageUpdaterR,SLOT(zoomIn(float)));
    connect(this,SIGNAL(zoomOut(float)),imageUpdaterR,SLOT(zoomOut(float)));

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
    // qDebug() << "Paso1";

    this->m_frameR.setPixmap(QPixmap::fromImage(this->imageUpdaterR->getNextFrame()));
    this->m_frameL.setPixmap(QPixmap::fromImage(this->imageUpdaterL->getNextFrame()));

    // qDebug() << "Paso2";

    m_mean = (this->imageUpdaterL->getCurrentFPS()+this->imageUpdaterR->getCurrentFPS()+m_mean)/3.0;
    this->m_fpsCounter->setText(QString("FPS: ") + QString::number((int)m_mean));

    // qDebug() << "Paso3";

    //this allow us to resize the scene when a change in the undistort setting is done
    int imageWidth = this->m_frameR.pixmap().width();    
    int imageHeight = this->m_frameR.pixmap().height();

    if ( imageHeight<this->m_frameL.pixmap().height() )
    {
        imageHeight = this->m_frameL.pixmap().height();
    }

    // qDebug() << "Paso4";
    // qDebug() << "FrameR_width" << imageWidth;
    // qDebug() << "Frame L_width" << m_frameL.pixmap().width();

    // this->m_frameL.setPos(imageWidth - 100,0);

    // Esta linea de aqui abajo es del Richard.
    // this->m_scene.setSceneRect(0,0,imageWidth+m_frameL.pixmap().width(),imageHeight);
    // this->m_scene.setSceneRect(0,0, 2 * imageWidth,imageHeight);

    this->m_scene.setSceneRect(0,0,2 * imageWidth + m_params.screenWidth, imageHeight + m_params.screenHeight);

    // qDebug() << "Paso5" << sceneRect();

    this->fitInView(this->sceneRect(),Qt::KeepAspectRatio);

    // qDebug() << "Paso6";
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

void VrFullscreenViewer::saveUserParameters(QString filename)
{
    FileStorage fs (filename.toStdString(), FileStorage::WRITE);
    fs << "OffsetLeftX" << m_params.offsetLeftX;
    fs << "OffsetLeftY" << m_params.offsetLeftY;
    // Camera on the right
    fs << "OffsetRightX" << m_params.offsetRightX;
    fs << "OffsetRightY" << m_params.offsetRightY;
    // Screen size
    fs << "ScreenWidth" << m_params.screenWidth;
    fs << "ScreenHeight" << m_params.screenHeight;
}

void VrFullscreenViewer::loadUserParameters(QString filename)
{
    FileStorage fs(filename.toStdString(), FileStorage::READ);
    fs["OffsetLeftX"] >> m_params.offsetLeftX;
    fs["OffsetLeftY"] >> m_params.offsetLeftY;

    fs["OffsetRightX"] >> m_params.offsetRightX;
    fs["OffsetRightY"] >> m_params.offsetRightY;

    fs["ScreenWidth"] >> m_params.screenWidth;
    fs["ScreenHeight"] >> m_params.screenHeight;
}

void VrFullscreenViewer::updateUserParamInFrame()
{
    this->m_frameL.setPos(0+m_params.offsetLeftX,m_params.offsetLeftY);

    this->m_frameR.setPos(m_params.offsetRightX,m_params.offsetRightY);
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
         qDebug() << "Zoom in";
         emit zoomIn(100);
        break;
    case Qt::Key_Minus:
        qDebug() << "Zoom out";
        emit zoomOut(100);
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
        this->m_frameL.moveBy(0,-5);
        m_params.offsetLeftY = m_params.offsetLeftY - 5;
        break;
    case Qt::Key_A:
        this->m_frameL.moveBy(-5,0);
        m_params.offsetLeftX = m_params.offsetLeftX - 5;
        break;
    case Qt::Key_S:
        this->m_frameL.moveBy(0,5);
        m_params.offsetLeftY = m_params.offsetLeftY + 5;
        break;
    case Qt::Key_D:
        this->m_frameL.moveBy(5,0);
        m_params.offsetLeftX = m_params.offsetLeftX + 5;
        break;
    //Key events to move the Frame Counter with the arrow keys
    case Qt::Key_Up:
        // m_fpsCounter->moveByOffset(0,-5);
        m_params.screenHeight = m_params.screenHeight - 10;
        break;
    case Qt::Key_Down:
        // m_fpsCounter->moveByOffset(0,5);
        m_params.screenHeight = m_params.screenHeight + 10;
        break;
    case Qt::Key_Left:
        // m_fpsCounter->moveByOffset(-5,0);
        m_params.screenWidth = m_params.screenWidth - 10;
        break;
    case Qt::Key_Right:
        // m_fpsCounter->moveByOffset(5,0);
        m_params.screenWidth = m_params.screenWidth + 10;
        break;
    //Key events to move the window of the right camera - IJKL keys
    case Qt::Key_I:
        this->m_frameR.moveBy(0,-5);
        m_params.offsetRightY = m_params.offsetRightY - 5;
        break;
    case Qt::Key_K:
        this->m_frameR.moveBy(0,5);
        m_params.offsetRightY = m_params.offsetRightY + 5;;
        break;
    case Qt::Key_J:
        this->m_frameR.moveBy(-5,0);
        m_params.offsetRightX = m_params.offsetRightX - 5;
        break;
    case Qt::Key_L:
        this->m_frameR.moveBy(5,0);
        m_params.offsetRightX = m_params.offsetRightX + 5;
        break;
    //Key events to change de user configuration
    case Qt::Key_1:
        if(m_currentUserParam == 1)
            saveUserParameters("./configFiles/UserParam1.yml");
        else
        {
            m_currentUserParam = 1;
            loadUserParameters("./configFiles/UserParam1.yml");
            updateUserParamInFrame();
        }
        break;
    case Qt::Key_2:
        if(m_currentUserParam == 2)
            saveUserParameters("./configFiles/UserParam2.yml");
        else
        {
            m_currentUserParam = 2;
            loadUserParameters("./configFiles/UserParam2.yml");
            updateUserParamInFrame();
        }
        break;
    case Qt::Key_3:
        if(m_currentUserParam == 3)
            saveUserParameters("./configFiles/UserParam3.yml");
        else
        {
            m_currentUserParam = 3;
            loadUserParameters("./configFiles/UserParam3.yml");
            updateUserParamInFrame();
        }
        break;
    case Qt::Key_4:
        if(m_currentUserParam == 4)
            saveUserParameters("./configFiles/UserParam4.yml");
        else
        {
            m_currentUserParam = 4;
            loadUserParameters("./configFiles/UserParam4.yml");
            updateUserParamInFrame();
        }
        break;
    default:
        break;
    }
//    qDebug() << m_currentUserParam;
}












