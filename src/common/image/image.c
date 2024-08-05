#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/queue.h>

#define isWidthDiff(a,b)  ((a->width==b->width)?   0:1)
#define isHeightDiff(a,b) ((a->height==b->height)?   0:1)

dsvi_status_t _dsvi_Image_Is_Initialized = -6;
DsviImageMemoryConfig_t _DsviMemHandle;

///Module initialization
dsvi_status_t   dsvi_image_Init(const DsviImageMemoryConfig_t* image_allocator)
{
    if(_dsvi_Image_Is_Initialized == 0)return DSVI_STATUS_INVALID_OPERATION;
    _DsviMemHandle.image_alloc   = image_allocator->image_alloc;
    _DsviMemHandle.image_free    = image_allocator->image_free;

    if((_DsviMemHandle.image_alloc==NULL) || (_DsviMemHandle.image_free==NULL))
    return DSVI_STATUS_NOT_IMPLEMENTED;
    else
    {
        _dsvi_Image_Is_Initialized = 0;
        return DSVI_STATUS_OK;
    }
}

///Module construction and destruction
dsvi_Image_t* dsvi_image_new(const ImageInfo_t* info, dsvi_status_t* status)
{
    dsvi_status_t _status       =   dsvi_image_getMemConfig();
    dsvi_Image_t*   newImage    =   NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if(status==NULL)
        {
            _status=DSVI_STATUS_NULL_ARG;
        }else
        {
            uint8_t bitsPerPixel    = -1;

            switch(info->format)   //why not change the enum value so that image formats equal to bits per pixel
            {
                case enDsviImagePixelFormat_BINARY:
                    bitsPerPixel=1;
                    break;
                case enDsviImagePixelFormat_DEFAULT:
                    bitsPerPixel=8;
                    break;
                case enDsviImagePixelFormat_RGB565:
                    bitsPerPixel=16;
                    break;
                case enDsviImagePixelFormat_RGB888:
                    bitsPerPixel=24;
                    break;
                default:
                    bitsPerPixel=-1;
                    break;
            }
            //
            if(bitsPerPixel==-1)
            {
                _status=DSVI_STATUS_NOT_SUPPORTED;

            }else
            {
                //local variables
                size_t          imageSize   =   0;

                imageSize       =   info->width*bitsPerPixel;                               //one row length in bits
                imageSize       =   (imageSize%8==0)?   (imageSize/8):((imageSize/8)+1);    //check byte compatibility...useful bpp<8
                imageSize       =   imageSize*info->height;
                //check image size matching constraints?
                newImage        =   (dsvi_Image_t*)(*_DsviMemHandle.image_alloc)(sizeof(dsvi_Image_t));
                if(newImage==NULL)
                {
                    _status=DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    newImage->data  =   (uint8_t*)(*_DsviMemHandle.image_alloc)(imageSize*sizeof(uint8_t));

                    if(newImage->data==NULL)
                    {
                        (*_DsviMemHandle.image_free)(newImage);
                        _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                    }else
                    {
                        newImage->height    =   info->height;
                        newImage->width     =   info->width;
                        newImage->format    =   info->format;
                        newImage->roi       =   NULL;
                        //initialize to zero
                        uint32_t i;
                        for(i=0;i<imageSize;i++)newImage->data[i]=0;

                        newImage->roi = NULL;
                        newImage->label = NULL;
                    }

                }
            }
        }
    }

    (*status)=_status;
    if(_status==DSVI_STATUS_OK)
    {
        return newImage;
    }else
    {
        return NULL;
    }
}

dsvi_status_t dsvi_image_delete(dsvi_Image_t* image)
{
    if(image==NULL)return DSVI_STATUS_NULL_ARG;

    if(_dsvi_Image_Is_Initialized == -6)return DSVI_STATUS_INVALID_OPERATION;

    (*_DsviMemHandle.image_free)(image->data);
    image->data=NULL;

    if(image->label!=NULL)
    {
        (*_DsviMemHandle.image_free)(image->label);
        image->label = NULL;
    }

    if(image->roi!=NULL)
    {
        (*_DsviMemHandle.image_free)(image->roi);
        image->roi = NULL;
    }

    (*_DsviMemHandle.image_free)(image);
    return DSVI_STATUS_OK;
}

///Module properties (attributes)
dsvi_status_t dsvi_image_getInfo(const dsvi_Image_t* image, ImageInfo_t* result)
{
    if((image==NULL) || (result==NULL))return DSVI_STATUS_NULL_ARG;
    //
    result->height  =   image->height;
    result->width   =   image->width;
    result->format  =   (EnDsviImagePixelFormat_t)image->format;
    //
    return DSVI_STATUS_OK;
}

dsvi_status_t dsvi_image_getData(const dsvi_Image_t* image, uint8_t** data)
{
    if((image==NULL) || (data==NULL))return DSVI_STATUS_NULL_ARG;
    if(*data!=NULL)return DSVI_STATUS_INVALID_ARG;

    if(_dsvi_Image_Is_Initialized == -6)return DSVI_STATUS_INVALID_OPERATION;
    size_t size =image->height*image->width*sizeof(uint8_t);
    (*data)=(uint8_t*)(*_DsviMemHandle.image_alloc)(size);

    if((*data)==NULL)return DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
    return DSVI_STATUS_OK;
}

dsvi_status_t dsvi_image_getMemConfig(void)
{
    return _dsvi_Image_Is_Initialized;
}

///Module functions (verbs)
dsvi_status_t dsvi_image_writeToFile(const dsvi_Image_t* image, const char* filename)
{
    if((image==NULL) || (filename==NULL))return DSVI_STATUS_NULL_ARG;
    FILE* Handle = fopen(filename,"wb");
    if(Handle==NULL)return DSVI_STATUS_NOT_IMPLEMENTED;

    fprintf(Handle,"%u:%u:%u\n",image->height,image->width,image->format);

    uint16_t external,internal=0,index=0;
    for(external=0;external<image->height;external++)
    {
        for(internal=0;internal<image->width;internal++)
        {
            fprintf(Handle,"%c",image->data[index]);
            index++;
        }
        fprintf(Handle,"\n");
    }
    fclose(Handle);
    return DSVI_STATUS_OK;
}

dsvi_status_t dsvi_image_readFromFile(dsvi_Image_t** DestImageRef, const char* filename)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if(DestImageRef==NULL) _status = DSVI_STATUS_NULL_ARG;
    if((*DestImageRef)!=NULL) _status = DSVI_STATUS_INVALID_ARG;

    FILE* SrcImageFileHandle     = fopen(filename,"rb");
    if(SrcImageFileHandle==NULL) _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;

    if(_status!=DSVI_STATUS_OK)return _status;

    uint32_t buf_index=0,char_index=0,param_index=0;
    char buffer[DSVI_STR_BUFFER_SIZE];
    char ImageParam[3][DSVI_STR_BUFFER_SIZE];

    fgets(buffer,DSVI_STR_BUFFER_SIZE,SrcImageFileHandle);
    while((buffer[buf_index]!=0) && (buf_index<DSVI_STR_BUFFER_SIZE))
    {
        if(buffer[buf_index]==':')
        {
            ImageParam[param_index][char_index]=0;
            buf_index++;
            char_index=0;
            param_index++;
        }
        ImageParam[param_index][char_index]=buffer[buf_index];
        if(buffer[buf_index]==10)
        {
            ImageParam[param_index][char_index]=0;
            break;
        }
        buf_index++;
        char_index++;
    }

    ImageInfo_t DestImgInfo;
    DestImgInfo.height = (uint16_t)Str2Int(ImageParam[0]);
    DestImgInfo.width  = (uint16_t)Str2Int(ImageParam[1]);
    DestImgInfo.format = (EnDsviImagePixelFormat_t)Str2Int(ImageParam[2]);
    DestImgInfo.extra  = NULL;

    (*DestImageRef)    = dsvi_image_new(&DestImgInfo,&_status);

    uint32_t data_index = 0;
    buf_index   = 0;
    param_index = 0;                                     //param_index reused as index variable for iterations
    char_index  = DestImgInfo.height*DestImgInfo.width;  //char_index reused to store the total array size

    fgets(buffer,DSVI_STR_BUFFER_SIZE,SrcImageFileHandle);
    if(_status==DSVI_STATUS_OK)
    {
        while(data_index<char_index)
        {
            while(buffer[buf_index]==32)buf_index++;
            if((buffer[buf_index]==0)||(buffer[buf_index]=='\n')||(buffer[buf_index]=='\r'))
            {
                fgets(buffer,DSVI_STR_BUFFER_SIZE,SrcImageFileHandle);
                buf_index=0;
            }else
            {
                while((buffer[buf_index]!=32) && (buffer[buf_index]!=0))
                {
                    ImageParam[0][param_index++]=buffer[buf_index++];
                }
                ImageParam[0][param_index]=(int)NULL;
                (*DestImageRef)->data[data_index++] = (uint8_t)Str2Int(ImageParam[0]);
                param_index=0;
            }
        }
    }

    fclose(SrcImageFileHandle);
    return _status;
}

dsvi_status_t dsvi_image_clone(const dsvi_Image_t* image, dsvi_Image_t** result)
{
    if((image==NULL)  || (result==NULL))
    {
        return DSVI_STATUS_NULL_ARG;
    }

    if(*result!=NULL)
    {
        return DSVI_STATUS_INVALID_ARG;
    }

    if(_dsvi_Image_Is_Initialized == -6)
    {
        return DSVI_STATUS_INVALID_OPERATION;
    }

    size_t ImageSize  = image->height*image->width;
    (*result)         = (dsvi_Image_t*)(*_DsviMemHandle.image_alloc)(sizeof(dsvi_Image_t));
    (*result)->height = image->height;
    (*result)->width  = image->width;
    (*result)->format = image->format;
    (*result)->data   = (uint8_t*)(*_DsviMemHandle.image_alloc)(ImageSize);

    ImageInfo_t NewImgInfo;
    dsvi_image_getInfo((*result),&NewImgInfo);
    dsvi_status_t status=0;
    (*result)         = (dsvi_Image_t*)dsvi_image_new(&NewImgInfo,&status);

    uint32_t index;
    for(index=0;index<ImageSize;index++)(*result)->data[index]=image->data[index];

    return DSVI_STATUS_OK;
}

dsvi_status_t dsvi_image_setROI(dsvi_Image_t* image, const dsvi_ImageROI_t* roi)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;

    if((image==NULL)||(roi==NULL))
    {
        _status =    DSVI_STATUS_NULL_ARG;
    }else
    {
        if(image->roi==NULL)
        {
            if(_dsvi_Image_Is_Initialized==-6)
            {
                _status = DSVI_STATUS_INVALID_OPERATION;

            }else
            {
                image->roi  =   (dsvi_ImageROI_t*)(*_DsviMemHandle.image_alloc)(sizeof(dsvi_ImageROI_t));
                if(image->roi==NULL)
                {
                    _status =    DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    image->roi->x       =   roi->x;
                    image->roi->y       =   roi->y;
                    image->roi->width   =   roi->width;
                    image->roi->height  =   roi->height;
                }
            }
        }
    }
    return  _status;
}

dsvi_Image_t* dsvi_image_crop(const dsvi_Image_t* image,dsvi_status_t* status)
{
    dsvi_status_t   _status =   DSVI_STATUS_OK;
    dsvi_Image_t*   result  =   NULL;
    if(image==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        if(image->roi==NULL)        //try cloning here.......
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            if(image->format==enDsviImagePixelFormat_NOTSUPPORTED)
            {
                _status =   DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                uint32_t    x0=0,y0=0,x1=0,y1=0;
                uint8_t bitsPerPixel;
                switch(image->format)
                {
                case enDsviImagePixelFormat_BINARY:
                    bitsPerPixel    =   1;
                    break;
                case enDsviImagePixelFormat_RGB565:
                    bitsPerPixel    =   16;
                    break;
                case enDsviImagePixelFormat_RGB888:
                    bitsPerPixel    =   24;
                    break;
                default:
                    bitsPerPixel    =   8;
                    break;
                }
                x0  =   image->roi->x;
                y0  =   image->roi->y;
                y1  =   image->roi->width+y0;
                x1  =   image->roi->height+x0;
                //adjustments to above variables done in the next step

                if((image->height<x1)||(image->width<y1))
                {
                    _status =   DSVI_STATUS_INVALID_ARG;
                }else
                {
                    y0 *=   bitsPerPixel/8;
                    y1 *=   bitsPerPixel;
                    if(bitsPerPixel<8)
                    {
                        y1 = ( (y1%8==0)? (y1/8):((y1/8)+1) );
                    }else
                    {
                        y1/=8;
                    }

                    ImageInfo_t newInfo;
                    newInfo.format  =   image->format;
                    newInfo.height  =   image->roi->height;
                    newInfo.width   =   image->roi->width;
                    result          =   dsvi_image_new(&newInfo,&_status);
                    if(_status==DSVI_STATUS_OK)
                    {
                        uint32_t rowIndex=0,colIndex=0,dstIndex=0,srcIndex=0;
                        uint32_t jumpLen = image->width*bitsPerPixel;

                        jumpLen = (jumpLen%8==0)? (jumpLen/8):((jumpLen/8)+1);
                        for(rowIndex=x0;rowIndex<x1;rowIndex++)
                        {
                            srcIndex = (rowIndex*jumpLen)+(y0);
                            for(colIndex=y0;colIndex<y1;colIndex++)
                            {
                                result->data[dstIndex++]=image->data[srcIndex++];
                            }
                        }
                    }
                }
            }
        }
    }
    (*status)   =  _status;

    if(_status==DSVI_STATUS_OK)
    {
        return result;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t* dsvi_image_cropEx(const dsvi_Image_t* image, const dsvi_ImageROI_t* roi, dsvi_status_t* status)
{
    dsvi_status_t   _status =   DSVI_STATUS_OK;
    dsvi_Image_t*   result  =   NULL;

    if((image==NULL)||(roi==NULL))
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        if(image->format==enDsviImagePixelFormat_NOTSUPPORTED)
        {
            _status =   DSVI_STATUS_NOT_SUPPORTED;
        }else
        {
            uint32_t    x0=0,y0=0,x1=0,y1=0;
            uint8_t bitsPerPixel;
            switch(image->format)
            {
            case enDsviImagePixelFormat_BINARY:
                bitsPerPixel    =   1;
                break;
            case enDsviImagePixelFormat_RGB565:
                bitsPerPixel    =   16;
                break;
            case enDsviImagePixelFormat_RGB888:
                bitsPerPixel    =   24;
                break;
            default:
                bitsPerPixel    =   8;
                break;
            }
            x0  =   roi->x;
            y0  =   roi->y;
            y1  =   roi->width+y0;
            x1  =   roi->height+x0;
            //adjustments to above variables done in the next step

            if((image->height<x1)||(image->width<y1))
            {
                _status =   DSVI_STATUS_INVALID_OPERATION;
            }else
            {
                y0 *=   bitsPerPixel/8;
                y1 *=   bitsPerPixel;
                if(bitsPerPixel<8)
                {
                    y1 = ( (y1%8==0)? (y1/8):((y1/8)+1) );
                }else
                {
                    y1/=8;
                }

                ImageInfo_t newInfo;
                newInfo.format  =   image->format;
                newInfo.height  =   roi->height;
                newInfo.width   =   roi->width;
                result          =   dsvi_image_new(&newInfo,&_status);
                if(_status==DSVI_STATUS_OK)
                {
                    uint32_t rowIndex=0,colIndex=0,dstIndex=0,srcIndex=0;
                    uint32_t jumpLen = image->width*bitsPerPixel;

                    jumpLen = (jumpLen%8==0)? (jumpLen/8):((jumpLen/8)+1);
                    for(rowIndex=x0;rowIndex<x1;rowIndex++)
                    {
                        srcIndex = (rowIndex*jumpLen)+(y0);
                        for(colIndex=y0;colIndex<y1;colIndex++)
                        {
                            result->data[dstIndex++]=image->data[srcIndex++];
                        }
                    }
                }
            }
        }
    }
    (*status)   =  _status;

    if(_status==DSVI_STATUS_OK)
    {
        return result;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t* dsvi_image_colorToGrey(const dsvi_Image_t* srcImage,dsvi_status_t* status) //weights predefined
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;
    dsvi_Image_t* newImage  =   NULL;

    if(srcImage==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        uint8_t bitsPerPixel;
        switch(srcImage->format)
        {
        case enDsviImagePixelFormat_RGB888:
            bitsPerPixel    =   24;
            break;
        case enDsviImagePixelFormat_RGB565:
            bitsPerPixel    =   16;
            break;
        default:
            bitsPerPixel    =   0;
        }

        if(bitsPerPixel==0)
        {
            _status =   DSVI_STATUS_NOT_SUPPORTED;
        }else
        {
            if(dsvi_image_getMemConfig()!=DSVI_STATUS_OK)
            {
                _status =   DSVI_STATUS_NOT_IMPLEMENTED;
            }else
            {
                ImageInfo_t info;
                info.format =   srcImage->format;
                info.width  =   srcImage->width;
                info.height =   srcImage->height;
                newImage    =   dsvi_image_new(&info,&_status);

                if(newImage==NULL)                {
                    _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    real_t grayVal=0;
                    uint8_t intGrayVal=0;
                    uint16_t rowIndex=0,colIndex=0;
                    uint32_t srcIndex=0;

                    newImage->width     = srcImage->width;
                    newImage->height    = srcImage->height;
                    newImage->format    = srcImage->format;

                    for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                    {
                        for(colIndex=0;colIndex<srcImage->width;colIndex++)
                        {
                            srcIndex    =   (rowIndex*srcImage->width+colIndex)*bitsPerPixel/8;
                            grayVal     = dsvi_Image_getGrey(&srcImage->data[srcIndex],bitsPerPixel,0.299,0.587,0.11,&_status);
                            if(srcImage->format==enDsviImagePixelFormat_RGB888)
                            {
                                newImage->data[srcIndex]    =   (uint8_t)grayVal;
                                newImage->data[srcIndex+1]  =   (uint8_t)grayVal;
                                newImage->data[srcIndex+2]  =   (uint8_t)grayVal;
                            }else //prev checks ensure only RGB888 and RGB 565 are allowed to process
                            {
                                intGrayVal            =   (uint8_t)grayVal;
                                newImage->data[srcIndex]    =   ((intGrayVal&0x03)|(intGrayVal>>5));  //add entries in image/defs file for image formats specifics?
                                newImage->data[srcIndex+1]  =   ((intGrayVal<<5)|(intGrayVal>>3));
                            }
                        }
                    }
                    //new grayscale image generated
                }
            }
        }
    }

    if(status!=NULL)
    {
        (*status)=_status;
    }
    if(_status==DSVI_STATUS_OK)
    {
        return newImage;
    }else
    {
        return NULL;
    }
}



dsvi_Image_t* dsvi_image_colorToGreyEx(const dsvi_Image_t* srcImage,real_t weightR,real_t weightG,real_t weightB,dsvi_status_t* status)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;
    dsvi_Image_t* newImage  =   NULL;

    if(srcImage==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        uint8_t bitsPerPixel;
        switch(srcImage->format)
        {
        case enDsviImagePixelFormat_RGB888:
            bitsPerPixel    =   24;
            break;
        case enDsviImagePixelFormat_HSV:
            bitsPerPixel    =   24;
            break;
        case enDsviImagePixelFormat_RGB565:
            bitsPerPixel    =   16;
            break;
        default:
            bitsPerPixel    =   0;
        }

        if(bitsPerPixel==0)
        {
            _status =   DSVI_STATUS_NOT_SUPPORTED;
        }else
        {
            if(dsvi_image_getMemConfig()!=DSVI_STATUS_OK)
            {
                _status =   DSVI_STATUS_NOT_IMPLEMENTED;
            }else
            {
                ImageInfo_t info;
                info.format =   srcImage->format;
                info.width  =   srcImage->width;
                info.height =   srcImage->height;
                newImage    =   dsvi_image_new(&info,&_status);

                if(newImage==NULL)                {
                    _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    real_t grayVal=0;
                    uint8_t intGrayVal=0;
                    uint16_t rowIndex=0,colIndex=0;
                    uint32_t srcIndex=0;

                    newImage->width     = srcImage->width;
                    newImage->height    = srcImage->height;
                    newImage->format    = srcImage->format;

                    for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                    {
                        for(colIndex=0;colIndex<srcImage->width;colIndex++)
                        {
                            srcIndex    =   (rowIndex*srcImage->width+colIndex)*bitsPerPixel/8;
                            grayVal     = dsvi_Image_getGrey(&srcImage->data[srcIndex],bitsPerPixel,weightR,weightG,weightB,&_status);
                            if(srcImage->format==enDsviImagePixelFormat_RGB888)
                            {
                                newImage->data[srcIndex]    =   (uint8_t)grayVal;
                                newImage->data[srcIndex+1]  =   (uint8_t)grayVal;
                                newImage->data[srcIndex+2]  =   (uint8_t)grayVal;
                            }else //prev checks ensure only RGB888 and RGB 565 are allowed to process
                            {
                                intGrayVal            =   (uint8_t)grayVal;
                                newImage->data[srcIndex]    =   ((intGrayVal&0x03)|(intGrayVal>>5));  //add entries in image/defs file for image formats specifics?
                                newImage->data[srcIndex+1]  =   ((intGrayVal<<5)|(intGrayVal>>3));
                            }
                        }
                    }
                    //new grayscale image generated
                }
            }
        }
    }

    if(status!=NULL)
    {
        (*status)=_status;
    }
    if(_status==DSVI_STATUS_OK)
    {
        return newImage;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t* dsvi_image_convertToDefault(const dsvi_Image_t* srcImage,dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage    =   NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if(srcImage==NULL)
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format!=enDsviImagePixelFormat_RGB888) //supports only RGB888 to default for now!!!
            {
                _status =   DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                uint8_t bitPerPixel =   -1;
                switch(srcImage->format)
                {
                case enDsviImagePixelFormat_RGB888:
                    bitPerPixel =   24;
                    break;
                }

                if(bitPerPixel==-1)
                {
                    _status =   DSVI_STATUS_NOT_SUPPORTED;
                }else
                {
                    ImageInfo_t newInfo;
                    _status =   dsvi_image_getInfo(srcImage,&newInfo);
                    if(_status==DSVI_STATUS_OK)
                    {
                        newInfo.format  =   enDsviImagePixelFormat_DEFAULT;
                        newImage        =   dsvi_image_new(&newInfo,&_status);

                        if(_status==DSVI_STATUS_OK)
                        {
                            uint16_t rowIndex,colIndex;
                            uint32_t srcIndex=0,dstIndex=0;

                            for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                            {
                                dstIndex    =   rowIndex*newImage->width;
                                for(colIndex=0;colIndex<srcImage->width;colIndex++)
                                {
                                    srcIndex    =   (rowIndex*srcImage->width)+colIndex;
                                    srcIndex   *=   bitPerPixel/8;
                                    newImage->data[dstIndex++]  =   (uint8_t)dsvi_Image_getGrey(&srcImage->data[srcIndex],24,0.33,0.34,0.33,&_status);
                                    //newImage->data[dstIndex++]    =   ((srcImage->data[srcIndex]&&0xe0)||((srcImage->data[srcIndex+1]&(0xe0))>>3)||(((srcImage->data[srcIndex+2])&0xc0)>>6));
                                }
                            }
                        }else
                        {
                            _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                            dsvi_image_delete(newImage);
                        }
                    }
                }
            }
        }
    }
    (*status)=_status;
    if(_status==DSVI_STATUS_OK)
    {
        return newImage;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t* dsvi_image_convertTo888(const dsvi_Image_t* srcImage,dsvi_status_t* status)
{
    dsvi_status_t _status   =   dsvi_image_getMemConfig();
    dsvi_Image_t* newImage  =   NULL;

    if(_status!=DSVI_STATUS_OK)
    {
        _status =   DSVI_STATUS_NOT_IMPLEMENTED;
    }else
    {
        if(srcImage==NULL)
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            uint8_t bitsPerPixel=-1;

            switch(srcImage->format)
            {
            case enDsviImagePixelFormat_DEFAULT:
                bitsPerPixel=8;
                break;
            }

            if((bitsPerPixel==-1)||(srcImage->format==enDsviImagePixelFormat_RGB888))
            {
                _status =   DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                ImageInfo_t newInfo = {0};
                _status =   dsvi_image_getInfo(srcImage,&newInfo);
                if(_status==DSVI_STATUS_OK)
                {
                    newInfo.format =   enDsviImagePixelFormat_RGB888;
                    newImage    =   dsvi_image_new(&newInfo,&_status);

                    if(_status==DSVI_STATUS_OK)
                    {
                        uint32_t srcIndex=0,dstIndex=0;
                        uint16_t rowIndex,colIndex=0;

                        for(rowIndex=0;rowIndex<newImage->height;rowIndex++)
                        {
                            for(colIndex=0;colIndex<newImage->width;colIndex++)
                            {
                                srcIndex    =   ((rowIndex*newImage->width)+colIndex);
                                dstIndex    =   srcIndex*3;
                                newImage->data[dstIndex]      =   srcImage->data[srcIndex];
                                newImage->data[dstIndex+1]    =   srcImage->data[srcIndex];
                                newImage->data[dstIndex+2]    =   srcImage->data[srcIndex];
                            }
                        }
                    }else
                    {
                        dsvi_image_delete(newImage);
                    }
                }
            }
        }
    }

    (*status)   =   _status;
    if(_status==DSVI_STATUS_OK)
    {
        return newImage;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t* dsvi_image_getChannel(const dsvi_Image_t* srcImage,uint8_t ch,dsvi_status_t* status)
{
    dsvi_status_t _status  = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;

    if(_status == DSVI_STATUS_OK)
    {
        if(srcImage==NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if((srcImage->format!=enDsviImagePixelFormat_RGB888)&&(srcImage->format!=enDsviImagePixelFormat_HSV))
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                if(ch>2)
                {
                    _status = DSVI_STATUS_INVALID_ARG;
                }else
                {
                    ImageInfo_t info = {0};
                    info.width = srcImage->width;
                    info.height = srcImage->height;
                    info.format = enDsviImagePixelFormat_DEFAULT;

                    newImage = dsvi_image_new(&info,&_status);

                    if(_status == DSVI_STATUS_OK)
                    {
                        uint8_t offset;
                        uint16_t row_i,col_i;
                        uint32_t src_i,dest_i;

                        switch(ch)
                        {
                        case 0 :
                            offset = 2;
                            break;
                        case 1 :
                            offset = 1;
                            break;
                        case 2 :
                            offset = 0;
                            break;
                        default :
                            offset = 1;
                            break;
                        }

                        for(row_i=0;row_i<newImage->height;row_i++)
                        {
                            dest_i = (row_i*newImage->width);
                            src_i = (dest_i*3)+offset;
                            for(col_i=0;col_i<newImage->width;col_i++)
                            {
                                newImage->data[dest_i] = srcImage->data[src_i];
                                dest_i++;
                                src_i+=3;
                            }
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

dsvi_Image_t* dsvi_image_replaceChannel(const dsvi_Image_t* srcImage,const dsvi_Image_t* srcChannel,uint8_t chIndex,dsvi_status_t* status)
{
    dsvi_status_t _status  = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if((srcImage==NULL)||(srcChannel==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if((srcImage->format!=enDsviImagePixelFormat_HSV)&&(srcImage->format!=enDsviImagePixelFormat_RGB888))
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                if((srcChannel->format!=enDsviImagePixelFormat_DEFAULT)||(chIndex>2))
                {
                    _status = DSVI_STATUS_INVALID_ARG;
                }else
                {
                    if((srcImage->width!=srcChannel->width)||(srcImage->height!=srcChannel->height))
                    {
                        _status = DSVI_STATUS_INVALID_OPERATION;
                    }else
                    {
                        ImageInfo_t info = {0};
                        info.width  = srcImage->width;
                        info.height = srcImage->height;
                        info.format = srcImage->format;

                        newImage = dsvi_image_new(&info,&_status);

                        if(_status == DSVI_STATUS_OK)
                        {
                            uint16_t row_i,col_i;
                            uint32_t srcCh_i,pix_i;

                            for(row_i=0;row_i<newImage->height;row_i++)
                            {
                                for(col_i=0;col_i<newImage->width;col_i++)
                                {
                                    srcCh_i = ((row_i*srcImage->width)+col_i);
                                    pix_i = srcCh_i*3;;
                                    switch(chIndex)
                                    {
                                    case 0:
                                        newImage->data[pix_i+0] = srcChannel->data[srcCh_i];
                                        newImage->data[pix_i+1] = srcImage->data[pix_i+1];
                                        newImage->data[pix_i+2] = srcImage->data[pix_i+2];
                                        break;
                                    case 1:
                                        newImage->data[pix_i+0] = srcImage->data[pix_i+0];
                                        newImage->data[pix_i+1] = srcChannel->data[srcCh_i];
                                        newImage->data[pix_i+2] = srcImage->data[pix_i+2];
                                        break;
                                    case 2:
                                        newImage->data[pix_i+0] = srcImage->data[pix_i+0];
                                        newImage->data[pix_i+1] = srcImage->data[pix_i+1];
                                        newImage->data[pix_i+2] = srcChannel->data[srcCh_i];
                                        break;
                                    default :
                                        newImage->data[pix_i+0] = 0;
                                        newImage->data[pix_i+1] = 0;
                                        newImage->data[pix_i+2] = 0;
                                        break;
                                    }
                                }
                            }
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

dsvi_Image_t* dsvi_image_mergeUsingOR(const dsvi_Image_t* srcImage1,const dsvi_Image_t* srcImage2,dsvi_status_t* status)
{
    dsvi_status_t _status  = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if((srcImage1==NULL)||(srcImage2==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if((srcImage1->format!=enDsviImagePixelFormat_DEFAULT)&&(srcImage2->format!=enDsviImagePixelFormat_DEFAULT))
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                if((srcImage1->width!=srcImage2->width)||(srcImage1->height!=srcImage2->height))
                {
                    _status = DSVI_STATUS_INVALID_OPERATION;
                }else
                {
                    ImageInfo_t info = {0};
                    info.width  = srcImage1->width;
                    info.height = srcImage1->height;
                    info.format = enDsviImagePixelFormat_DEFAULT;

                    newImage = dsvi_image_new(&info,&_status);

                    if(_status == DSVI_STATUS_OK)
                    {
                        uint16_t row_i,col_i;
                        uint32_t pix_i;

                        for(row_i=0;row_i<newImage->height;row_i++)
                        {
                            for(col_i=0;col_i<newImage->width;col_i++)
                            {
                                pix_i = ((row_i*newImage->width)+col_i);
                                newImage->data[pix_i] = srcImage1->data[pix_i] | srcImage2->data[pix_i];
                            }
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

dsvi_Image_t* dsvi_image_mergeUsingAND(const dsvi_Image_t* srcImage1,const dsvi_Image_t* srcImage2,dsvi_status_t* status)
{
    dsvi_status_t _status  = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;

    if(_status==DSVI_STATUS_OK)
    {
        if((srcImage1==NULL)||(srcImage2==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if((srcImage1->format!=enDsviImagePixelFormat_DEFAULT)&&(srcImage2->format!=enDsviImagePixelFormat_DEFAULT))
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                if((srcImage1->width!=srcImage2->width)||(srcImage1->height!=srcImage2->height))
                {
                    _status = DSVI_STATUS_INVALID_OPERATION;
                }else
                {
                    ImageInfo_t info = {0};
                    info.width  = srcImage1->width;
                    info.height = srcImage1->height;
                    info.format = enDsviImagePixelFormat_DEFAULT;

                    newImage = dsvi_image_new(&info,&_status);

                    if(_status == DSVI_STATUS_OK)
                    {
                        uint16_t row_i,col_i;
                        uint32_t pix_i;

                        for(row_i=0;row_i<newImage->height;row_i++)
                        {
                            for(col_i=0;col_i<newImage->width;col_i++)
                            {
                                pix_i = ((row_i*newImage->width)+col_i);
                                newImage->data[pix_i] = srcImage1->data[pix_i] & srcImage2->data[pix_i];
                            }
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

dsvi_Image_t* dsvi_image_combine3chTo888(const dsvi_Image_t* srcChR,const dsvi_Image_t* srcChG,const dsvi_Image_t* srcChB,dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;
    if(_status==DSVI_STATUS_OK)
    {
        if((srcChR==NULL)||(srcChB==NULL)||(srcChG==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if((srcChR->format!=enDsviImagePixelFormat_DEFAULT)||(srcChG->format!=enDsviImagePixelFormat_DEFAULT)||(srcChB->format!=enDsviImagePixelFormat_DEFAULT))
            {
                _status = DSVI_STATUS_INVALID_ARG;
            }else
            {
                int error = (isWidthDiff(srcChR,srcChG)||isWidthDiff(srcChR,srcChB)||isWidthDiff(srcChB,srcChR)||isHeightDiff(srcChR,srcChG)||isHeightDiff(srcChG,srcChB)||isHeightDiff(srcChB,srcChR));
                if(error)
                {
                    _status = DSVI_STATUS_INVALID_OPERATION;
                }else
                {
                    ImageInfo_t newInfo = {0};
                    newInfo.format = enDsviImagePixelFormat_RGB888;
                    newInfo.width  = srcChR->width;
                    newInfo.height  = srcChR->height;

                    newImage = dsvi_image_new(&newInfo,&_status);

                    if(_status==DSVI_STATUS_OK)
                    {
                        uint16_t row_i,col_i;
                        uint32_t src_i,dest_i;

                        for(row_i=0;row_i<srcChR->height;row_i++)
                        {
                            src_i = (row_i*srcChR->width);
                            for(col_i=0;col_i<srcChR->width;col_i++)
                            {
                                dest_i = src_i*3;

                                newImage->data[dest_i+0] = srcChR->data[src_i];
                                newImage->data[dest_i+1] = srcChG->data[src_i];
                                newImage->data[dest_i+2] = srcChB->data[src_i];

                                src_i++;
                            }
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

#undef isWidthDiff
#undef isHeightDiff
