#include "undistort_rectify.h"
#include "calib_stereo.h"
#include "calibtemp.h"
#include "cameracalibration.h"

int main(int argc, char const **argv)
{
    printf("Starting the program\n");
    int phase = 2;
    switch (phase) {
    case 0:
        makeCalibration();
        break;
    case 1:
        //calibrateStereo();
        makeRectification();
        break;
    case 2:
        char imgs_directory[] = "../../../Dataset/Calib/right/";
        char imgs_filename[]  = "";
        char out_file[] = "calibRight.txt";
        char extension[] = "png";

        CameraCalibration* cam1 = new CameraCalibration(11,7,25,1,imgs_directory,imgs_filename,extension);
       // CameraCalibration* cam1 = new CameraCalibration(out_file);
        cam1->saveParamsInFile(out_file);
        break;
    case 3:
        break;
    }

    return 0;
  }
