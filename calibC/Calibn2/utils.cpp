#include "utils.h"

/* Function Mat2QImage
 * -------------------------------
 * transforms a opencv's Mat to a Qt's QImage
 *
 * src : matrix to transform
 *
 * return -> the input Mat as a QImage
*/
QImage Mat2QImage(Mat const& src)
{
    Mat img;
    img = imread("./Dataset/Calib/right/1.png");
     Mat temp;
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     // of QImage::QImage ( const uchar * data, int width, int height, Format format )
     return dest;
}

/* Function QImage2Mat
 * -------------------------------
 * transforms a Qt's QImage to opencv's Mat
 *
 * src : QImage to tranform
 *
 * return -> the input QImage as Mat
*/
Mat QImage2Mat(QImage const& src)
{
     Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
     cvtColor(tmp, tmp,CV_BGR2RGB);
     return tmp;
}

/* Function odd2Even
 * -------------------------------
 * transforms a odd number into an even number
 * if a even number is given, the same number will be
 * returned
 *
 * num : odd number
 *
 * return -> even number
*/
int odd2Even(int num)
{
    if(num%2 != 0)
        num = num - 1;
    else
        num = num;
    return num;
}
