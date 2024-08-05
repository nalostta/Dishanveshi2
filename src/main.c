#define DSVI_CONFIG_PUBLIC_IMAGE
#include <stdio.h>
#include <stdlib.h>
#include <dishanveshi/types.h>
#include <dishanveshi/image/image.h>

int main()
{
    //
    DsviImageMemoryConfig_t image_allocator;
    image_allocator.image_alloc=&malloc;
    image_allocator.image_free=&free;
    dsvi_status_t status = dsvi_image_Init(&image_allocator);
    //
    ImageInfo_t  ImageInfo1;
    ImageInfo1.height=4;
    ImageInfo1.width=4;
    ImageInfo1.format=enDvsiImagePixelFormat_RGB565;
    ImageInfo1.extra=NULL;
    //
    dsvi_Image_t* Image1 = (dsvi_Image_t*)dsvi_image_new(&ImageInfo1,&status);
    dsvi_Image_t* Image2 = NULL;
    //
    int i;
    for(i=0;i<16;i++)
    {
        Image1->data[i] = (i*(i%5))-((i>0)? Image1->data[i-1]:0);
    }
    dsvi_Image_clone(Image1,&Image2);
    dsvi_Image_writeToFile(Image2,"img_dump.bmp");
    //
    dsvi_image_delete(Image1);
    dsvi_image_delete(Image2);

    return 0;
}
