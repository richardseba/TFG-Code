#ifndef PROCESSINGIMAGES_H
#define PROCESSINGIMAGES_H

#include "utils.h"
#include "./libelasSrc/elas.h"

Mat getColorFrom(Mat backgroundSrc, Mat colorSrc);

MatPair processDisparity(QImage* Im1, QImage* Im2,Elas::setting elasSetting);
QImagePair postProcessImages(MatPair src,bool colormap);

float getMeanOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, float excludedValues);
float getMaxOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, float excludedValues);
float getMinOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, float excludedValues);
float getMedianOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, float excludedValues);
cv::Rect calculateCenteredROI(Size currentRect,int newWidth,int newHeight);
double medianMat(cv::Mat Input,Mat mask, int nVals);

#endif // PROCESSINGIMAGES_H
