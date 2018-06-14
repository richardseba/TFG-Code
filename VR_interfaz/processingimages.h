#ifndef PROCESSINGIMAGES_H
#define PROCESSINGIMAGES_H

#include "utils.h"
#include "./libelasSrc/elas.h"

Mat getColorFrom(Mat backgroundSrc, Mat colorSrc);
QImagePair processDisparity(QImage* Im1, QImage* Im2, bool colormap, Elas::setting elasSetting);
float getMeanOfROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR);
cv::Rect calculateCenteredROI(Size currentRect,int newWidth,int newHeight);

#endif // PROCESSINGIMAGES_H
