#ifndef UTILS_H
#define UTILS_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "QImage"

using namespace cv;               //To use opencv

Mat QImage2Mat(QImage const& src);
QImage Mat2QImage(Mat const& src);
int odd2Even(int num);


#endif // UTILS_H
