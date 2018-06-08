#include "undistort_rectify.h"
#include "calib_stereo.h"
#include "calibtemp.h"
#include "cameracalibration.h"
#include "stereocalibration.h"

void calib()
{
//    char imgs_directory[] = "C:/Users/rsegovia/Desktop/Dataset/1100x1100 30 may/right";
//    char imgs_directory[] = "C:/Users/rsegovia/Desktop/Dataset/1100x1100 30 may/left";
    char imgs_directory[] = "C:/Users/rsegovia/Desktop/TFG/TFG-Code/calibC/1/";
    char imgs_filename[]  = "right";
    char out_file[] = "calibRight.txt";
//    char out_file[] = "calibLeft.txt";
    char extension[] = "jpg";

    CameraCalibration* cam1 = new CameraCalibration(11,7,16,2.4,imgs_directory,imgs_filename,extension);
   // CameraCalibration* cam1 = new CameraCalibration(out_file);
    cam1->saveParamsInFile(out_file);
}

void stereo()
{
    char rConfig[] = "calibRight.txt";
    char lConfig[] = "calibLeft.txt";
    CameraCalibration camLeft(lConfig);
    CameraCalibration camRight(rConfig);

//    char leftimg_path[] = "C:/Users/rsegovia/Desktop/Dataset/1100x1100 30 may/left/";
    char leftimg_path[] = "C:/Users/rsegovia/Desktop/TFG/TFG-Code/calibC/1/";
//    char rightimg_path[] = "C:/Users/rsegovia/Desktop/Dataset/1100x1100 30 may/right/";
    char rightimg_path[] = "C:/Users/rsegovia/Desktop/TFG/TFG-Code/calibC/1/";
//    char leftimg_filename[] = "C:/Users/rsegovia/Desktop/Dataset/1100x1100 30 may/left/1.png";
    char leftimg_filename[] = "C:/Users/rsegovia/Desktop/TFG/TFG-Code/calibC/1/left1.jpg";
//    char rightimg_filename[] = "C:/Users/rsegovia/Desktop/Dataset/1100x1100 30 may/right/1.png";
    char rightimg_filename[] = "C:/Users/rsegovia/Desktop/TFG/TFG-Code/calibC/1/right1.jpg";
    char calib_file[] = "./stereoCalib.txt";
    char img_left_prefix[]  = "left";
    char img_right_prefix[]  = "right";
    char extension[] = "jpg";

    StereoCalibration stereoCalib(camLeft,camRight,11,7,13,1,leftimg_path,rightimg_path,
                                  img_left_prefix,img_right_prefix,extension);
    stereoCalib.saveParamsInFile(calib_file);

    stereoCalib.initUndistortImage();

    Mat imgLeft,imgRight,imgLU,imgRU;

    imgLeft = imread(leftimg_filename, CV_LOAD_IMAGE_COLOR);
    imgRight = imread(rightimg_filename, CV_LOAD_IMAGE_COLOR);

    imgLU = stereoCalib.undistortLeft(imgLeft, cv::INTER_LINEAR);
    imgRU = stereoCalib.undistortRight(imgRight, cv::INTER_LINEAR);

    imwrite("imgLU.png", imgLU);
    imwrite("imgRU.png", imgRU);
}

int main(int argc, char const **argv)
{
    printf("Starting the program\n");
    int phase = 1;
    switch (phase) {
    case 0:
        makeCalibration();
        break;
    case 1:
        calibrateStereo();
        makeRectification();
        break;
    case 2:
        calib();
        break;
    case 3:
        stereo();
        break;
    }

    return 0;
}
