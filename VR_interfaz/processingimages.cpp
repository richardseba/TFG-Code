#include "processingimages.h"

Mat getColorFrom(Mat backgroundSrc, Mat colorSrc)
{
    cvtColor(backgroundSrc,backgroundSrc,CV_RGB2YCrCb);
    cvtColor(colorSrc,colorSrc,CV_RGB2YCrCb);

    Mat channelOrg[3];
    Mat channelColor[3];
    split(backgroundSrc,channelOrg);
    split(colorSrc,channelColor);

    channelOrg[2] = channelColor[2];
//    channelOrg[1] = channelColor[1];

    Mat imgMix(Size(backgroundSrc.size().width,backgroundSrc.size().height),CV_8UC3);
    merge(channelOrg,3,imgMix);

    cvtColor(imgMix,imgMix,CV_YCrCb2RGB);
    return imgMix;
}

MatPair processDisparity(QImage* Im1,QImage* Im2, Elas::setting elasSetting)
{
    Mat leftim(Im1->height(),Im1->width(),CV_8UC1,(uchar*)Im1->bits(),Im1->bytesPerLine());
    Mat rightim(Im2->height(),Im2->width(),CV_8UC1,(uchar*)Im2->bits(),Im2->bytesPerLine());

    Mat l,r;
    if(leftim.channels()==3){cvtColor(leftim,l,CV_BGR2GRAY); qDebug() << "converted to gray";}
    else leftim.copyTo(l);
    if(rightim.channels()==3){cvtColor(rightim,r,CV_BGR2GRAY); qDebug() << "converted to gray";}
    else rightim.copyTo(r);

    const cv::Size imsize = l.size();
    const int32_t dims[3] = {imsize.width,imsize.height,imsize.width}; // bytes per line = width

    Mat leftdpf = cv::Mat::zeros(imsize,CV_32F);
    Mat rightdpf = cv::Mat::zeros(imsize,CV_32F);

    Elas elas(elasSetting);
    elas.process(l.data,r.data,leftdpf.ptr<float>(0),rightdpf.ptr<float>(0),dims);

    MatPair outputMat;

    outputMat.l = leftdpf.clone();
    outputMat.r = rightdpf.clone();

    return outputMat;
}

QImagePair postProcessImages(MatPair src,bool colormap)
{
    double maxValue = 0;
    double minValue = 0;

    Mat leftdpf = src.l;
    Mat rightdpf = src.r;

    minMaxLoc(leftdpf,&minValue,&maxValue);
    int width = leftdpf.size().width;
    int height = rightdpf.size().height;

//    qDebug() << "max:" << maxValue;

    Mat D1(height,width,CV_8UC1);
    Mat D2(height,width,CV_8UC1);

    for (int32_t i=0; i<height; i++) {
        for(int  j = 0; j<width; j++){
            Point2d point(j,i);
            D1.at<uint8_t>(point) = (uint8_t)max(255.0*(leftdpf.at<float>(point)/maxValue),0.0);
            D2.at<uint8_t>(point) = (uint8_t)max(255.0*(rightdpf.at<float>(point)/maxValue),0.0);
        }
    }

    Mat temp;
    cvtColor(D1,temp,CV_GRAY2RGB);
    QImage tempIm1 = Mat2QImage(temp);
    cvtColor(D2,temp,CV_GRAY2RGB);
    QImage tempIm2 = Mat2QImage(temp);

    if(colormap)
    {
        Mat colormapL, colormapR;

        applyColorMap(D1,colormapL,COLORMAP_JET);
        applyColorMap(D2,colormapR,COLORMAP_JET);

        tempIm1 = Mat2QImage(colormapL);
        tempIm2 = Mat2QImage(colormapR);
    }

    QImagePair outPair;
    outPair.l = tempIm1.copy();
    outPair.r = tempIm2.copy();
    return outPair;
}


float getMeanOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, float excludedValues)
{
    Mat cutL = Mat(imL,rectL);
    Mat cutR = Mat(imR,rectR);

    Mat maskL = cutL > excludedValues;
    Mat maskR = cutR > excludedValues;

    cv::Scalar perChannelMeanL = cv::mean(cutL,maskL);
    float meanL = perChannelMeanL[0];

    cv::Scalar perChannelMeanR =  cv::mean(cutR,maskR);
    float meanR = perChannelMeanR[0];

    return (meanL+meanR)/2.0;
}

float getMaxOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, float excludedValues)
{
    Mat cutL = Mat(imL,rectL);
    Mat cutR = Mat(imR,rectR);

    Mat maskL = cutL > excludedValues;
    Mat maskR = cutR > excludedValues;

    double maxL, minL , maxR, minR;
    minMaxLoc(cutL,&minL,&maxL,0,0,maskL);
    minMaxLoc(cutR,&minR,&maxR,0,0,maskR);

    return (maxL+maxR)/2.0;
}

float getMinOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, float excludedValues)
{
    Mat cutL = Mat(imL,rectL);
    Mat cutR = Mat(imR,rectR);

    Mat maskL = cutL > excludedValues;
    Mat maskR = cutR > excludedValues;

    double maxL, minL , maxR, minR;
    minMaxLoc(cutL,&minL,&maxL,0,0,maskL);
    minMaxLoc(cutR,&minR,&maxR,0,0,maskR);

    return (minL+minR)/2.0;
}

float getMedianOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, float excludedValues)
{
    Mat cutL = Mat(imL,rectL);
    Mat cutR = Mat(imR,rectR);

    Mat maskL = cutL > excludedValues;
    Mat maskR = cutR > excludedValues;

    double medianL, medianR;
    medianL = medianMat(cutL,maskL,3);
    medianR = medianMat(cutR,maskR,3);

    qDebug() << medianL;
    return (medianL+medianR)/2.0;
}

double medianMat(cv::Mat Input, Mat mask, int nVals){

    // COMPUTE HISTOGRAM OF SINGLE CHANNEL MATRIX
    float range[] = { 0, nVals };
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;
    cv::Mat hist;
    calcHist(&Input, 1, 0, mask, hist, 1, &nVals, &histRange, uniform, accumulate);

    // COMPUTE CUMULATIVE DISTRIBUTION FUNCTION (CDF)
    cv::Mat cdf;
    hist.copyTo(cdf);
    for (int i = 1; i <= nVals-1; i++){
        cdf.at<float>(i) += cdf.at<float>(i - 1);
    }
    cdf /= Input.total();

    // COMPUTE MEDIAN
    double medianVal;
    for (int i = 0; i <= nVals-1; i++){
        if (cdf.at<float>(i) >= 0.5) { medianVal = i;  break; }
}
return medianVal/nVals; }

cv::Rect calculateCenteredROI(Size currentRect,int newWidth,int newHeight){
    cv::Rect roi;

    int centerX = currentRect.width/2;
    int centerY = currentRect.height/2;

    if(centerX > newWidth/2)
        roi.x = centerX-newWidth/2;
    else
        qDebug() << "ERROR calculating the centered ROI";

    if(centerY > newHeight/2)
        roi.y = centerY-newHeight/2;
    else
        qDebug() << "ERROR calculating the centered ROI";

    roi.width = newWidth;
    roi.height = newHeight;
    return roi;
}





















