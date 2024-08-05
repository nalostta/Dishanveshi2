#define DSVI_CONFIG_PUBLIC_IMAGE

#include <dishanveshi/init.h>
#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/HSV.h>
#include <dishanveshi/image/operations/histogram.h>
#include <misc.h>

#include "common/image/bitmap/bitmap.h"

int main()
{
    dsvi_status_t _status = dsvi_test_init();

    dsvi_Image_t* srcImage = dsvi_image_bitmap_read("bin/resources/input.bmp",&_status);
    checkStatus("reading src image");

    dsvi_Image_t* hsvImage = dsvi_image_getHSV(srcImage,&_status);
    checkStatus("converting image to hsv format");
    dsvi_Image_t* newImage = dsvi_image_getRGB(hsvImage,&_status);
    checkStatus("converting image back to rgb format");

    _status = dsvi_image_bitmap_write(newImage,"bin/resources/reconvertedImage.bmp");
    checkStatus("writing new image");

    _status = dsvi_image_delete(srcImage);
    checkStatus("deleting srcImage");
    _status = dsvi_image_delete(hsvImage);
    checkStatus("deleting hsvImage");
    _status = dsvi_image_delete(newImage);
    checkStatus("deleting newImage");

    return _status;
}
