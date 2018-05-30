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
void process (const char* file_1,const char* file_2) {

  cout << "Processing: " << file_1 << ", " << file_2 << endl;
  QImage I1 = QImage(file_1);
  QImage I2 = QImage(file_2);

  // check for correct size
   if (I1.width()<=0 || I1.height() <=0 || I2.width()<=0 || I2.height() <=0 ||
       I1.width()!=I2.width() || I1.height()!=I2.height()) {
     qDebug() << "ERROR: Images must be of same size, but" << endl;
     qDebug() << "       I1: " << I1.width() <<  " x " << I1.height() <<
                  ", I2: " << I2.width() <<  " x " << I2.height() << endl;
     return;
   }

  // get image width and height
   int32_t width  = I1.width();
   int32_t height = I1.height();


  // allocate memory for disparity images
  const int32_t dims[3] = {width,height,width}; // bytes per line = width
  float* D1_data = (float*)malloc(width*height*sizeof(float));
  float* D2_data = (float*)malloc(width*height*sizeof(float));

  // process

  Elas::parameters param;
  param.postprocess_only_left = true;
  //Elas elas(param);

  Elas elas(Elas::setting::MIDDLEBURY);
  elas.process(I1.bits(),I2.bits(),D1_data,D2_data,dims);

  // find maximum disparity for scaling output disparity images to [0..255]
  float disp_max = 0;
  for (int32_t i=0; i<width*height; i++) {
    if (D1_data[i]>disp_max) disp_max = D1_data[i];
    if (D2_data[i]>disp_max) disp_max = D2_data[i];
  }

  QImage D1((int)width,(int)height,QImage::Format_Grayscale8);
  QImage D2((int)width,(int)height,QImage::Format_Grayscale8);

  for (int32_t i=0; i<width*height; i++) {
    D1.bits()[i] = (uint8_t)max(255.0*D1_data[i]/disp_max,0.0);
    D2.bits()[i] = (uint8_t)max(255.0*D2_data[i]/disp_max,0.0);
  }


  Mat imgL(D1.height(),D1.width(),CV_8UC1,(uchar*)D1.bits(),D1.bytesPerLine());
  Mat imgR(D2.height(),D2.width(),CV_8UC1,(uchar*)D2.bits(),D2.bytesPerLine());

  Mat colormapL, colormapR;

  applyColorMap(imgL,colormapL,COLORMAP_JET);
  applyColorMap(imgR,colormapR,COLORMAP_JET);

  imwrite("l.png",imgL);
  imwrite("r.png",imgR);

  free(D1_data);
  free(D2_data);
}

int main (int argc, char** argv) {
    QTime crono = QTime();
    crono.start();
  // run demo
  if (argc==2 && !strcmp(argv[1],"demo")) {
    process("img/cones_left.pgm",   "img/cones_right.pgm");
    process("img/aloe_left.pgm",    "img/aloe_right.pgm");
    process("img/raindeer_left.pgm","img/raindeer_right.pgm");
    process("img/urban1_left.pgm",  "img/urban1_right.pgm");
    process("img/urban2_left.pgm",  "img/urban2_right.pgm");
    process("img/urban3_left.pgm",  "img/urban3_right.pgm");
    process("img/urban4_left.pgm",  "img/urban4_right.pgm");
    cout << "... done!" << endl;
    qDebug() << crono.restart()/1000.0;

  // compute disparity from input pair
  } else if (argc==3) {
    process(argv[1],argv[2]);
    cout << "... done!" << endl;

  // display help
  } else {
    cout << endl;
    cout << "ELAS demo program usage: " << endl;
    cout << "./elas demo ................ process all test images (image dir)" << endl;
    cout << "./elas left.pgm right.pgm .. process a single stereo pair" << endl;
    cout << "./elas -h .................. shows this help" << endl;
    cout << endl;
    cout << "Note: All images must be pgm greylevel images. All output" << endl;
    cout << "      disparities will be scaled such that disp_max = 255." << endl;
    cout << endl;
  }

  return 0;
}


