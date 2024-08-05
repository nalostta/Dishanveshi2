#include <dishanveshi/image/operations/dither.h>
#include <stdlib.h>

dsvi_Image_t* dsvi_image_dither(const dsvi_Image_t* srcImage,uint8_t threshold, dsvi_status_t* status)
{
	dsvi_status_t _status = DSVI_STATUS_OK;
	dsvi_Image_t* newImage = NULL;
	if(srcImage==NULL)
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        if(srcImage->format!=enDsviImagePixelFormat_DEFAULT)
        {
            _status = DSVI_STATUS_NOT_SUPPORTED;
        }else
        {
            uint32_t size = srcImage->width*srcImage->height;

            ImageInfo_t newInfo = {0};
            newInfo.format    = srcImage->format;
            newInfo.width     = srcImage->width;
            newInfo.height    = srcImage->height;

            newImage = dsvi_image_new(&newInfo,&_status);

            if(_status==DSVI_STATUS_OK)
            {
                uint16_t temp=0;

                for(uint32_t i=0;i<size;i++)
                {
                     temp = srcImage->data[i] + (rand()%90);
                     newImage->data[i] = ((temp>=threshold)? 255:0);
                }
            }
        }
    }
    if(status!=NULL) *status = _status;
    return newImage;
}
