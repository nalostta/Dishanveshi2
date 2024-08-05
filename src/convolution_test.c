#define DSVI_CONFIG_PUBLIC_IMAGE

#include <dishanveshi/types.h>
#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/convolve.h>
#include <dishanveshi/image/operations/misc.h>
#include <bitmap.h>
#include <stdlib.h>

int main()
{
    //Initialization
    DsviImageMemoryConfig_t image_allocator;
    image_allocator.image_alloc = &malloc;
    image_allocator.image_free  = &free;
    dsvi_image_Init(&image_allocator);


    dsvi_status_t _status = DSVI_STATUS_OK;

    int i=0;
    dsvi_Kernel_t k;
    k.width  = 3;
    k.height = 3;
    k.el     = (int16_t*)malloc(9*sizeof(int16_t));
    for(i=0;i<9;i++)
    {
        k.el[i]=0;
    }
    k.el[0]=0;
    k.el[1]=-1;
    k.el[2]=0;

    k.el[3]=-1;
    k.el[4]=4;
    k.el[5]=-1;

    k.el[6]=0;
    k.el[7]=-1;
    k.el[8]=0;

    dsvi_printKernel(&k);

    dsvi_Image_t*   image1  =   dsvi_image_bitmap_read("Bin/Resources/input.bmp",&_status);
    dsvi_Image_t*   image2  =   dsvi_image_convertToDefault(image1,&_status);
    dsvi_Matrix_t*  matrix  =   dsvi_image_getMtxFromImage(image2,&_status);
    dsvi_Matrix_t*  matrix2 =   dsvi_image_convolve(matrix,&k,&_status);
    dsvi_Image_t*   image3  =   dsvi_image_getImageFromMtx(matrix2,&_status);
    dsvi_Image_t*   image4  =   dsvi_image_convertTo888(image3,&_status);

    dsvi_image_bitmap_write(image4,"Bin/Resources/output1.bmp");

    dsvi_image_delete(image1);
    dsvi_image_delete(image2);
    dsvi_image_delete(image3);
    dsvi_image_delete(image4);

    return _status;
}
