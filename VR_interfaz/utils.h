#ifndef UTILS_H
#define UTILS_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "QImage"
#include "QDebug"
#include <QImage>

using namespace cv;               //To use opencv

typedef struct
{
    QImage im1;
    QImage im2;
} QImagePair;

typedef struct
{
    QImage* im1;
    QImage* im2;
} QImagePairPtr;


typedef struct
{
    Mat im1;
    Mat im2;
} MatPair;


Mat QImage2Mat(QImage const& src);
QImage Mat2QImage(Mat const& src);
int odd2Even(int num);


#endif // UTILS_H
