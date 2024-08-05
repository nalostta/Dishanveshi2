#define DSVI_CONFIG_PUBLIC_IMAGE
#define _DSVI_LABELBITS_8

#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/convolve.h>
#include <dishanveshi/image/operations/CCL.h>
#include <dishanveshi/image/operations/filter.h>
#include <bitmap.h>
#include <stdio.h>

#define checkStatus(M, ...) if(_status==DSVI_STATUS_OK){printf( M ": OK\n",##__VA_ARGS__);}else{printf( M ": Error : %d\n",_status,##__VA_ARGS__);}

/**
 *
 * need to rename fns in all the modules of image op
 *
 */



int main()
{
    DsviImageMemoryConfig_t config;
    config.image_alloc  =   &malloc;
    config.image_free   =   &free;
    dsvi_status_t _status = dsvi_image_Init(&config);
    checkStatus("image memory init");

    dsvi_Image_t* srcImage  =   dsvi_image_bitmap_read("bin/resources/input.bmp",&_status);
    checkStatus("image read");

    // kernel
    dsvi_RealKernel_t k = {0};
    k.height = 5;
    k.width  = 5;
    k.el = malloc(25*sizeof(real_t));
    int i;
    for(i=0;i<25;i++)k.el[i] = 0;
    k.el[0] = 1;
    k.el[1] = 4;
    k.el[2] = 7;
    k.el[3] = 4;
    k.el[4] = 1;

    k.el[5] = 4;
    k.el[6] = 16;
    k.el[7] = 26;
    k.el[8] = 16;
    k.el[9] = 4;

    k.el[10] = 7;
    k.el[11] = 26;
    k.el[12] = 41;
    k.el[13] = 26;
    k.el[14] = 7;

    k.el[15] = 4;
    k.el[16] = 16;
    k.el[17] = 26;
    k.el[18] = 16;
    k.el[19] = 4;

    k.el[20] = 1;
    k.el[21] = 4;
    k.el[22] = 7;
    k.el[23] = 4;
    k.el[24] = 1;
    for(i=0;i<25;i++)k.el[i] = k.el[i]/273;
    //

    dsvi_Image_t* defImage = dsvi_image_convertToDefault(srcImage,&_status);
    checkStatus("converting 888 to default");

    dsvi_Matrix_t* srcMatrix = dsvi_image_getMtxFromImage(defImage,&_status);
    checkStatus("extracting matrix from defImage");

    dsvi_RealMatrix_t* newMatrix = dsvi_image_RKConvolve(srcMatrix,&k,&_status);
    checkStatus("convolving image matrix with kernel");

    dsvi_Image_t* newImage = dsvi_image_getImageFromRealMatrix(newMatrix,&_status);
    checkStatus("forming image from matrix");

    dsvi_Image_t* newImage888 = dsvi_image_convertTo888(newImage,&_status);
    checkStatus("converting newImage to RGB888 format");

    _status = dsvi_image_bitmap_write(newImage888,"bin/resources/convolved.bmp");
    checkStatus("writing final image");

    dsvi_Image_t* defImage888 = dsvi_image_convertTo888(defImage,&_status);

    _status = dsvi_image_bitmap_write(defImage888,"bin/resources/debug.bmp");
    checkStatus("writing debug image");

    dsvi_Image_t* thImage = dsvi_image_threshold(newImage,200,&_status);
    checkStatus("thresholding image");

    dsvi_Image_t* thImage888 = dsvi_image_convertTo888(thImage,&_status);
    checkStatus("thImage888");

    _status = dsvi_image_bitmap_write(thImage888,"bin/resources/thImage888.bmp");
    checkStatus("writing thresholded image");

    _status = dsvi_imageOp_label(thImage,200);
    dsvi_Image_t* debugImage = dsvi_imageOp_debugLabels(thImage,&_status);
    checkStatus("debugging labels");

    dsvi_Image_t* debugImage888 = dsvi_image_convertTo888(debugImage,&_status);

    dsvi_image_bitmap_write(debugImage888,"bin/resources/labelDebugImage.bmp");
    checkStatus("writing debug image");

    free(k.el);
    free(srcMatrix->el);
    free(srcMatrix);
    free(newMatrix->el);
    free(newMatrix);
    dsvi_image_delete(defImage);
    checkStatus("deleting defImage");
    dsvi_image_delete(srcImage);
    checkStatus("deleting srcImage");
    dsvi_image_delete(newImage);
    checkStatus("deleting newImage");
    dsvi_image_delete(newImage888);
    checkStatus("deleting newImage888");
    dsvi_image_delete(defImage888);
    dsvi_image_delete(thImage);

    return _status;
}
