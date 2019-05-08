#ifndef UTILS_H
#define UTILS_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/types_c.h"

#include "QImage"
#include "QDebug"
#include <QImage>
#include <QPixmap>
#include <QtGlobal>
#include <QPainter>


/* Processing images file
 * -------------------------------
 * This File contains some convinient functions used for the development of this software
*/

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

void mixImages(QImage* background, QImage foreground, QPoint positionOverImage);

/**
   Functions to convert between OpenCV's cv::Mat and Qt's QImage and QPixmap.

   Andy Maloney <asmaloney@gmail.com>
   https://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap
**/
QImage  cvMatToQImage( const cv::Mat &inMat );
QPixmap cvMatToQPixmap( const cv::Mat &inMat );

cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );
cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData = true );


#endif // UTILS_H
