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

#include <QImage>
#include <QImage>
#include <QDebug>
#include <QTime>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

// compute disparities of pgm image input pair file_1, file_2
void process (const char* file_1,const char* file_2, bool colorMap) {

  cout << "Processing: " << file_1 << ", " << file_2 << endl;

  QImage imLeft = QImage(QString(file_1));
  QImage imRight= QImage(QString(file_2));

  Mat leftim(imLeft.height(),imLeft.width(),CV_8UC1,(uchar*)imLeft.bits(),imLeft.bytesPerLine());

  Mat rightim(imRight.height(),imRight.width(),CV_8UC1,(uchar*)imRight.bits(),imRight.bytesPerLine());

  int bd = 0;

  Mat l,r;
  if(leftim.channels()==3)cvtColor(leftim,l,CV_BGR2GRAY);
  else l=leftim;
  if(rightim.channels()==3)cvtColor(rightim,r,CV_BGR2GRAY);
  else r=rightim;

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

  float disp_max = 0;
  int width = leftdpf.size().width;
  int height = rightdpf.size().height;
  for (int i=0; i<height; i++) {
      for(int j = 0; j<width; j++){
        if (leftdpf.at<float>(i,j)>disp_max) disp_max = leftdpf.at<float>(i,j);
        if (rightdpf.at<float>(i,j)>disp_max) disp_max = rightdpf.at<float>(i,j);
      }
  }
  Mat D1(height,width,CV_8UC1);
  Mat D2(height,width,CV_8UC1);

  for (int32_t i=0; i<height; i++) {
      for(int  j = 0; j<width; j++){
          Point2d point(j,i);
          D1.at<uint8_t>(point) = (uint8_t)max(255.0*(leftdpf.at<float>(point)/disp_max),0.0);
          D2.at<uint8_t>(point) = (uint8_t)max(255.0*(rightdpf.at<float>(point)/disp_max),0.0);
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

  tempIm1.save(QString(file_1)+"_disp.png");
  tempIm2.save(QString(file_2)+"_disp.png");

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

    float disp_max = 0;
    int width = leftdpf.size().width;
    int height = rightdpf.size().height;
    for (int i=0; i<height; i++) {
        for(int j = 0; j<width; j++){
          if (leftdpf.at<float>(i,j)>disp_max) disp_max = leftdpf.at<float>(i,j);
          if (rightdpf.at<float>(i,j)>disp_max) disp_max = rightdpf.at<float>(i,j);
        }
    }
    Mat D1(height,width,CV_8UC1);
    Mat D2(height,width,CV_8UC1);

    for (int32_t i=0; i<height; i++) {
        for(int  j = 0; j<width; j++){
            Point2d point(j,i);
            D1.at<uint8_t>(point) = (uint8_t)max(255.0*(leftdpf.at<float>(point)/disp_max),0.0);
            D2.at<uint8_t>(point) = (uint8_t)max(255.0*(rightdpf.at<float>(point)/disp_max),0.0);
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

void processframes(QString path_L,QString path_R)
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

        process(frameL,frameR,out1,out2,true);

        newVideoL << QImage2Mat(out1);
        newVideoR << QImage2Mat(out2);
    }

    newVideoL.release();
    newVideoR.release();
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
    processframes(argv[2],argv[3]);
  // display help
  } else {
    cout << endl;
    cout << "ELAS demo program usage: " << endl;
    cout << "./elas demo ................ process all test images (image dir)" << endl;
    cout << "./elas left.pgm right.pgm .. process a single stereo pair" << endl;
    cout << "./elas video videoLeft.avi videoRight.avi .. process a undistorted stereo pair video" << endl;
    cout << "./elas -h .................. shows this help" << endl;
    cout << endl;
    cout << "Note: All images must be pgm greylevel images. All output" << endl;
    cout << "      disparities will be scaled such that disp_max = 255." << endl;
    cout << endl;
  }

  return 0;
}


