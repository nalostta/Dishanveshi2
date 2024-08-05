#include <dishanveshi/image/operations/region.h>
/*
dsvi_Image_t* dsvi_image_getHueSatBasedRegion_fromCutOut(dsvi_Image_t* srcImage,dsvi_template_t* srcTemplate,dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();

    if(_status == DSVI_STATUS_OK)
    {
        if((srcImage==NULL)||(srcTemplate==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format!=enDsviImagePixelFormat_RGB888)
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                //gaussian filtering
                dsvi_RealKernel_t gaussian;
                gaussian.width  = 5;
                gaussian.height = 5;
                gaussian.el = malloc(25*sizeof*gaussian.el);

                gaussian.el[0] = 1.0/273;
                gaussian.el[1] = 4.0/273;
                gaussian.el[2] = 7.0/273;
                gaussian.el[3] = 4.0/273;
                gaussian.el[4] = 1.0/273;
                gaussian.el[5] = 4.0/273;
                gaussian.el[6] = 16.0/273;
                gaussian.el[7] = 26.0/273;
                gaussian.el[8] = 16.0/273;
                gaussian.el[9] = 4.0/273;
                gaussian.el[10] = 7.0/273;
                gaussian.el[11] = 26.0/273;
                gaussian.el[12] = 41.0/273;
                gaussian.el[13] = 26.0/273;
                gaussian.el[14] = 7.0/273;
                gaussian.el[15] = 4.0/273;
                gaussian.el[16] = 16.0/273;
                gaussian.el[17] = 26.0/273;
                gaussian.el[18] = 16.0/273;
                gaussian.el[19] = 4.0/273;
                gaussian.el[20] = 1.0/273;
                gaussian.el[21] = 4.0/273;
                gaussian.el[22] = 7.0/273;
                gaussian.el[23] = 4.0/273;
                gaussian.el[24] = 1.0/273;

                dsvi_ImageROI_t roi = {0};
                roi.x = 2;
                roi.x = 2;
                roi.width = srcImage->width-4;
                roi.height = srcImage->height-4;
                dsvi_Image_t* croppedImage = dsvi_image_cropEx(srcImage,&roi,&_status);

            }
        }
    }
}*/
