#include "irimagegenerator.h"
#include "utils.h"
#include "vector"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;               //To use opencv
using namespace std;               //To use opencv

IRimageGenerator::IRimageGenerator() : ImageGenerator () {}

IRimageGenerator::IRimageGenerator(ImageGenerator* imgGen) : ImageGenerator () {
    m_imgGen = imgGen;
}

void IRimageGenerator::start()
{
    m_imgGen->start();
    ImageGenerator::start();
}

void IRimageGenerator::stop()
{
    ImageGenerator::stop();
    ImageGenerator::waitEndLoopEvent();
    m_imgGen->stop();
}

int IRimageGenerator::hasEnded()
{
    return 0;
}

void IRimageGenerator::process()
{
    Mat matC, retC;
    QImage imageC = this->m_imgGen->getFrame();
    if(!imageC.isNull()){
        Mat maskC;
        cvtColor(QImage2Mat(imageC),matC,COLOR_RGB2GRAY);
        cv::threshold(matC,maskC, 200, 255,THRESH_BINARY);

        vector<Mat> channels;
        Mat g = Mat::zeros(Size(maskC.cols, maskC.rows), CV_8UC1);
        Mat b = Mat::zeros(Size(maskC.cols, maskC.rows), CV_8UC1);
        channels.push_back(b);
        channels.push_back(g);
        channels.push_back(maskC);
        channels.push_back(maskC);

        merge(channels, retC);
    } else retC = Mat::zeros(Size(matC.cols, matC.rows), CV_8UC4);

    m_imageMutex.lock();
    m_currentFrame = cvMatToQImage(retC).copy();
    m_imageMutex.unlock();
}

/* public Function getFrame
 * -------------------------------
 * function that will return the most recent image
*/
QImage IRimageGenerator::getFrame()
{
    QImage image;

    m_imageMutex.lock();
    image = m_currentFrame.copy();
    m_imageMutex.unlock();

    return image;
}
