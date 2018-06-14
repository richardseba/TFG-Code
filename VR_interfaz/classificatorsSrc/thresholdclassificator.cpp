#include "thresholdclassificator.h"
#include "processingimages.h"

ThresholdClassificator::ThresholdClassificator()
{
    m_oldMean = TemporalMean();
    m_th1 = 0;
    m_th2 = 0;
}

ThresholdClassificator::ThresholdClassificator(int numMeanStored, float threshold1,float threshold2)
{
    m_oldMean = TemporalMean(numMeanStored);
    m_th1 = threshold1;
    m_th2 = threshold2;
}


Distance ThresholdClassificator::calcClasificationProximity(Mat imgL, Mat imgR)
{
    Distance output;
    cv::Rect rectL; rectL.x = 0; rectL.y = 0; rectL.width = imgL.size().width; rectL.height = imgL.size().height;
    cv::Rect rectR; rectR.x = 0; rectR.y = 0; rectR.width = imgR.size().width; rectR.height = imgR.size().height;
    float mean = getMeanOfROI(imgL,imgR,rectL,rectR,0);
    m_oldMean.setNewValue(mean);
    mean = m_oldMean.getCurrentMean();
    qDebug() << "mean" << mean;

    if(mean>m_th1)
        output = CLOSE;
    else if(mean>m_th2)
        output = MEDIUM;
    else
        output = FAR;

    return output;
}

float ThresholdClassificator::calcClasificationFloat(Mat imgL, Mat imgR)
{
    cv::Scalar perChannelMeanL = cv::mean(imgL);
    float meanL = mean(perChannelMeanL)[0];
    cv::Scalar perChannelMeanR =  cv::mean(imgR);
    float meanR =mean(perChannelMeanR)[0];

    m_oldMean.setNewValue((meanL+meanR)/2);

    return m_oldMean.getCurrentMean();
}
