#include "undistort_rectify.h"
#include "calib_stereo.h"
#include "calibtemp.h"
#include "cameracalibration.h"
#include "stereocalibration.h"

void calib()
{
    char imgs_directory[] = "../../../Dataset/Calib/left/";
    char imgs_filename[]  = "";
    char out_file[] = "calibLeft.txt";
    char extension[] = "png";

    CameraCalibration* cam1 = new CameraCalibration(11,7,25,1,imgs_directory,imgs_filename,extension);
   // CameraCalibration* cam1 = new CameraCalibration(out_file);
    cam1->saveParamsInFile(out_file);
}

void stereo()
{
    char rConfig[] = "calibRight.txt";
    char lConfig[] = "calibLeft.txt";
    CameraCalibration camLeft(lConfig);
    CameraCalibration camRight(rConfig);

    char leftimg_path[] = "../../../Dataset/Calib/left/";
    char rightimg_path[] = "../../../Dataset/Calib/right/";
    char leftimg_filename[] = "../../../Dataset/Calib/left/1.png";
    char rightimg_filename[] = "../../../Dataset/Calib/right/1.png";
    char calib_file[] = "./stereoCalibtemp.txt";
    char img_prefix[]  = "";
    char extension[] = "png";

    StereoCalibration stereoCalib(camLeft,camRight,11,7,25,1,leftimg_path,rightimg_path,
                                  img_prefix,img_prefix,extension);
    stereoCalib.saveParamsInFile(calib_file);
}

int main(int argc, char const **argv)
{
    printf("Starting the program\n");
    int phase = 3;
    switch (phase) {
    case 0:
        makeCalibration();
        break;
    case 1:
        //calibrateStereo();
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
