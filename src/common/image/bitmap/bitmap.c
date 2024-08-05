#include "bitmap.h"

BitmapHeader_t* dsvi_image_bitmap_getheader(const char* buf_header,dsvi_status_t* status)
{
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    dsvi_status_t _status   =  dsvi_image_getMemConfig();
    BitmapHeader_t* pHeader = NULL;

    if(_status==DSVI_STATUS_OK)
    {
        pHeader = (BitmapHeader_t*)(*_DsviMemHandle.image_alloc)(sizeof(BitmapHeader_t));

        if(buf_header==NULL)
        {
            _status=DSVI_STATUS_NULL_ARG;
        }else
        {
            if((buf_header[0]=='B')&&(buf_header[1]=='M')) //checks signature
            {
                //will need read from buffer in LE here...
                pHeader->filesize     = dsvi_bufferRead_LE(&buf_header[DSVI_BITMAP_OFFSET_FHDR_FILESIZE],sizeof(pHeader->filesize));
                pHeader->dataoffset   = dsvi_bufferRead_LE(&buf_header[DSVI_BITMAP_OFFSET_FHDR_DATAOFFSET],sizeof(pHeader->dataoffset));

                pHeader->width        = dsvi_bufferRead_LE(&buf_header[DSVI_BITMAP_OFFSET_IHDR_WIDTH],sizeof(pHeader->width));
                pHeader->height       = dsvi_bufferRead_LE(&buf_header[DSVI_BITMAP_OFFSET_IHDR_HEIGHT],sizeof(pHeader->height));
                pHeader->bitsPerPixel = dsvi_bufferRead_LE(&buf_header[DSVI_BITMAP_OFFSET_IHDR_BITSPERPIXEL],sizeof(pHeader->bitsPerPixel));
                pHeader->imageSize    = dsvi_bufferRead_LE(&buf_header[DSVI_BITMAP_OFFSET_IHDR_IMAGESIZE],sizeof(pHeader->imageSize));

            }else
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }
        }
    }

    (*status)=_status;
    if(_status==DSVI_STATUS_OK)
    {
        return pHeader;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t* dsvi_image_bitmap_read(const char* filename,dsvi_status_t* status) //accepts only RGB888 format!
{
    dsvi_status_t   _status     =   dsvi_image_getMemConfig();
    dsvi_Image_t*   newImage    =   NULL;
    FILE* BitmapHandle          =   fopen(filename,"rb");

    extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(_status!=DSVI_STATUS_OK)
    {
        _status =   DSVI_STATUS_NOT_IMPLEMENTED;
    }else
    {
        if(BitmapHandle==NULL)
        {
            _status = DSVI_STATUS_MEMORY_READ_FAILED;
        }else
        {
            //local variables
            char            buf_header[BITMAP_COMBINED_HEADER_SIZE];
            BitmapHeader_t* header=NULL;

            fread(buf_header,BITMAP_COMBINED_HEADER_SIZE,1,BitmapHandle);

            //validate header and return bitmap info header
            header = dsvi_image_bitmap_getheader(buf_header,&_status);

            if(_status!=DSVI_STATUS_OK)
            {
                (*status)=_status;
                return NULL;
            }else
            {
                //local variables
                ImageInfo_t     imageInfo;

                switch(header->bitsPerPixel)   //why not change the enum value so that image formats equal to bits per pixel
                {
                    case 1:
                        imageInfo.format=enDsviImagePixelFormat_BINARY;
                        break;
                    case 16:
                        imageInfo.format=enDsviImagePixelFormat_RGB565;
                        break;
                    case 24:
                        imageInfo.format=enDsviImagePixelFormat_RGB888;
                        break;
                    default:
                        imageInfo.format=enDsviImagePixelFormat_NOTSUPPORTED;
                        break;
                }

                if(imageInfo.format!=enDsviImagePixelFormat_RGB888)
                {
                    _status         =   DSVI_STATUS_NOT_SUPPORTED;
                }else
                {
                    imageInfo.height    =   header->height;
                    imageInfo.width     =   header->width;
                    newImage            =   dsvi_image_new(&imageInfo,&_status);
                    if(newImage==NULL)
                    {
                        _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                    }else
                    {
                        int8_t* buf_image   = (int8_t*)(*_DsviMemHandle.image_alloc)(header->imageSize*sizeof(int8_t));

                        if(buf_image==NULL)
                        {
                            _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                        }else
                        {
                            fseek(BitmapHandle,header->dataoffset,SEEK_SET);
                            fread(buf_image,header->imageSize,1,BitmapHandle);
                            //
                            //locals
                            uint32_t dataIndex=0,srcIndex=0;
                            uint32_t biWidthInBytes =   header->width*header->bitsPerPixel;
                            biWidthInBytes          =   (biWidthInBytes%8==0)?  (biWidthInBytes/8):((biWidthInBytes/8)+1);

                            uint32_t biWidthWithPadding =   (biWidthInBytes%4==0)? biWidthInBytes:((biWidthInBytes-(biWidthInBytes%4))+4);

                            int32_t rowIndex,colIndex;

                            for(rowIndex=(header->height-1);rowIndex>=0;rowIndex--)
                            {
                                srcIndex    =   (rowIndex*biWidthWithPadding);
                                for(colIndex=0;colIndex<biWidthInBytes;colIndex++)
                                {
                                    newImage->data[dataIndex++] = buf_image[srcIndex++];
                                }
                            }
                            (*_DsviMemHandle.image_free)(buf_image);
                        }
                    }
                }

            }
            fclose(BitmapHandle);
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

dsvi_status_t dsvi_image_bitmap_write(const dsvi_Image_t* srcImage,const char* filename)
{
    dsvi_status_t _status   =   dsvi_image_getMemConfig();
    FILE* fileHandle =   fopen(filename,"wb");
    extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(_status==DSVI_STATUS_OK)
    {
        if(srcImage==NULL)
        {
            _status =   DSVI_STATUS_NULL_ARG;
        }else
        {
            if(fileHandle==NULL)
            {
                _status = DSVI_STATUS_MEMORY_READ_FAILED;
            }else
            {
                //local variables
                uint8_t  bitsPerPixel=0;
                uint32_t widthInBytes=0,widthWithPadding=0,imageSize=0,dataOffset=0,fileSize=0,matrixSize=0;

                switch(srcImage->format)
                {
                case enDsviImagePixelFormat_BINARY:
                    bitsPerPixel=1;
                    break;
                case enDsviImagePixelFormat_RGB888:
                    bitsPerPixel=24;
                    break;
                case enDsviImagePixelFormat_RGB565:
                    bitsPerPixel=16;
                    break;
                default:
                    bitsPerPixel=0;  //error case return right away
                    break;
                }

                if(srcImage->format==enDsviImagePixelFormat_NOTSUPPORTED)
                {
                    _status = DSVI_STATUS_NOT_SUPPORTED;
                }else
                {
                    widthInBytes        =   srcImage->width*bitsPerPixel;
                    widthInBytes        =   (widthInBytes%8==0)?    (widthInBytes/8):((widthInBytes/8)+1);
                    widthWithPadding    =   (widthInBytes%4==0)?    widthInBytes:(widthInBytes-(widthInBytes%4)+4);
                    dataOffset          =   ((srcImage->format==enDsviImagePixelFormat_BINARY)? 62:54);
                    imageSize           =   widthInBytes*srcImage->height;
                    matrixSize          =   widthWithPadding*srcImage->height;
                    fileSize            =   matrixSize+dataOffset;

                char* buf_file      =  NULL;
                //int* p = (*_DsviMemHandle.image_alloc)(sizeof(int));
                buf_file            =  (char*)(*_DsviMemHandle.image_alloc)(fileSize*sizeof(char));

                if(buf_file==NULL)
                {
                    _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    //all memory write operations
                    //Bitmap File Header--------------------------------------------------------------------------------------------
                    buf_file[0] =   'B';
                    buf_file[1] =   'M';
                    //FileSize 4bytes
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_FHDR_FILESIZE],fileSize,sizeof(fileSize));
                    //reserved 4 bytes [0]
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_FHDR_RESERVED],0,4);
                    //img mtx offset : ??
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_FHDR_DATAOFFSET],dataOffset,sizeof(dataOffset));

                    //BITMAP INFO HEADER -> Size-------------------------------------------------------------------------------------
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_INFOHEADER],DSVI_BITMAP_INFOHEADER_SIZE,4);
                    //width & height in pixel (unsigned)
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_WIDTH],srcImage->width,4);
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_HEIGHT],srcImage->height,4);
                    //num_planes => fixed [1]
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_PLANES],1,2);
                    //bits per pixel
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_BITSPERPIXEL],bitsPerPixel,2);
                    //compression method
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_COMPRESSION],0,4);
                    //Img Mtx Size
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_IMAGESIZE],imageSize,4);
                    //resolution -> fixed [0]
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_XPIXPERM],0,4);
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_YPIXPERM],0,4);
                    //colors in palette -> Semi-Fixed [0] for BINARY
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_COLORSUSED],0,4);  //check for other formats
                    //num_imp colors -> fixed [0]
                    dsvi_bufferWrite_LE(&buf_file[DSVI_BITMAP_OFFSET_IHDR_IMPCOLORS],0,4);
                    //print Color Palette--------------------------------------------------------------------------------------------
                    if(srcImage->format==enDsviImagePixelFormat_BINARY)
                    {
                        dsvi_bufferWrite_LE(&buf_file[0x36],0x0,4);
                        dsvi_bufferWrite_LE(&buf_file[0x3a],0xffffff,4);
                    }//add more color palettes for other formats here

                    int32_t    rowIndex,colIndex,srcIndex,dstIndex;

                    dstIndex    =   dataOffset;
                    //Write Matrix data----------------------------------------------------------------------------------------------

                    for(rowIndex=(srcImage->height-1);rowIndex>=0;rowIndex--)
                    {
                        srcIndex    =   rowIndex*widthInBytes;
                        for(colIndex=0;colIndex<widthInBytes;colIndex++)
                        {
                            buf_file[dstIndex++]    =   srcImage->data[srcIndex++];
                        }
                        for(colIndex=widthInBytes;colIndex<widthWithPadding;colIndex++)
                        {
                            buf_file[dstIndex++]    =   0;
                        }
                    }
                    fwrite(buf_file,fileSize,1,fileHandle);
                    (*_DsviMemHandle.image_free)(buf_file);
                }
                fclose(fileHandle);
                }
            }
        }
    }

    //
    return _status;
}



