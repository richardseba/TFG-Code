#include "depthprocessing.h"

#include "processingimages.h"
#include "utils.h"
#include <QTime>

DepthProcessing::DepthProcessing()
{
}

DepthProcessing::DepthProcessing(QTimer* timer, StereoCalibration stereoCalib,
                                 float threshold1, float threshold2, int meanBuffSize,
                                 int subsampling, Elas::setting elasSetting)
{
    m_timerTrigger = timer;
    m_currentCalib = stereoCalib;
    m_classifier = ThresholdClassificator(meanBuffSize,threshold1,threshold2);
    m_currentDistance = FAR;
    m_currentDistanceValue = 0;
    m_subsampling = subsampling;
    m_libelasSettings = elasSetting;
    connect(m_timerTrigger, SIGNAL (timeout()), this, SLOT (frameProcessingEvent()));
}

DepthProcessing::~DepthProcessing()
{
    m_timerTrigger->stop();
}

//Gets
Distance DepthProcessing::getCurrentDistance()
{
    Distance returnVal;
    m_mutexDistance.lock();
    returnVal = m_currentDistance;
    m_mutexDistance.unlock();
    return returnVal;
}

float DepthProcessing::getCurrentDistanceValue()
{
    float returnVal;
    m_mutexDistanceValue.lock();
    returnVal = m_currentDistanceValue;
    m_mutexDistanceValue.unlock();
    return returnVal;
}

int DepthProcessing::getSubsampling()
{
    int returnVal;
    m_mutexSubsampling.lock();
    returnVal = m_subsampling;
    m_mutexSubsampling.unlock();
    return returnVal;
}

QImagePair DepthProcessing::getImages2Process()
{
    QImagePair returnImgs;
    m_mutexImages2Process.lock();
    returnImgs.l = m_images2Process.l.copy();
    returnImgs.r = m_images2Process.r.copy();
    m_mutexImages2Process.unlock();
    return returnImgs;
}

Elas::setting DepthProcessing::getLibelasSetting()
{
    Elas::setting returnVal;
    m_mutexElasSettings.lock();
    returnVal = m_libelasSettings;
    m_mutexElasSettings.unlock();
    return returnVal;
}

//Sets
void DepthProcessing::setCurrentDistance(Distance dist)
{
    m_mutexDistance.lock();
    m_currentDistance = dist;
    m_mutexDistance.unlock();
}

void DepthProcessing::setCurrentDistanceValue(float dist)
{
    m_mutexDistanceValue.lock();
    m_currentDistanceValue = dist;
    m_mutexDistanceValue.unlock();
}

void DepthProcessing::setSubsampling(int subsampling)
{
    m_mutexSubsampling.lock();
    m_subsampling = subsampling;
    m_mutexSubsampling.unlock();
}

void DepthProcessing::setImages2Process(QImagePair imgPair,Size processingWindowSize)
{
//    QTime crono;
//    crono.start();

    cv::Rect rect = calculateCenteredROI(Size(imgPair.l.size().width(), imgPair.l.size().height()),
                                        processingWindowSize.width,processingWindowSize.height);

    QRect imrect(rect.x, rect.y, rect.width, rect.height);

    m_mutexImages2Process.lock();
    m_images2Process.l = imgPair.l.copy(imrect);
    m_images2Process.r = imgPair.r.copy(imrect);
    m_mutexImages2Process.unlock();

//    qDebug() << crono.restart();
}

void DepthProcessing::setLibelasSetting(Elas::setting setting)
{
    m_mutexElasSettings.lock();
    m_libelasSettings = setting;
    m_mutexElasSettings.unlock();
 }

//Slots
void DepthProcessing::setProcessingEvent(bool processing)
{
    if(processing){
        m_timerTrigger->start();
    } else {
        m_timerTrigger->stop();
    }
}

void DepthProcessing::frameProcessingEvent()
{
    QImagePair currentImages = this->getImages2Process();

    if(!currentImages.l.isNull() && !currentImages.r.isNull()) {
        QImage imLeft = currentImages.l.convertToFormat(QImage::Format_RGB888);
        QImage imRight= currentImages.r.convertToFormat(QImage::Format_RGB888);
        int downSampling = this->getSubsampling();

        Mat undisL,undisR ;
        QImage tempL,tempR;

        if(downSampling >0){
            Mat subsampL,subsampR;
            QRect outRect= imLeft.rect();
            Size outSize(outRect.width()/downSampling,outRect.height()/downSampling);

            undisL = m_currentCalib.undistortLeft(QImage2Mat(imLeft),CV_INTER_LINEAR);
            undisR = m_currentCalib.undistortRight(QImage2Mat(imRight),CV_INTER_LINEAR);

            cv::resize(undisL,subsampL,outSize,INTER_LINEAR);
            cv::resize(undisR,subsampR,outSize,INTER_LINEAR);

            tempL = Mat2QImage(subsampL).convertToFormat(QImage::Format_Grayscale8);
            tempR = Mat2QImage(subsampR).convertToFormat(QImage::Format_Grayscale8);
        } else {
            tempL = Mat2QImage(m_currentCalib.undistortLeft(QImage2Mat(imLeft),CV_INTER_LINEAR)).convertToFormat(QImage::Format_Grayscale8);
            tempR = Mat2QImage(m_currentCalib.undistortRight(QImage2Mat(imRight),CV_INTER_LINEAR)).convertToFormat(QImage::Format_Grayscale8);
        }

        MatPair dispOut;

        dispOut = processDisparity(&tempL,&tempR,this->getLibelasSetting());
        cv::Rect centerROI = calculateCenteredROI(dispOut.l.size(),dispOut.l.size().width/downSampling,dispOut.l.size().height/downSampling);

        Mat cutL = Mat(dispOut.l,centerROI);
        Mat cutR = Mat(dispOut.r,centerROI);

        Distance value = m_classifier.calcClasificationProximity(cutL,cutR);

        setCurrentDistance(value);
//        if(value == CLOSE)
//            qDebug() << "near";
//        else if(value == MEDIUM)
//            qDebug() << "medium";
//        else
//            qDebug() << "far";
    }
}











