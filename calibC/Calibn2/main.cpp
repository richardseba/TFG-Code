#include "undistort_rectify.h"
#include "calib_stereo.h"
#include "calibtemp.h"

int main(int argc, char const **argv)
{
    printf("Starting the program\n");
    int phase = 0;
    switch (phase) {
    case 0:
        makeCalibration();
        break;
    case 1:
        calibrateStereo();
        makeRectification();
        break;
    default:
        printf("nothing selected\n");
        break;
    }

    return 0;
  }
