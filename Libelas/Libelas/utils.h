#ifndef UTILS_H
#define UTILS_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "QImage"
#include "QDebug"

using namespace cv;               //To use opencv

typedef struct
{
    QImage l;
    QImage r;
} QImagePair;

typedef struct
{
    QImage* l;
    QImage* r;
} QImagePairPtr;


typedef struct
{
    Mat l;
    Mat r;
} MatPair;

typedef struct
{
    Mat* l;
    Mat* r;
} MatPairPtr;


Mat QImage2Mat(QImage const& src);
QImage Mat2QImage(Mat const& src);
int odd2Even(int num);


#endif // UTILS_H
