/*
Copyright 2011. All rights reserved.
Institute of Measurement and Control Systems
Karlsruhe Institute of Technology, Germany

This file is part of libelas.
Authors: Andreas Geiger

libelas is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or any later version.

libelas is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
libelas; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA 02110-1301, USA
*/

// Demo program showing how libelas can be used, try "./elas -h" for help

#include <iostream>
#include "elas.h"
#include "image.h"
#include <algorithm>
#include "utils.h"
#include "../../VR_interfaz/stereocalibration.h"
#include "../../VR_interfaz/cameracalibration.h"
#include "../../VR_interfaz/processingimages.h"
#include "../../VR_interfaz/temporalmean.h"

#include <QImage>
#include <QImage>
#include <QDebug>
#include <QTime>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

enum OperationClass { meanopt=0, maxopt=1, minopt=2, medianopt=3};
enum SizeOperation { supsampling=0, roi=1, noOpt=2};


float getValueFromROI(Mat imL, Mat imR, cv::Rect rectL, cv::Rect rectR, TemporalMean &oldValues, OperationClass typeOpt )
{
    float mean = 0;
    switch (typeOpt) {
    case meanopt:
        mean = getMeanOfROI(imL,imR,rectL,rectR,0);
        break;
    case maxopt:
        mean = getMaxOfROI(imL,imR,rectL,rectR,0);
        break;
    case minopt:
        mean = getMinOfROI(imL,imR,rectL,rectR,0);
        break;
    case medianopt:
        mean = getMedianOfROI(imL,imR,rectL,rectR,0);
        break;
    }
//    qDebug() << mean;
    oldValues.setNewValue(mean);
    mean = oldValues.getCurrentMean();
    return mean;
}

void writeMatToFile(cv::Mat& m, const char* filename)
{
    ofstream fout(filename);

    if(!fout)
    {
        qDebug() <<"File Not Opened"<<endl;  return;
    }

    for(int i=0; i<m.rows; i++)
    {
        for(int j=0; j<m.cols; j++)
        {
            fout<<m.at<float>(i,j)<<", ";
        }
        fout<<endl;
    }

    fout.close();
}

void process (Mat I1,Mat I2, QImage &Im1, QImage &Im2, bool colorMap) {

    int bd = 0;

    Mat l,r;
    if(I1.channels()==3)cvtColor(I1,l,CV_BGR2GRAY);
    else l=I1;
    if(I2.channels()==3)cvtColor(I2,r,CV_BGR2GRAY);
    else r=I2;

    Mat lb,rb;
    cv::copyMakeBorder(l,lb,0,0,bd,bd,cv::BORDER_REPLICATE);
    cv::copyMakeBorder(r,rb,0,0,bd,bd,cv::BORDER_REPLICATE);

    const cv::Size imsize = lb.size();
    const int32_t dims[3] = {imsize.width,imsize.height,imsize.width}; // bytes per line = width

    cv::Mat leftdpf = cv::Mat::zeros(imsize,CV_32F);
    cv::Mat rightdpf = cv::Mat::zeros(imsize,CV_32F);

    Elas::parameters param;
    param.postprocess_only_left = true;
    Elas elas(param);
    elas.process(lb.data,rb.data,leftdpf.ptr<float>(0),rightdpf.ptr<float>(0),dims);

    double maxValue = 0;
    double minValue = 0;

    minMaxLoc(leftdpf,&minValue,&maxValue);
    int width = leftdpf.size().width;
    int height = rightdpf.size().height;

  //    qDebug() << disp_max << maxValue;
    Mat D1(height,width,CV_8UC1);
    Mat D2(height,width,CV_8UC1);

    for (int32_t i=0; i<height; i++) {
        for(int  j = 0; j<width; j++){
            Point2d point(j,i);
            D1.at<uint8_t>(point) = (uint8_t)max(255.0*(leftdpf.at<float>(point)/maxValue),0.0);
            D2.at<uint8_t>(point) = (uint8_t)max(255.0*(rightdpf.at<float>(point)/maxValue),0.0);
        }
    }

    Mat temp;
    cvtColor(D1,temp,CV_GRAY2RGB);
    QImage tempIm1 = Mat2QImage(temp);
    cvtColor(D2,temp,CV_GRAY2RGB);
    QImage tempIm2 = Mat2QImage(temp);

    if(colorMap)
    {
        Mat colormapL, colormapR;

        applyColorMap(D1,colormapL,COLORMAP_JET);
        applyColorMap(D2,colormapR,COLORMAP_JET);

        tempIm1 = Mat2QImage(colormapL);
        tempIm2 = Mat2QImage(colormapR);
    }

  Im1 = tempIm1;
  Im2 = tempIm2;

}

// compute disparities of pgm image input pair file_1, file_2
void process (const char* file_1,const char* file_2, bool colorMap) {

  cout << "Processing: " << file_1 << ", " << file_2 << endl;

  QImage imLeft = QImage(QString(file_1)).convertToFormat(QImage::Format_Grayscale8);
  QImage imRight= QImage(QString(file_2)).convertToFormat(QImage::Format_Grayscale8);

  Mat leftim(imLeft.height(),imLeft.width(),CV_8UC1,(uchar*)imLeft.bits(),imLeft.bytesPerLine());

  Mat rightim(imRight.height(),imRight.width(),CV_8UC1,(uchar*)imRight.bits(),imRight.bytesPerLine());

  QImage outL;
  QImage outR;

  process(leftim,rightim,outL,outR,colorMap);

  if(colorMap){
      outL.save(QString(file_1)+"_disp.png");
      outR.save(QString(file_2)+"_disp.png");
  } else {
      outL.save(QString(file_1)+"_disp.pgm");
      outR.save(QString(file_2)+"_disp.pgm");
  }

}


void processframes(QString path_L,QString path_R,bool colorMap)
{
    VideoWriter newVideoL;
    VideoWriter newVideoR;

    Mat frameL, frameR;
    QImage out1,out2;

    frameL = imread((path_L+"frame_L_0.pgm").toLatin1().data());
    frameR = imread((path_R+"frame_R_0.pgm").toLatin1().data());

    newVideoL.open("new_video_out_L.avi",-1,33,frameL.size());
    newVideoR.open("new_video_out_R.avi",-1,33,frameR.size());

    for(int i = 0; i < 350; i++)
    {
        if(i%50 == 0) qDebug() << i << "frames procesed";
        QString nameL = path_L+"frame_L_"+QString::number(i)+".pgm";
        QString nameR = path_R+"frame_R_"+QString::number(i)+".pgm";

        frameL = imread(nameL.toLatin1().data());
        frameR = imread(nameR.toLatin1().data());

        process(frameL,frameR,out1,out2,colorMap);

        newVideoL << QImage2Mat(out1);
        newVideoR << QImage2Mat(out2);
    }

    newVideoL.release();
    newVideoR.release();
}

void processVideos(QString path_L,QString path_R,bool colorMap,Size outSize,OperationClass optType,SizeOperation typeSize, bool saveVideo, char* outfilename)
{
    VideoCapture videoL(path_L.toLatin1().data());
    VideoCapture videoR(path_R.toLatin1().data());

    int width = videoL.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = videoL.get(CV_CAP_PROP_FRAME_HEIGHT);
    int nFrames = videoL.get(CV_CAP_PROP_FRAME_COUNT);

    Size originalSize(width,height);

    if(outSize.empty())
        outSize = originalSize;

    VideoWriter newVideoL;
    VideoWriter newVideoR;

    char pathLeft[] ="./configFiles/calibLeft.yml";
    char pathRight[] ="./configFiles/calibRight.yml";
    char pathStereo[] ="./configFiles/calibStereo.yml";
    StereoCalibration stereocalib = StereoCalibration(CameraCalibration(pathLeft),CameraCalibration(pathRight),pathStereo);
    stereocalib.initUndistortImage();

    TemporalMean oldValues(10);
    Mat outputValues(1,nFrames,CV_32F);

    Mat frameL, frameR;
    QImage out1,out2;

    if(saveVideo){
        newVideoL.open("./video/Lejos_L.avi",-1,33,outSize);
        newVideoR.open("./video/Lejos_R.avi",-1,33,outSize);
    }

    for(int i = 0; i < nFrames; i++)
    {
        if(i%50 == 0) qDebug() << i << "frames procesed";
        videoL >> frameL;
        videoR >> frameR;

        QImage imLeft = Mat2QImage(frameL).convertToFormat(QImage::Format_RGB888);
        QImage imRight= Mat2QImage(frameR).convertToFormat(QImage::Format_RGB888);

        Mat tempL = stereocalib.undistortLeft(QImage2Mat(imLeft),CV_INTER_LINEAR);
        Mat tempR = stereocalib.undistortRight(QImage2Mat(imRight),CV_INTER_LINEAR);

        if(typeSize == supsampling) {
            Mat subsamplingL;
            Mat subsamplingR;

            cv::resize(tempL,subsamplingL,outSize,INTER_LINEAR);
            cv::resize(tempR,subsamplingR,outSize,INTER_LINEAR);

            subsamplingL.copyTo(tempL);
            subsamplingR.copyTo(tempR);
        }
        if(typeSize == roi) {
                cv::Rect rect = calculateCenteredROI(Size(tempL.size().width,tempL.size().height),outSize.width,outSize.height);
                tempL = Mat(tempL,rect);
                tempR = Mat(tempR,rect);
        }

        QImage qtempL = Mat2QImage(tempL).convertToFormat(QImage::Format_Grayscale8);
        QImage qtempR = Mat2QImage(tempR).convertToFormat(QImage::Format_Grayscale8);

        MatPair output = processDisparity(&qtempL,&qtempR,Elas::MIDDLEBURY);

        cv::Rect rect = calculateCenteredROI(outSize,outSize.width/2,outSize.height/2);

        outputValues.at<float>(0,i) = getValueFromROI(output.l,output.r,rect,rect,oldValues,optType);

        if(saveVideo){
            QImagePair dispImOut = postProcessImages(output,colorMap);
            newVideoL << QImage2Mat(dispImOut.l.copy());
            newVideoR << QImage2Mat(dispImOut.r.copy());
        }
    }
    writeMatToFile(outputValues,outfilename);

    if(saveVideo){
        newVideoL.release();
        newVideoR.release();
    }
}

int main (int argc, char** argv) {
    QTime crono = QTime();
    crono.start();
    bool colormap = true;
  // run demo
  if (argc==2 && !strcmp(argv[1],"demo")) {
    process("img/cones_left.pgm",   "img/cones_right.pgm",colormap);
    process("img/aloe_left.pgm",    "img/aloe_right.pgm",colormap);
    process("img/raindeer_left.pgm","img/raindeer_right.pgm",colormap);
    process("img/urban1_left.pgm",  "img/urban1_right.pgm",colormap);
    process("img/urban2_left.pgm",  "img/urban2_right.pgm",colormap);
    process("img/urban3_left.pgm",  "img/urban3_right.pgm",colormap);
    process("img/urban4_left.pgm",  "img/urban4_right.pgm",colormap);
    cout << "... done!" << endl;
    qDebug() << crono.restart()/1000.0;

  // compute disparity from input pair
  } else if (argc==3) {
    process(argv[1],argv[2],colormap);
    cout << "... done!" << endl;
  } else if (argc==4 && !strcmp(argv[1],"videof")){
    cout << "procesing videof \n";
    processframes(argv[2],argv[3],colormap);
  } else if (argc==4 && !strcmp(argv[1],"videov")){
      cout << "procesing videov \n";
      char LejosL[] = "C:/Users/rsegovia/Desktop/VideosParaPruebas/Lejos_L.avi";
      char LejosR[] = "C:/Users/rsegovia/Desktop/VideosParaPruebas/Lejos_R.avi";
      OperationClass typeopt = meanopt;
//      processVideos(LejosL,LejosR,colormap,Size(1100,1100),typeopt,noOpt,         false,(char*)"./outfiles/Lejos/org/minopt.txt");
//      qDebug() << "first done";
//      processVideos(LejosL,LejosR,colormap,Size(550,550),  typeopt,roi,           false,(char*)"./outfiles/Lejos/roi/meanopt.txt");

//      processVideos(LejosL,LejosR,colormap,Size(275,275),  typeopt,roi,           true,(char*)"./outfiles/Lejos/roi 4/meanopt.txt");

//      processVideos(LejosL,LejosR,colormap,Size(91,91),  typeopt,roi,           true,(char*)"./outfiles/Lejos/roi 12/meanopt.txt");
//      qDebug() << "second done";
//      processVideos(LejosL,LejosR,colormap,Size(550,550),  typeopt,supsampling,   false,(char*)"./outfiles/Lejos/sup 2/maxopt.txt");

//      processVideos(LejosL,LejosR,colormap,Size(275,275),  typeopt,supsampling,   false,(char*)"./outfiles/Lejos/sup 4/minopt.txt");
//      qDebug() << "first done";
//      processVideos(LejosL,LejosR,colormap,Size(183,183),  typeopt,supsampling,   false,(char*)"./outfiles/Lejos/sup 6/maxopt.txt");
//      qDebug() << "second done";
//      processVideos(LejosL,LejosR,colormap,Size(137,137),  typeopt,supsampling,   false,(char*)"./outfiles/Lejos/sup 8/maxopt.txt");
//      qDebug() << "third done";
//      processVideos(LejosL,LejosR,colormap,Size(110,110),  typeopt,supsampling,   false,(char*)"./outfiles/Lejos/sup 10/maxopt.txt");
      qDebug() << "fift done";
//      processVideos(LejosL,LejosR,colormap,Size(91,91),  typeopt,supsampling,   false,(char*)"./outfiles/Lejos/sup 12/minopt.txt");

      qDebug() << "------------Medium Processing";
      char MediumL[] = "C:/Users/rsegovia/Desktop/VideosParaPruebas/Medium_L.avi";
      char MediumR[] = "C:/Users/rsegovia/Desktop/VideosParaPruebas/Medium_R.avi";


//      processVideos(MediumL,MediumR,colormap,Size(1100,1100),typeopt,noOpt,         false,(char*)"./outfiles/Medium/org/maxopt.txt");
//      qDebug() << "first done";
//      processVideos(MediumL,MediumR,colormap,Size(550,550),  typeopt,roi,           false,(char*)"./outfiles/Medium/roi/meanopt.txt");

//      processVideos(MediumL,MediumR,colormap,Size(275,275),  typeopt,roi,           false,(char*)"./outfiles/Medium/roi 4/meanopt.txt");

//      processVideos(MediumL,MediumR,colormap,Size(91,91),  typeopt,roi,           true,(char*)"./outfiles/Medium/roi 12/meanopt.txt");
//      qDebug() << "second done";
//      processVideos(MediumL,MediumR,colormap,Size(550,550),  typeopt,supsampling,   false,(char*)"./outfiles/Medium/sup 2/minopt.txt");
//      qDebug() << "third done";
//      processVideos(MediumL,MediumR,colormap,Size(275,275),  typeopt,supsampling,   false,(char*)"./outfiles/Medium/sup 4/minopt.txt");
//      qDebug() << "forth done";
//      processVideos(MediumL,MediumR,colormap,Size(183,183),  typeopt,supsampling,   false,(char*)"./outfiles/Medium/sup 6/minopt.txt");
//      qDebug() << "fift done";
//      processVideos(MediumL,MediumR,colormap,Size(137,137),  typeopt,supsampling,   false,(char*)"./outfiles/Medium/sup 8/minopt.txt");
//      qDebug() << "sixth done";
//      processVideos(MediumL,MediumR,colormap,Size(110,110),  typeopt,supsampling,   false,(char*)"./outfiles/Medium/sup 10/minopt.txt");
//      qDebug() << "seventh done";
//      processVideos(MediumL,MediumR,colormap,Size(91,91),  typeopt,supsampling,   false,(char*)"./outfiles/Medium/sup 12/meanopt.txt");

      qDebug() << "------------Closer Processing";
      char CercaL[] = "C:/Users/rsegovia/Desktop/VideosParaPruebas/Cerca_L.avi";
      char CercaR[] = "C:/Users/rsegovia/Desktop/VideosParaPruebas/Cerca_R.avi";

//      processVideos(CercaL,CercaR,colormap,Size(1100,1100),typeopt,noOpt,         false,(char*)"./outfiles/Cerca/org/minopt.txt");
//      qDebug() << "first done";
//      processVideos(CercaL,CercaR,colormap,Size(550,550),  typeopt,roi,           false,(char*)"./outfiles/Cerca/roi/meanopt.txt");

//      processVideos(CercaL,CercaR,colormap,Size(275,275),  typeopt,roi,           true,(char*)"./outfiles/Cerca/roi 4/meanopt.txt");

      processVideos(CercaL,CercaR,colormap,Size(91,91),  typeopt,roi,           true,(char*)"./outfiles/Cerca/roi 12/meanopt.txt");
//      qDebug() << "second done";
//      processVideos(CercaL,CercaR,colormap,Size(550,550),  typeopt,supsampling,   false,(char*)"./outfiles/Cerca/sup 2/undisTime.txt");
//      qDebug() << "third done";
//      processVideos(CercaL,CercaR,colormap,Size(275,275),  typeopt,supsampling,   false,(char*)"./outfiles/Cerca/sup 4/meanopt.txt");
//      qDebug() << "forth done";
//      processVideos(CercaL,CercaR,colormap,Size(183,183),  typeopt,supsampling,   false,(char*)"./outfiles/Cerca/sup 6/undisTime.txt");
//      qDebug() << "fift done";
//      processVideos(CercaL,CercaR,colormap,Size(137,137),  typeopt,supsampling,   false,(char*)"./outfiles/Cerca/sup 8/undisTime.txt");
//      qDebug() << "sixth done";
//      processVideos(CercaL,CercaR,colormap,Size(110,110),  typeopt,supsampling,   false,(char*)"./outfiles/Cerca/sup 10/undisTime.txt");
//      qDebug() << "seventh done";
//      processVideos(CercaL,CercaR,colormap,Size(91,91),  typeopt,supsampling,   false,(char*)"./outfiles/Cerca/sup 12/minopt.txt");


  // display help
  } else if (argc==2 && !strcmp(argv[1],"test")){
//      getMeanOfROI();
  // display help
  } else {
    cout << endl;
    cout << "ELAS demo program usage: " << endl;
    cout << "./elas demo ................ process all test images (image dir)" << endl;
    cout << "./elas left.pgm right.pgm .. process a single stereo pair" << endl;
    cout << "./elas videof ./folder/frames* ./folder/frames* .. process a undistorted stereo pair frames" << endl;
    cout << "./elas videov videoLeft.avi videoRight.avi .. process a undistorted stereo pair video" << endl;
    cout << "./elas -h .................. shows this help" << endl;
    cout << endl;
    cout << "Note: All images must be pgm greylevel images. All output" << endl;
    cout << "      disparities will be scaled such that disp_max = 255." << endl;
    cout << endl;
  }
  qDebug() << "done";

  return 0;
}


