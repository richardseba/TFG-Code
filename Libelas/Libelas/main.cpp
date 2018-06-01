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

  Mat leftim = imread(file_1);
  Mat rightim =imread(file_2);

  Mat leftdisp;
  Mat rightdisp;

  int bd = 0;

  Mat l,r;
  if(leftim.channels()==3){cvtColor(leftim,l,CV_BGR2GRAY);cout<<"convert gray"<<endl;}
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


  qDebug() << imwrite("./test1.pgm",D1);
  qDebug() << imwrite("./test2.pgm",D2);


}

void oldProcess (const char* file_1,const char* file_2, bool colorMap) {
    // load images
      image<uchar> *I1,*I2;
      I1 = loadPGM(file_1);
      I2 = loadPGM(file_2);

      // check for correct size
      if (I1->width()<=0 || I1->height() <=0 || I2->width()<=0 || I2->height() <=0 ||
          I1->width()!=I2->width() || I1->height()!=I2->height()) {
        cout << "ERROR: Images must be of same size, but" << endl;
        cout << "       I1: " << I1->width() <<  " x " << I1->height() <<
                     ", I2: " << I2->width() <<  " x " << I2->height() << endl;
        delete I1;
        delete I2;
        return;
      }

      // get image width and height
      int32_t width  = I1->width();
      int32_t height = I1->height();

      // allocate memory for disparity images
      const int32_t dims[3] = {width,height,width}; // bytes per line = width
      float* D1_data = (float*)malloc(width*height*sizeof(float));
      float* D2_data = (float*)malloc(width*height*sizeof(float));

      // process
      Elas::parameters param;
      param.postprocess_only_left = true;
      Elas elas(param);
      elas.process(I1->data,I2->data,D1_data,D2_data,dims);

      // find maximum disparity for scaling output disparity images to [0..255]
      float disp_max = 0;
      for (int32_t i=0; i<width*height; i++) {
        if (D1_data[i]>disp_max) disp_max = D1_data[i];
        if (D2_data[i]>disp_max) disp_max = D2_data[i];
      }

      qDebug() << disp_max;

      // copy float to uchar
      image<uchar> *D1 = new image<uchar>(width,height);
      image<uchar> *D2 = new image<uchar>(width,height);
      for (int32_t i=0; i<width*height; i++) {
        D1->data[i] = (uint8_t)max(255.0*D1_data[i]/disp_max,0.0);
        D2->data[i] = (uint8_t)max(255.0*D2_data[i]/disp_max,0.0);
      }

      // save disparity images
      char output_1[1024];
      char output_2[1024];
      strncpy(output_1,file_1,strlen(file_1)-4);
      strncpy(output_2,file_2,strlen(file_2)-4);
      output_1[strlen(file_1)-4] = '\0';
      output_2[strlen(file_2)-4] = '\0';
      strcat(output_1,"_disp.pgm");
      strcat(output_2,"_disp.pgm");
      savePGM(D1,"./test1_old.pgm");
      savePGM(D2,"./test2_old.pgm");

      // free memory
      delete I1;
      delete I2;
      delete D1;
      delete D2;
      free(D1_data);
      free(D2_data);

}

void process (Mat I1,Mat I2, QImage &Im1, QImage &Im2, bool colorMap) {

  int bd = 0;
  Mat l,r;
  if(I1.channels()==3){cvtColor(I1,l,CV_BGR2GRAY);}
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
  Elas elas(Elas::setting::MIDDLEBURY);
  elas.process(lb.data,rb.data,leftdpf.ptr<float>(0),rightdpf.ptr<float>(0),dims);

  Mat disp;
  Mat(leftdpf(cv::Rect(bd,0,I1.cols,I1.rows))).copyTo(disp);
  disp.convertTo(I1,CV_8UC1,16);
  Mat(rightdpf(cv::Rect(bd,0,I2.cols,I2.rows))).copyTo(disp);
  disp.convertTo(I2,CV_8UC1,16);

  Mat temp;
  cvtColor(I1,temp,CV_GRAY2RGB);
  QImage D1 = Mat2QImage(temp);
  cvtColor(I2,temp,CV_GRAY2RGB);
  QImage D2 = Mat2QImage(temp);

  if(colorMap)
  {
      Mat colormapL, colormapR;

      applyColorMap(I1,colormapL,COLORMAP_JET);
      applyColorMap(I2,colormapR,COLORMAP_JET);

      D1 = Mat2QImage(colormapL);
      D2 = Mat2QImage(colormapR);
  }

  Im1 = D1;
  Im2 = D2;

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
    bool colormap = false;

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


