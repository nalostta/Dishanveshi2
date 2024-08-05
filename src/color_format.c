#define DSVI_CONFIG_PUBLIC_IMAGE

#include <dishanveshi/init.h>
#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/HSV.h>
#include <misc.h>

#include "common/image/bitmap/bitmap.h"

int main()
{
    dsvi_status_t _status = dsvi_test_init();
    //
    dsvi_colorFormatRGB rgb = {0};
    dsvi_colorFormatRGB rgb2 = {0};
    dsvi_colorFormatHSV hsv = {0};

    rgb.r = 2;
    rgb.g = 250;
    rgb.b = 1;

    _status = dsvi_image_colorFormat_getHSV(&rgb,&hsv);
    real_t Hue = hsv.h*24/17;
    real_t Sat = hsv.s*100/255;
    real_t Val = hsv.v*100/255;
    printf("\n--------------\nr=%d\tg=%d\tb=%d\nh=%d\ts=%d\tv=%d\n--------------\nexternal debug:H=%f\tSat=%f\tVal=%f\n\n",rgb.r,rgb.g,rgb.b,hsv.h,hsv.s,hsv.v,Hue,Sat,Val);
    checkStatus("converting from rgb to hsv");
    _status = dsvi_image_colorFormat_getRGB(&hsv,&rgb2);
    checkStatus("converting back");
    printf("r=%d\tg=%d\tb=%d\n\n",rgb2.r,rgb2.g,rgb2.b);

    return _status;
}
