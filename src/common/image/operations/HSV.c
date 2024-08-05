#include <dishanveshi/image/operations/HSV.h>
#include <math.h>

#define HSV_BYTES_PER_PIXEL 3

#define MaxCH(r,g,b) ((r>g)? ((r>b)? r:b):((g>b)?  g:b))
#define MinCH(r,g,b) ((r<g)? ((r<b)? r:b):((g<b)?  g:b))
#define abs(a) ((a>=0)? a:-a)

#define _8bitRange2Degree 1.411765
#define _8bitMax_f 255.0
#define _degreeRangeTo8Bit 0.708334
#define _percentTo8bit 2.55
#define _8bit2Percent 0.392157


dsvi_status_t dsvi_image_colorFormat_getRGB(const dsvi_colorFormatHSV* inHSV,dsvi_colorFormatRGB* outRGB)
{
    dsvi_status_t _status = DSVI_STATUS_OK;

    if((inHSV==NULL)||(outRGB==NULL))
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        real_t H = inHSV->h*_8bitRange2Degree;
        real_t S = inHSV->s/_8bitMax_f;
        real_t V = inHSV->v/_8bitMax_f;
        //
        real_t C = V*S;
        real_t X = H/60;
        uint8_t band = X;
        X = 1-abs((X-1-(band-(band%2))));
        X*= C;
        real_t m = V-C;
        X += m;
        C += m;

        //printf("\ninternal debug2 : m=%f\tX=%f\tC=%f\n\t\t  m'=%f\tX'=%f\tC'=%f\n",m,X,C,m*255,X*255,C*255);

        switch(band)
        {
        case 0:
            outRGB->r = C*_8bitMax_f;
            outRGB->g = X*_8bitMax_f;
            outRGB->b = m*_8bitMax_f;
            break;
        case 1:
            outRGB->r = X*_8bitMax_f;
            outRGB->g = C*_8bitMax_f;
            outRGB->b = m*_8bitMax_f;
            break;
        case 2:
            outRGB->r = m*_8bitMax_f;
            outRGB->g = C*_8bitMax_f;
            outRGB->b = X*_8bitMax_f;
            break;
        case 3: //
            outRGB->r = m*_8bitMax_f;
            outRGB->g = X*_8bitMax_f;
            outRGB->b = C*_8bitMax_f;
            break;
        case 4:
            outRGB->r = X*_8bitMax_f;
            outRGB->g = m*_8bitMax_f;
            outRGB->b = C*_8bitMax_f;
            break;
        case 5:
            outRGB->r = C*_8bitMax_f;
            outRGB->g = m*_8bitMax_f;
            outRGB->b = X*_8bitMax_f;
            break;
        default :
            outRGB->r=0;
            outRGB->g=0;
            outRGB->b=0;
            _status = DSVI_STATUS_ERROR;
            break;
        }
    }
    return _status;
}

dsvi_status_t   dsvi_image_colorFormat_getHSV(const dsvi_colorFormatRGB* inRGB,dsvi_colorFormatHSV* outHSV)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;

    if((inRGB==NULL)||(outHSV==NULL))
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        uint8_t rgbMax  = MaxCH(inRGB->r,inRGB->g,inRGB->b);
        uint8_t rgbMin  = MinCH(inRGB->r,inRGB->g,inRGB->b);
        uint8_t delta   = rgbMax-rgbMin;

        real_t H=0.0;
        real_t S=0.0;

        if(delta==0)
        {
            outHSV->h = 0;
            outHSV->s = 0;
        }else
        {
            if(inRGB->r==rgbMax)
            {
                H = ((real_t)(inRGB->g-inRGB->b)/delta);
                uint8_t temp = H/6.0;
                H = (H-(temp*6.0))*60;
            }else
            if(inRGB->g==rgbMax)
            {
                H = ((real_t)(inRGB->b-inRGB->r)/delta);
                H = (H+2)*60;
            }else //blue = max
            {
                H = ((real_t)(inRGB->r-inRGB->g)/delta);
                H = (H+4)*60;
            }
            S = delta*100.0/rgbMax;
            while(H>360||H<0)
            {
                if(H<0)
                {
                    H+=360;
                }else
                if(H>360)
                {
                    H-=360;
                }
            }
            //printf("\ninternal debug delta=%d\tmax=%d\tH=%f\tS=%f",delta,rgbMax,H,S);
            outHSV->h = H*_degreeRangeTo8Bit;
            outHSV->s = S*_percentTo8bit;
        }
        //real_t V = rgbMax*_8bit2Percent;
        //printf("\tVal=%f\n",V);
        outHSV->v = rgbMax;
    }
    return _status;
}

dsvi_Image_t*   dsvi_image_getHSV(const dsvi_Image_t* srcImage,dsvi_status_t* status)
{
    dsvi_status_t   _status     =   dsvi_image_getMemConfig();
    dsvi_Image_t*   newImage    =   NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if(srcImage==NULL)
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format!=enDsviImagePixelFormat_RGB888)
            {
                _status =  DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                ImageInfo_t newInfo;
                newInfo.width  =   srcImage->width;
                newInfo.height =   srcImage->height;
                newInfo.format =   enDsviImagePixelFormat_HSV;

                newImage        =   dsvi_image_new(&newInfo,&_status);

                if(_status==DSVI_STATUS_OK)
                {
                    uint16_t    rowIndex=0,colIndex=0;
                    uint32_t    dataIndex=0;

                    dsvi_colorFormatRGB rgb = {0};
                    dsvi_colorFormatHSV hsv = {0};

                    for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                    {
                        for(colIndex=0;colIndex<srcImage->width;colIndex++)
                        {
                            dataIndex   =   ((rowIndex*srcImage->width)+colIndex)*3;
                            rgb.r       =   srcImage->data[dataIndex+2];
                            rgb.g       =   srcImage->data[dataIndex+1];
                            rgb.b       =   srcImage->data[dataIndex+0];

                            dsvi_image_colorFormat_getHSV(&rgb,&hsv);

                            newImage->data[dataIndex+0] =   hsv.h;
                            newImage->data[dataIndex+1] =   hsv.s;
                            newImage->data[dataIndex+2] =   hsv.v;
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

dsvi_Image_t* dsvi_image_getRGB(const dsvi_Image_t* srcImage,dsvi_status_t* status)
{
    dsvi_status_t   _status     =   dsvi_image_getMemConfig();
    dsvi_Image_t*   newImage    =   NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if(srcImage==NULL)
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format!=enDsviImagePixelFormat_HSV)
            {
                _status =  DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                ImageInfo_t newInfo;
                newInfo.width  =   srcImage->width;
                newInfo.height =   srcImage->height;
                newInfo.format =   enDsviImagePixelFormat_RGB888;

                newImage       =   dsvi_image_new(&newInfo,&_status);

                if(_status==DSVI_STATUS_OK)
                {
                    uint16_t    rowIndex=0,colIndex=0;
                    uint32_t    dataIndex=0;

                    dsvi_colorFormatRGB rgb;
                    dsvi_colorFormatHSV hsv;

                    for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                    {
                        for(colIndex=0;colIndex<srcImage->width;colIndex++)
                        {
                            dataIndex   =   ((rowIndex*srcImage->width)+colIndex)*3;
                            hsv.h       =   srcImage->data[dataIndex+0];
                            hsv.s       =   srcImage->data[dataIndex+1];
                            hsv.v       =   srcImage->data[dataIndex+2];

                            dsvi_image_colorFormat_getRGB(&hsv,&rgb);

                            newImage->data[dataIndex+2] =   rgb.r;
                            newImage->data[dataIndex+1] =   rgb.g;
                            newImage->data[dataIndex+0] =   rgb.b;
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

dsvi_Image_t*   dsvi_image_suppressBGUsingHueBand(const dsvi_Image_t* srcImage,uint8_t hueMin,uint8_t hueMax,dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;

    if(_status == DSVI_STATUS_OK)
    {
        if((srcImage==NULL)||(status==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format!=enDsviImagePixelFormat_HSV)
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                ImageInfo_t newInfo = {0};
                newInfo.width  = srcImage->width;
                newInfo.height = srcImage->height;
                newInfo.format = srcImage->format;

                newImage = dsvi_image_new(&newInfo,&_status);

                if(_status == DSVI_STATUS_OK)
                {
                    uint16_t row_i=0,col_i=0;
                    uint32_t el_i;

                    for(row_i=0;row_i<srcImage->height;row_i++)
                    {
                        for(col_i=0;col_i<srcImage->width;col_i++)
                        {
                            el_i = ((srcImage->width*row_i)+col_i)*HSV_BYTES_PER_PIXEL;
                            if((srcImage->data[el_i]>=hueMin)&&(srcImage->data[el_i]<=hueMax))
                            {
                                newImage->data[el_i+0]  =   srcImage->data[el_i+0];
                                newImage->data[el_i+1]  =   srcImage->data[el_i+1];
                                newImage->data[el_i+2]  =   srcImage->data[el_i+2];
                            }else
                            {
                                newImage->data[el_i+0]  =   srcImage->data[el_i+0];
                                newImage->data[el_i+1]  =   srcImage->data[el_i+1];
                                newImage->data[el_i+2]  =   0;
                            }
                        }
                    }
                }
            }
        }
    }

    (*status) = _status;
    return newImage;
}

#undef root2
#undef root3
#undef _8bitRange2Degree
#undef abs
