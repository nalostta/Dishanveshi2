#include <dishanveshi/image/operations/histogram.h>

dsvi_histogram_t* dsvi_image_histogram_new(const dsvi_histogramInfo_t* histogramInfo,dsvi_status_t* status)
{
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    dsvi_status_t _status   =   dsvi_image_getMemConfig();

    dsvi_histogram_t*   histogram   =  NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if(histogramInfo==NULL)
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            _status =   dsvi_image_histogram_checkBoundsIntegrity(histogramInfo);

            if(_status==DSVI_STATUS_OK) //integrity check passed, bounds are proper...
            {
                histogram   =   (dsvi_histogram_t*)(*_DsviMemHandle.image_alloc)(sizeof(dsvi_histogram_t));
                if(histogram==NULL)
                {
                    _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    histogram->data =   (uint32_t*)(*_DsviMemHandle.image_alloc)(histogramInfo->bins*sizeof(uint32_t));
                    if(histogram->data==NULL)
                    {
                        (*_DsviMemHandle.image_free)(histogram);
                        _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                    }else
                    {
                        histogram->bounds   =   (*_DsviMemHandle.image_alloc)((histogramInfo->bins-1)*sizeof(uint8_t));
                        if(histogram->bounds==NULL)
                        {
                            (*_DsviMemHandle.image_free)(histogram->data);
                            (*_DsviMemHandle.image_free)(histogram);
                            _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                        }else
                        {
                            histogram->bins     =   histogramInfo->bins;
                            uint8_t i;
                            for(i=0;i<(histogram->bins-1);i++)
                            {
                                histogram->bounds[i]    =   histogramInfo->bounds[i];
                                histogram->data[i]      =   0;
                            }
                            histogram->data[histogram->bins-1]=0;
                        }
                    }
                }
            }
        }
    }

    if(status!=NULL)
    {
        (*status)   =   _status;
    }

    if(_status==DSVI_STATUS_OK)
    {
        return histogram;
    }else
    {
        return NULL;
    }
}

dsvi_status_t   dsvi_image_histogram_delete(dsvi_histogram_t* histogram)
{
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    dsvi_status_t _status   =   dsvi_image_getMemConfig();

    if(_status==DSVI_STATUS_OK)
    {
        if(histogram==NULL)
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            (*_DsviMemHandle.image_free)(histogram->bounds);
            histogram->bounds   =   NULL;
            (*_DsviMemHandle.image_free)(histogram->data);
            histogram->data     =   NULL;
            (*_DsviMemHandle.image_free)(histogram);
            histogram           =   NULL;
        }
    }

    return _status;
}

dsvi_status_t dsvi_image_histogram_checkBoundsIntegrity(const dsvi_histogramInfo_t* srcInfo)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;

    if(srcInfo==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        if(srcInfo->bins<2)
        {
            _status =   DSVI_STATUS_NOT_SUPPORTED;
        }else
        {
            uint8_t index;
            uint8_t prevVal=0;

            for(index=0;index<(srcInfo->bins-1);index++)
            {
                if((srcInfo->bounds[index]==0)||(srcInfo->bounds[index]==255))
                {
                    _status =   DSVI_STATUS_ERROR;
                    break;
                }
                if(srcInfo->bounds[index]>prevVal)
                {
                    prevVal = srcInfo->bounds[index];
                }else
                {
                    _status =   DSVI_STATUS_ERROR;
                }
            }
        }
    }

    return _status;
}

dsvi_histogram_t*   dsvi_image_getHistogram(const dsvi_Image_t* srcImage,const dsvi_histogramInfo_t* hInfo,dsvi_status_t* status)
{
    dsvi_status_t _status           =   DSVI_STATUS_OK;
    dsvi_histogram_t*   histogram   =   NULL;

    if((srcImage==NULL)||(hInfo)==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        if(srcImage->data==NULL)
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            histogram   =   dsvi_image_histogram_new(hInfo,&_status);

            if(_status==DSVI_STATUS_OK)
            {
                uint8_t hIndex;
                uint16_t rowIndex=0,colIndex=0,upperLimit=0;
                uint32_t srcIndex;

                for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                {
                    for(colIndex=0;colIndex<srcImage->width;colIndex++)
                    {
                        srcIndex    =   (rowIndex*srcImage->width)+colIndex;
                        upperLimit  =   histogram->bounds[0];
                        hIndex=0;

                        while(srcImage->data[srcIndex]>=upperLimit)
                        {
                            hIndex++;
                            upperLimit  =   (hIndex==(histogram->bins-1)) ? 256:histogram->bounds[hIndex];
                        }
                        histogram->data[hIndex]++;
                    }
                }
            }
        }
    }

    if(status!=NULL)
    {
        (*status)   =   _status;
    }
    if(_status==DSVI_STATUS_OK)
    {
        return histogram;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t*   dsvi_image_histogramToImage(const dsvi_histogram_t* histogram,dsvi_status_t* status)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;
    dsvi_Image_t* newImage  =   NULL;

    if(histogram==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        uint32_t srcIndex,height=0,width=0;
        ImageInfo_t newInfo;

        for(srcIndex=0;srcIndex<histogram->bins;srcIndex++)
        {
            height  =  (height>histogram->data[srcIndex])?  height:(histogram->data[srcIndex]);
        }
        width   =   histogram->bins;

        uint16_t heightScale=1,widthScale=1;

        if(height>width)
        {
            heightScale     =   1;
            widthScale      =   height/width;
            newInfo.width   =   (widthScale*width)+4;
            newInfo.height  =   height;
        }else
        {
            heightScale     =   width/height;
            widthScale      =   1;
            newInfo.width   =   width+4;
            newInfo.height  =   heightScale*height;
        }

        newInfo.format  =   enDsviImagePixelFormat_DEFAULT;

        newImage    =   dsvi_image_new(&newInfo,&_status);


        if(_status==DSVI_STATUS_OK)
        {
            uint16_t rowIndex=0,colIndex=0;
            uint16_t  binIndex=0,binHeight=0;

            for(rowIndex=0;rowIndex<newImage->height;rowIndex++)
            {
                for(colIndex=0;colIndex<newImage->width;colIndex++)
                {
                    srcIndex    =   (rowIndex*newImage->width)+colIndex;

                    binIndex    =   (colIndex>=2)? ((colIndex-2)/widthScale):0;
                    binHeight   =   histogram->data[binIndex]*heightScale;

                    if(((colIndex>=2)&&(colIndex<=(newImage->width-2))) && ((newImage->height-rowIndex)<=binHeight))
                    {
                        newImage->data[srcIndex]    =   0x00;
                    }else
                    {
                        newImage->data[srcIndex]    =   0xff;   //white color
                    }

                }
            }
        }
    }

    if(status!=NULL)
    {
        (*status)   =   _status;
    }
    if(_status==DSVI_STATUS_OK)
    {
        return newImage;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t*   dsvi_image_histogramToImageFixedHeight(const dsvi_histogram_t* histogram,dsvi_status_t* status)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;
    dsvi_Image_t* newImage  =   NULL;

    if(histogram==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        uint32_t srcIndex,heightMax=0;
        //uint32_t width = histogram->bins;
        ImageInfo_t newInfo = {0};

        for(srcIndex=0;srcIndex<histogram->bins;srcIndex++)
        {
            heightMax  =  (heightMax>histogram->data[srcIndex])?  heightMax:(histogram->data[srcIndex]);
        }

        newInfo.format  =   enDsviImagePixelFormat_DEFAULT;
        newInfo.height  =   480;
        newInfo.width   =   histogram->bins+4;

        newImage = dsvi_image_new(&newInfo,&_status);


        if(_status==DSVI_STATUS_OK)
        {
            uint16_t rowIndex=0,colIndex=0;
            uint16_t  binIndex=0,binHeight=0;

            for(rowIndex=0;rowIndex<newImage->height;rowIndex++)
            {
                for(colIndex=0;colIndex<newImage->width;colIndex++)
                {
                    srcIndex    =   (rowIndex*newImage->width)+colIndex;

                    binIndex    =   (colIndex>=2)? ((colIndex-2)):0;
                    binHeight   =   ((histogram->data[binIndex]*512)/heightMax);

                    if(((colIndex>=2)&&(colIndex<=(newImage->width-2))) && ((newImage->height-rowIndex)<=binHeight))
                    {
                        newImage->data[srcIndex]    =   0x00;
                    }else
                    {
                        newImage->data[srcIndex]    =   0xff;   //white color
                    }

                }
            }
        }
    }

    if(status!=NULL)
    {
        (*status)   =   _status;
    }
    if(_status==DSVI_STATUS_OK)
    {
        return newImage;
    }else
    {
        return NULL;
    }
}
