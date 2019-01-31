#ifndef HISTOGRAMIMAGE_H
#define HISTOGRAMIMAGE_H

#include <QImage>
#include <QtCharts>


class HistogramImage
{
public:
//        static histogramImage& getInstance();
        HistogramImage(HistogramImage const&)  = delete;
        void operator=(HistogramImage const&)  = delete;

        static void updateHistogram(QtCharts::QChart* currentChart, QImage newImage,int numBins);
private:
        static int* createDataseries(QImage image, int numBins);


};

#endif // HISTOGRAMIMAGE_H
