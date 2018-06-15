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
    float meanL = mean(perChannelMeanL)[0];

    cv::Scalar perChannelMeanR =  cv::mean(cutR,maskR);
    float meanR = mean(perChannelMeanR)[0];

    return (meanL+meanR)/2.0;
}

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





















