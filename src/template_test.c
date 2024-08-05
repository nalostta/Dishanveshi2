#define DSVI_CONFIG_PUBLIC_IMAGE

#include <dishanveshi/types.h>
#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/HSV.h>
#include <dishanveshi/image/operations/template.h>
#include "common/image/bitmap/bitmap.h"

int main()
{
    //config image init functions
    DsviImageMemoryConfig_t image_allocator;

    image_allocator.image_alloc =   &malloc;
    image_allocator.image_free  =   &free;
    dsvi_status_t  _status      =   dsvi_image_Init(&image_allocator);
    //
    dsvi_Image_t* srcImage  =   dsvi_image_bitmap_read("bin/resources/input.bmp",&_status);

    typedef struct
    {
        uint16_t x;
        uint16_t y;
        uint16_t* parameters;
        enShape  templateShape;
    }dsvi_template_t;

    dsvi_template_t* cutout =  malloc(sizeof(dsvi_template_t));
    cutout->x    =   0;
    cutout->y    =   100;
    cutout->parameters       =   malloc(2*sizeof(uint16_t));
    cutout->parameters[0]    =   51;
    cutout->parameters[1]    =   20;
    cutout->templateShape    =   enTrapezium;

    dsvi_Image_t* monoImage =   dsvi_image_convertToDefault(srcImage,&_status);
    dsvi_Image_t* cutoutIm  =   dsvi_image_getTemplate(monoImage,cutout,0,&_status);
    dsvi_Image_t* newImage  =   dsvi_image_convertTo888(cutoutIm,&_status);


    dsvi_image_bitmap_write(newImage,"bin/resources/output.bmp");

    free(cutout->parameters);
    free(cutout);
    dsvi_image_delete(srcImage);
    dsvi_image_delete(monoImage);
    dsvi_image_delete(cutoutIm);
    dsvi_image_delete(newImage);
    return _status;
}
