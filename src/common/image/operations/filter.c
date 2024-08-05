#include <dishanveshi/image/operations/filter.h>
#include <dishanveshi/image/operations/convolve.h>

#define DSVI_IMAGEOP_MEDIANFILTER_MAXFRAMESIZE 99
#define DSVI_IMAGEOP_AMF_MAX_FRAME_LEN 15
#define DSVI_IMAGEOP_AMF_MAX_FRAME_SIZE DSVI_IMAGEOP_AMF_MAX_FRAME_LEN*DSVI_IMAGEOP_AMF_MAX_FRAME_LEN

#define isNoisy(a,bandpassL,bandpassH) ((a<=bandpassL)||(a>=bandpassH))
#define frameWithinImage(pixIndex) ((pixIndex>=(windowLen/2))? 1:0)
#define IsFrameInsideImage(row_i,col_i) (frameWithinImage(row_i)&&frameWithinImage(newImage->height-row_i)&&frameWithinImage(col_i)&&frameWithinImage(newImage->width-col_i))

dsvi_Image_t* dsvi_image_threshold(const dsvi_Image_t* srcImage,uint8_t threshold,dsvi_status_t* status)
{
    dsvi_status_t _status   = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage  = NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if(srcImage->format!=enDsviImagePixelFormat_DEFAULT)
        {
            _status =   DSVI_STATUS_NOT_SUPPORTED;
        }else
        {
            ImageInfo_t newInfo;
            newInfo.height = srcImage->height;
            newInfo.width  = srcImage->width;
            newInfo.format = enDsviImagePixelFormat_DEFAULT;

            newImage = dsvi_image_new(&newInfo,&_status);

            if(_status==DSVI_STATUS_OK)
            {
                uint16_t rowIndex=0,colIndex=0;
                uint32_t elIndex=0;

                for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                {
                    elIndex =   rowIndex*srcImage->width;
                    for(colIndex=0;colIndex<srcImage->width;colIndex++)
                    {
                        if(srcImage->data[elIndex]>=threshold)
                        {
                            newImage->data[elIndex]    =   255;
                        }else
                        {
                            newImage->data[elIndex]    =   0;
                        }
                        elIndex++;
                    }
                }
            }
        }
    }

    if(status!=NULL)
    {
        (*status)   =   _status;
    }

    return newImage;
}


dsvi_Image_t* dsvi_image_bandThreshold(const dsvi_Image_t* srcImage,uint8_t lower,uint8_t upper,dsvi_status_t* status)
{
    dsvi_status_t _status   = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage  = NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if(srcImage->format!=enDsviImagePixelFormat_DEFAULT)
        {
            _status =   DSVI_STATUS_NOT_SUPPORTED;
        }else
        {
            ImageInfo_t newInfo;
            newInfo.height = srcImage->height;
            newInfo.width  = srcImage->width;
            newInfo.format = enDsviImagePixelFormat_DEFAULT;

            newImage = dsvi_image_new(&newInfo,&_status);

            if(lower<upper)
            {
                if(_status==DSVI_STATUS_OK)
                {
                    uint16_t rowIndex=0,colIndex=0;
                    uint32_t elIndex=0;

                    for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                    {
                        elIndex =   rowIndex*srcImage->width;
                        for(colIndex=0;colIndex<srcImage->width;colIndex++)
                        {
                            if((srcImage->data[elIndex]>=lower)&&(srcImage->data[elIndex]<=upper))
                            {
                                newImage->data[elIndex]    =   255;
                            }else
                            {
                                newImage->data[elIndex]    =   0;
                            }
                            elIndex++;
                        }
                    }
                }
            }else
            {
                _status = DSVI_STATUS_INVALID_ARG;
            }
        }
    }

    if(status!=NULL)
    {
        (*status)   =   _status;
    }

    return newImage;
}


dsvi_Image_t* dsvi_image_medianFilter(const dsvi_Image_t* srcImage,uint8_t frameSize,dsvi_status_t* status)
{
    dsvi_status_t _status  = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;

    if(_status == DSVI_STATUS_OK)
    {
        if((srcImage == NULL)||(status==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if((frameSize>=srcImage->width)||(frameSize>=srcImage->height)||(frameSize>DSVI_IMAGEOP_MEDIANFILTER_MAXFRAMESIZE)||(srcImage->format!=enDsviImagePixelFormat_DEFAULT))
            {
                _status = DSVI_STATUS_INVALID_ARG;
            }else
            {
                ImageInfo_t newInfo = {0};
                newInfo.height = srcImage->height - frameSize+1;
                newInfo.width  = srcImage->width  - frameSize+1;
                newInfo.format = enDsviImagePixelFormat_DEFAULT;

                newImage = dsvi_image_new(&newInfo,&_status);

                if(_status == DSVI_STATUS_OK)
                {
                    uint16_t row_i,col_i,array_i;
                    uint16_t latch_x,latch_y;
                    uint32_t src_i=0,dest_i=0;
                    uint8_t  array[frameSize*frameSize];

                    for(row_i=0;row_i<newImage->height;row_i++)
                    {
                        for(col_i=0;col_i<newImage->width;col_i++)
                        {
                            src_i = ((row_i*srcImage->width)+col_i);
                            array_i=0;
                            //fill up the array
                            for(latch_y=0;latch_y<frameSize;latch_y++)
                            {
                                for(latch_x=0;latch_x<frameSize;latch_x++)
                                {
                                    array[array_i] = srcImage->data[src_i++];
                                    array_i++;
                                }
                                src_i+= (srcImage->width-frameSize);
                            }

                            for(latch_y=0;latch_y<(frameSize*frameSize);latch_y++)
                            {
                                for(latch_x=0;latch_x<(frameSize*frameSize)-latch_y-1;latch_x++)
                                {
                                    if(array[latch_x]>array[latch_x+1])
                                    {
                                        int temp = array[latch_x];
                                        array[latch_x]   = array[latch_x+1];
                                        array[latch_x+1] = temp;
                                    }
                                }
                            }
                            newImage->data[dest_i] = array[((frameSize*frameSize)/2)];
                            dest_i++;
                        }
                    }
                }
            }
        }
    }
    (*status) = _status;
    return newImage;
}

dsvi_Image_t* dsvi_image_medianFilterAdaptive(const dsvi_Image_t* srcImage,uint8_t bandpassL,uint8_t bandpassH,dsvi_status_t* status)
{
    dsvi_status_t _status  = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;
    //extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(_status == DSVI_STATUS_OK)
    {
        if((srcImage==NULL)||(status==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format!=enDsviImagePixelFormat_DEFAULT)
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                if(bandpassL>=bandpassH)
                {
                    _status = DSVI_STATUS_INVALID_ARG;
                }else
                {
                    ImageInfo_t newInfo = {0};
                    newInfo.height = srcImage->height;
                    newInfo.width  = srcImage->width;
                    newInfo.format = enDsviImagePixelFormat_DEFAULT;

                    newImage = dsvi_image_new(&newInfo,&_status);

                    if(_status == DSVI_STATUS_OK)
                    {
                        uint8_t buffer[DSVI_IMAGEOP_AMF_MAX_FRAME_SIZE];

                        if(buffer == NULL)
                        {
                            _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                        }else
                        {
                            uint16_t row_i=0,col_i=0;
                            uint32_t pix_i=0;
                            //frame variables
                            int16_t windowLen = -1;
                            uint32_t noiseFree = 0;
                            uint16_t fx_i=0,fy_i=0,fel_i=0,buf_i=0;

                            //
                            for(row_i=0;row_i<newImage->height;row_i++)
                            {
                                for(col_i=0;col_i<newImage->width;col_i++)
                                {
                                    //check noise
                                    pix_i = ((row_i*srcImage->width)+col_i);
                                    if(!isNoisy(srcImage->data[pix_i],bandpassL,bandpassH))
                                    {
                                        //pixel isn't noisy
                                        newImage->data[pix_i] = srcImage->data[pix_i];
                                    }else
                                    {
                                        //pixel is noisy
                                        windowLen = -1;
                                        do
                                        {
                                            windowLen+= 2; //windowLen always init@ 1 in the first iteration
                                            fx_i=0;
                                            fy_i=0;
                                            fel_i=0;
                                            buf_i=0;
                                            noiseFree = 0;

                                            /*boundary conditions satisfied & window size ain't that big*/
                                            if(IsFrameInsideImage(row_i,col_i)&&(windowLen<=DSVI_IMAGEOP_AMF_MAX_FRAME_LEN))
                                            {
                                                for(fy_i= row_i-(windowLen/2);fy_i< row_i+(windowLen/2);fy_i++)
                                                {
                                                    for(fx_i= col_i-(windowLen/2);fx_i< col_i+(windowLen/2);fx_i++)
                                                    {
                                                        fel_i = ((fy_i*newImage->width)+fx_i);
                                                        buffer[buf_i++] = srcImage->data[fel_i];
                                                        if(!isNoisy(srcImage->data[fel_i],bandpassL,bandpassH))
                                                        {
                                                            noiseFree++;
                                                        }
                                                    }
                                                }
                                            }else
                                            {
                                                break;
                                            }
                                        }while((noiseFree<8)&&(windowLen<=DSVI_IMAGEOP_AMF_MAX_FRAME_LEN)&&IsFrameInsideImage(row_i,col_i));

                                        //i got window size,now find m(x,y) and add it to newImage->data[index]...
                                        //printf("\nwindow = %d",windowLen);
                                        if(windowLen>=3)
                                        {
                                            newImage->data[pix_i] = dsvi_image_getMedianVal(&buffer[0],windowLen,NULL);
                                        }else
                                        {
                                            newImage->data[pix_i] = srcImage->data[pix_i];
                                        }
                                    }
                                }
                            }
                            //(*_DsviMemHandle.image_free)(buffer);
                        }
                    }
                }
            }
        }
    }
    if(status!=NULL)
    {
        (*status) = _status;
    }
    return newImage;
}

uint8_t dsvi_image_getMedianVal(uint8_t* buffer,uint8_t windowLen,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    uint8_t temp = 0;

    if((buffer==NULL)||(windowLen<3))
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        //WARNING THE SIZE OF THE GIVEN ARRAY IS NOT TESTED FOR THE TOTAL WINDOW SIZE, THIS COULD POTENTIALLY LEAD TO THE PROGRAM TO CRASH!
        uint16_t i,j;
        for(i=0;i<(windowLen*windowLen);i++)
        {
            for(j=0;j<(windowLen*windowLen)-i-1;j++)
            {
                if(buffer[j]>buffer[j+1])
                {
                    temp = buffer[j];
                    buffer[j] = buffer[j+1];
                    buffer[j+1] = temp;
                }

            }
        }
    }
    if(status!=NULL)
    {
        (*status) = _status;
    }
    temp = (windowLen*windowLen)/2;
    return buffer[temp];
}

dsvi_Image_t* dsvi_image_gaussianFilter_ch1sz5(const dsvi_Image_t* srcImage,dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    dsvi_Image_t* newImage = NULL;
    if(_status==DSVI_STATUS_OK)
    {
        if(srcImage==NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format!=enDsviImagePixelFormat_DEFAULT)
            {
                _status = DSVI_STATUS_INVALID_ARG;
            }else
            {
                dsvi_RealKernel_t gaussian;
                gaussian.width  = 5;
                gaussian.height = 5;
                gaussian.el = (*_DsviMemHandle.image_alloc)(25*sizeof*gaussian.el);

                if(gaussian.el==NULL)
                {
                    _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
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

                    dsvi_Matrix_t* imageMtx   = dsvi_image_getMtxFromImage(srcImage,&_status);

                    if(_status==DSVI_STATUS_OK)
                    {
                        dsvi_RealMatrix_t* newMtx  = dsvi_image_RKConvolve_preserveSize(imageMtx,&gaussian,&_status);
                        if(_status == DSVI_STATUS_OK)
                        {
                            newImage   = dsvi_image_getImageFromRealMatrix(newMtx,&_status);
                        }
                        dsvi_image_deleteMatrix(imageMtx);
                    }
                    (*_DsviMemHandle.image_free)(gaussian.el);
                }
            }
        }
    }
    if(status!=NULL)
    {
        (*status) = _status;
    }
    return newImage;
}

#undef DSVI_IMAGEOP_MEDIANFILTER_MAXFRAMESIZE
#undef frameWithinImage
#undef IsFrameInsideImage
#undef isNoisy
