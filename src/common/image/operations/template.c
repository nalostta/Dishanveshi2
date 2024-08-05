#include <dishanveshi/image/operations/template.h>

dsvi_Image_t* dsvi_image_getTemplate(const dsvi_Image_t* srcImage,const dsvi_template_t* srcTemplate,uint8_t bgVal,dsvi_status_t* status)
{
    dsvi_status_t _status   = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage  =   NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if((srcTemplate==NULL)||(srcImage==NULL))
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcTemplate->templateShape!=enTrapezium)
            {
                _status =   DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                uint16_t base   =   srcTemplate->parameters[0]; //base
                uint16_t height =   srcTemplate->parameters[1]; //height

                if((height<1)||(base<((2*height)-1)))
                {
                    _status =    DSVI_STATUS_INVALID_OPERATION;
                }else
                {
                    if(((base+srcTemplate->x)>srcImage->width)||((srcTemplate->y-height)>srcImage->height))
                    {
                        _status =   DSVI_STATUS_INVALID_OPERATION;
                    }else
                    {
                        ImageInfo_t newInfo;
                        newInfo.format  =   srcImage->format;
                        newInfo.height  =   height;
                        newInfo.width   =   base;

                        newImage        =   dsvi_image_new(&newInfo,&_status);

                        if(_status==DSVI_STATUS_OK)
                        {
                            uint16_t lowerLim   =   srcTemplate->x+height-1;
                            uint16_t upperLim   =   srcTemplate->x+base-height;

                            uint16_t rowIndex=0,colIndex=0;
                            uint32_t srcIndex=0,destIndex=0;

                            for(rowIndex=(srcTemplate->y-height);rowIndex<srcTemplate->y;rowIndex++)
                            {
                                for(colIndex=srcTemplate->x;colIndex<(srcTemplate->x+base);colIndex++)
                                {
                                    srcIndex    =   (rowIndex*srcImage->width)+colIndex;
                                    if((colIndex>=lowerLim)&&(colIndex<=upperLim))
                                    {
                                        newImage->data[destIndex++]   =   srcImage->data[srcIndex];
                                    }else
                                    {
                                        newImage->data[destIndex++]   =   bgVal;
                                    }
                                }
                                upperLim++;
                                lowerLim--;
                            }
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
        return newImage;
    }else
    {
        return NULL;
    }
}
