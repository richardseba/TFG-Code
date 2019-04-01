#include "histogramimage.h"

#include <QtCharts/QBarSet>
#include <QTime>

#include "utils.h"



void HistogramImage::updateHistogram(QtCharts::QChart* currentChart, QImage newImage, int numBins)
{
//    QTime crono;
//    crono.start();
    QtCharts::QBarSeries* barSerie = new QtCharts::QBarSeries();
    currentChart->removeAllSeries();
//    qDebug() << "first step" << crono.restart();
    int* histogramVal = createDataseries(newImage, numBins);
    for (int i = 0; i < numBins; ++i) {
        QtCharts::QBarSet* currentSet = new QtCharts::QBarSet("testing");
        currentSet->append(histogramVal[i]);
        barSerie->append(currentSet);
    }
    currentChart->addSeries(barSerie);
//    qDebug() << "second step" << crono.restart();
}
int* HistogramImage::createDataseries(QImage image, int numBins)
{

    Mat imageMat;
    imageMat = QImage2Mat(image);

    float multFactor = (numBins/255.0);

    int* histogramVal = new int[numBins] ;
    std::fill(histogramVal,histogramVal+numBins,0); //initialize vector to zeros
    for (int i = 0; i < imageMat.rows; i++) {
        for (int j = 0; j < imageMat.cols; ++j) {

            int red = imageMat.at<Vec3b>(i,j)[0];
            int green = imageMat.at<Vec3b>(i,j)[1];
            int blue = imageMat.at<Vec3b>(i,j)[2];
            int idxHist = ((blue + red + green)/3)*multFactor;
            histogramVal[idxHist]++;
        }
    }
    return histogramVal;
}




















