#define DSVI_CONFIG_PUBLIC_IMAGE
#define DSVI_CONFIG_PUBLIC_HISTOGRAM

#include <stdio.h>
#include <stdlib.h>
#include <dishanveshi/types.h>
#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/CCL.h>
#include <dishanveshi/image/operations/histogram.h>
#include "common/image/bitmap/bitmap.h"

#define checkStatus(M, ...) if(_status==DSVI_STATUS_OK){printf( M ": OK\n",##__VA_ARGS__);}else{printf( M ": Error : %d\n",_status,##__VA_ARGS__);}

int main()
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;

    //config image init functions
    DsviImageMemoryConfig_t image_allocator;
    image_allocator.image_alloc=&malloc;
    image_allocator.image_free=&free;
    _status = dsvi_image_Init(&image_allocator);
    //

    dsvi_Image_t* srcImage  =   dsvi_image_bitmap_read("bin/resources/input.bmp",&_status);
    checkStatus("reading input.bmp");
    //
    uint8_t a[255];
    int j;
    for(j=0;j<255;j++) a[j]=j+1;

    dsvi_histogramInfo_t info;
    info.bins    =  255;
    info.bounds  =  &a;

    dsvi_Image_t* defImage          =   dsvi_image_convertToDefault(srcImage,&_status);
    checkStatus("conversion to default");

    dsvi_Image_t* defImage888        =   dsvi_image_convertTo888(defImage,&_status);
    _status =   dsvi_image_bitmap_write(defImage888,"bin/resources/grayscale.bmp");
    checkStatus("printing grayscale image");

    dsvi_histogram_t* srcHistogram  =   dsvi_image_getHistogram(defImage,&info,&_status);
    checkStatus("obtain histogram");

    int i;
    for(i=0;i<info.bins;i++)
    {
        printf("%d : %d\n",i,srcHistogram->data[i]);
    }

    dsvi_Image_t* hImage = dsvi_image_histogramToImage(srcHistogram,&_status);
    checkStatus("plotting histogram into an image");
    dsvi_Image_t* hImage888 = dsvi_image_convertTo888(hImage,&_status);
    checkStatus("conversion to 888");
    dsvi_image_bitmap_write(hImage888,"bin/resources/histogram.bmp");
    checkStatus("writing image");


    dsvi_image_delete(srcImage);
    dsvi_image_delete(defImage);
    dsvi_image_delete(hImage);
    dsvi_image_delete(hImage888);
    dsvi_image_histogram_delete(srcHistogram);

    return _status;
    //
}
