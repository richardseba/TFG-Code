#ifndef CALIB_STEREO_H
#define CALIB_STEREO_H

void load_image_points(int board_width, int board_height, int num_imgs, float square_size,
                      char* leftimg_dir, char* rightimg_dir, char* leftimg_filename, char* rightimg_filename, char* extension);
void calibrateStereo();

#endif // CALIB_STEREO_H
