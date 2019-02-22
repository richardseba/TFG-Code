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
     Mat temp;
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copy, that is what i need
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

/*
   Endianness
   ---

   Although not totally clear from the docs, some of QImage's formats we use here are
   endian-dependent. For example:

      Little Endian
         QImage::Format_ARGB32 the bytes are ordered:    B G R A
         QImage::Format_RGB32 the bytes are ordered:     B G R (255)
         QImage::Format_RGB888 the bytes are ordered:    R G B

      Big Endian
         QImage::Format_ARGB32 the bytes are ordered:    A R G B
         QImage::Format_RGB32 the bytes are ordered:     (255) R G B
         QImage::Format_RGB888 the bytes are ordered:    R G B

   Notice that Format_RGB888 is the same regardless of endianness. Since OpenCV
   expects (B G R) we need to swap the channels for this format.

   This is why some conversions here swap red and blue and others do not.

   This code assumes little endian. It would be possible to add conversions for
   big endian machines though. If you are using such a machine, please feel free
   to submit a pull request on the GitHub page.
*/
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
#error Some of QImage's formats are endian-dependant. This file assumes little endian. See comment at top of header.
#endif

QImage cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
       // 8-bit, 4 channel
       case CV_8UC4:
       {
          QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_ARGB32 );

          return image;
       }

       // 8-bit, 3 channel
       case CV_8UC3:
       {
          QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_RGB888 );

          return image.rgbSwapped();
       }

       // 8-bit, 1 channel
       case CV_8UC1:
       {
          QImage image( inMat.data,
                        inMat.cols, inMat.rows,
                        static_cast<int>(inMat.step),
                        QImage::Format_Grayscale8 );
          return image;
       }
       default:
          qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
          break;
    }
    return QImage();
}

QPixmap cvMatToQPixmap( const cv::Mat &inMat )
{
  return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

// If inImage exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inImage's
// data with the cv::Mat directly
//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
//    NOTE: This does not cover all cases - it should be easy to add new ones as required.
cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData)
{
  switch ( inImage.format() )
  {
     // 8-bit, 4 channel
     case QImage::Format_ARGB32:
     case QImage::Format_ARGB32_Premultiplied:
     {
        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC4,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        return (inCloneImageData ? mat.clone() : mat);
     }

     // 8-bit, 3 channel
     case QImage::Format_RGB32:
     {
        if ( !inCloneImageData )
        {
           qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC4,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        cv::Mat  matNoAlpha;

        cv::cvtColor( mat, matNoAlpha, cv::COLOR_BGRA2BGR );   // drop the all-white alpha channel

        return matNoAlpha;
     }

     // 8-bit, 3 channel
     case QImage::Format_RGB888:
     {
        if ( !inCloneImageData )
        {
           qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
        }

        QImage   swapped = inImage.rgbSwapped();

        return cv::Mat( swapped.height(), swapped.width(),
                        CV_8UC3,
                        const_cast<uchar*>(swapped.bits()),
                        static_cast<size_t>(swapped.bytesPerLine())
                        ).clone();
     }

     // 8-bit, 1 channel
     case QImage::Format_Indexed8:
     {
        cv::Mat  mat( inImage.height(), inImage.width(),
                      CV_8UC1,
                      const_cast<uchar*>(inImage.bits()),
                      static_cast<size_t>(inImage.bytesPerLine())
                      );

        return (inCloneImageData ? mat.clone() : mat);
     }

     default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
  }
  return cv::Mat();
}

// If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
// with the cv::Mat directly
//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
cv::Mat QPixmapToCvMat( const QPixmap &inPixmap, bool inCloneImageData)
{
  return QImageToCvMat( inPixmap.toImage(), inCloneImageData );
}


