#ifndef CALIBTEMP_H
#define CALIBTEMP_H

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

void setup_calibration(int board_width, int board_height, int num_imgs,
                       float square_size, char* imgs_directory, char* imgs_filename,
                       char* extension);

double computeReprojectionErrors(const vector< vector< Point3f > >& objectPoints,
                                 const vector< vector< Point2f > >& imagePoints,
                                 const vector< Mat >& rvecs, const vector< Mat >& tvecs,
                                 const Mat& cameraMatrix , const Mat& distCoeffs);

void makeCalibration();

#endif // CALIBTEMP_H
