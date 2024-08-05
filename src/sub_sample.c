#define DSVI_CONFIG_PUBLIC_IMAGE

#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/convolve.h>
#include <misc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bitmap.h>


/*
Using the implemented functions you shall create a test application which will take:
        ->file name, (x,y) and (w,h)
and will create a sampled image from the source, the application shall save the image as ${input_filename}_x_y_w_h.bmp.
*/
int main(int argc,char* argv[])
{
    if(argc!=6)
    {
        printf("\n[error]:invalid number of arguments");
        return DSVI_STATUS_INVALID_ARG;
    }
    dsvi_status_t _status = DSVI_STATUS_OK;

    //Initialization
    DsviImageMemoryConfig_t image_allocator;
    image_allocator.image_alloc = &malloc;
    image_allocator.image_free  = &free;
    dsvi_image_Init(&image_allocator);

    //local variables
    char          filename[DSVI_STR_BUFFER_SIZE]="Bin/Resources/";

    strcat(filename,argv[1]);       //This array is reused first to open the src bmp file
    strcat(filename,".bmp");
    dsvi_Image_t* srcImage  =   dsvi_image_bitmap_read(filename,&_status);
    dsvi_Image_t* newImage  =   NULL;

    //check _status

    if(_status==DSVI_STATUS_OK)
    {
        dsvi_ImageROI_t roi;

        roi.x       =   Str2Int(argv[2]);
        roi.y       =   Str2Int(argv[3]);
        roi.height  =   Str2Int(argv[4]);
        roi.width   =   Str2Int(argv[5]);

        _status     =   dsvi_image_setROI(srcImage,&roi);

        if(_status==DSVI_STATUS_OK)
        {
            newImage    =   dsvi_image_crop(srcImage,&_status);

            if(_status==DSVI_STATUS_OK)
            {
                dsvi_image_bitmap_write(newImage,"bin/Resources/cropped.bmp");
            }
            dsvi_image_delete(newImage);
        }
    }
    dsvi_image_delete(srcImage);
    return _status;
}
