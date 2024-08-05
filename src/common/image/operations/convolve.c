#include <dishanveshi/image/operations/convolve.h>
#include <dishanveshi/image/image.h>
#include <stdlib.h>
#include <math.h>

dsvi_Matrix_t* dsvi_image_getMtxFromImage(const dsvi_Image_t* srcImage,dsvi_status_t* status)
{
    dsvi_status_t _status     =   DSVI_STATUS_OK;
    dsvi_Matrix_t* newMatrix   =   NULL;
    extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(srcImage==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        if(dsvi_image_getMemConfig()!=DSVI_STATUS_OK)
        {
            _status =   DSVI_STATUS_NOT_IMPLEMENTED;
        }else
        {
            newMatrix   =   (*_DsviMemHandle.image_alloc)(sizeof(dsvi_Matrix_t));

            if(newMatrix==NULL)
            {
                _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
            }else
            {
                newMatrix->height   =   srcImage->height;
                newMatrix->width    =   srcImage->width;
                newMatrix->el       =   (*_DsviMemHandle.image_alloc)(newMatrix->width*newMatrix->height*sizeof(int16_t));

                if(newMatrix->el==NULL)
                {
                    _status =   DSVI_STATUS_NULL_ARG;
                    (*_DsviMemHandle.image_free)(newMatrix);
                }else
                {
                    uint16_t rowIndex,colIndex=0;
                    uint32_t elIndex=0;

                    for(rowIndex=0;rowIndex<srcImage->height;rowIndex++)
                    {
                        //elIndex =   (rowIndex*srcImage->height)+colIndex; //redundant code here
                        for(colIndex=0;colIndex<srcImage->width;colIndex++)
                        {
                            newMatrix->el[elIndex]   =   srcImage->data[elIndex];
                            elIndex++;
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
        return newMatrix;
    }else
    {
        return NULL;
    }
}

dsvi_Image_t*   dsvi_image_getImageFromMtx(const dsvi_Matrix_t* srcMatrix,dsvi_status_t* status)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;
    dsvi_Image_t* newImage  =   NULL;

    if(srcMatrix==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        ImageInfo_t newInfo;
        newInfo.format  =   enDsviImagePixelFormat_DEFAULT;
        newInfo.width   =   srcMatrix->width;
        newInfo.height  =   srcMatrix->height;

        newImage        =   dsvi_image_new(&newInfo,&_status);

        if(_status  ==  DSVI_STATUS_OK)
        {
            uint16_t rowIndex,colIndex=0;
            uint32_t elIndex=0;

            for(rowIndex=0;rowIndex<newImage->height;rowIndex++)
            {
                for(colIndex=0;colIndex<newImage->width;colIndex++)
                {
                    newImage->data[elIndex] =   (int16_t)srcMatrix->el[elIndex];
                    elIndex++;
                }
            }
        }
    }

    if(status!=NULL)
    {
        (*status)   =   _status;
    }
    if(_status  ==DSVI_STATUS_OK)
    {
        return newImage;
    }else
    {
        return NULL;
    }
}

dsvi_Matrix_t* dsvi_image_convolve(const dsvi_Matrix_t* srcMatrix, const dsvi_Kernel_t* kernel, dsvi_status_t* status)
{
    dsvi_status_t _status   =  DSVI_STATUS_OK;
    dsvi_Matrix_t* newMatrix   =   NULL;

    if((srcMatrix==NULL)||(kernel==NULL))
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        newMatrix           =   (dsvi_Matrix_t*)malloc(sizeof(dsvi_Matrix_t));   //typecasting from kernel to matrix &/ vice-versa??

        if(newMatrix==NULL)
        {
            _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
        }else
        {
            uint32_t size       =   0;
            newMatrix->width    =   srcMatrix->width-kernel->width+1;  //padding is considered to be all zero,convolved image size is maintained the same as the src
            newMatrix->height   =   srcMatrix->height-kernel->height+1;
            size                =   newMatrix->width*newMatrix->height;
            newMatrix->el       =   (int16_t*)malloc(size*sizeof(int16_t));   //use variables from image.c

            if(newMatrix->el==NULL)
            {
                free(newMatrix);
                _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
            }else
            {
                int32_t srcIndex=0,kernelIndex=0,kx=0,ky=0,destIndex=0;

                int32_t sop =   0;
                int32_t LatchXEnd           =   newMatrix->height;
                int32_t LatchYEnd           =   newMatrix->width;
                int32_t LatchX,LatchY;

                for(LatchX=0;LatchX<LatchXEnd;LatchX++)
                {
                    for(LatchY=0;LatchY<LatchYEnd;LatchY++)
                    {
                        //
                        sop=0;
                        for(kx=0;kx<kernel->height;kx++)

                        {
                            for(ky=0;ky<kernel->width;ky++)
                            {
                                kernelIndex =   (kx*kernel->width)+ky;
                                srcIndex    =   ((LatchX+kx)*(srcMatrix->width))+(LatchY+ky);
                                sop+=(kernel->el[kernelIndex])*(srcMatrix->el[srcIndex]);
                            }
                        }
                        newMatrix->el[destIndex++]    =   sop;
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
        return newMatrix;
    }else
    {
        return NULL;
    }
}

dsvi_RealMatrix_t* dsvi_image_RKConvolve(const dsvi_Matrix_t* srcMatrix, const dsvi_RealKernel_t* kernel, dsvi_status_t* status)
{
    //check validity of kernel-values
    //do convolution
    //check number validity
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    dsvi_RealMatrix_t* newMatrix = NULL;

    if(_status == DSVI_STATUS_OK)
    {
        uint16_t row_i=0,col_i=0;
        for(row_i=0;row_i<(kernel->height*kernel->width);row_i++)
        {
            if(isnan(kernel->el[row_i])||(!isfinite(kernel->el[row_i])))
            {
                col_i=1;
                break;
            }
        }

        if(col_i!=0)
        {
            _status = DSVI_STATUS_INVALID_ARG;
        }else
        {
            newMatrix   =   (*_DsviMemHandle.image_alloc)(sizeof(dsvi_RealMatrix_t));

            if(newMatrix == NULL)
            {
                _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
            }else
            {
                uint32_t mtxSize = srcMatrix->width*srcMatrix->height;

                newMatrix->el    = (*_DsviMemHandle.image_alloc)(mtxSize*sizeof(real_t));

                if(newMatrix->el == NULL)
                {
                    _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                    //(*_DsviMemHandle.image_free)(newMatrix);
                    newMatrix = NULL;
                }else
                {
                    newMatrix->height = srcMatrix->height-kernel->height+1;
                    newMatrix->width  = srcMatrix->width-kernel->width+1;

                    int32_t srcIndex=0,kernelIndex=0,kx=0,ky=0,destIndex=0;

                    real_t sop =   0;
                    int32_t LatchXEnd           =   newMatrix->height;
                    int32_t LatchYEnd           =   newMatrix->width;
                    int32_t LatchX,LatchY;

                    for(LatchX=0;LatchX<LatchXEnd;LatchX++)
                    {
                        for(LatchY=0;LatchY<LatchYEnd;LatchY++)
                        {
                            //
                            sop=0;
                            for(kx=0;kx<kernel->height;kx++)

                            {
                                for(ky=0;ky<kernel->width;ky++)
                                {
                                    kernelIndex =   (kx*kernel->width)+ky;
                                    srcIndex    =   ((LatchX+kx)*(srcMatrix->width))+(LatchY+ky);
                                    sop+=(kernel->el[kernelIndex])*(srcMatrix->el[srcIndex]);
                                }
                            }
                            newMatrix->el[destIndex++]    =   sop;
                        }
                    }
                }
            }
        }
    }

    if(status != NULL)
    {
        (*status) = _status;
    }

    return newMatrix;
}

dsvi_RealMatrix_t* dsvi_image_RKConvolve_preserveSize(const dsvi_Matrix_t* srcMatrix, const dsvi_RealKernel_t* kernel, dsvi_status_t* status)
{
    //check validity of kernel-values
    //do convolution
    //check number validity
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    dsvi_RealMatrix_t* newMatrix = NULL;

    if(_status == DSVI_STATUS_OK)
    {
        uint16_t row_i=0,col_i=0;
        for(row_i=0;row_i<(kernel->height*kernel->width);row_i++)
        {
            if(isnan(kernel->el[row_i])||(!isfinite(kernel->el[row_i])))
            {
                col_i=1;
                break;
            }
        }

        if(col_i!=0)
        {
            _status = DSVI_STATUS_INVALID_ARG;
        }else
        {
            newMatrix   =   (*_DsviMemHandle.image_alloc)(sizeof(dsvi_RealMatrix_t));

            if(newMatrix == NULL)
            {
                _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
            }else
            {
                uint32_t mtxSize = srcMatrix->width*srcMatrix->height;

                newMatrix->el    = (*_DsviMemHandle.image_alloc)(mtxSize*sizeof(real_t));

                if(newMatrix->el == NULL)
                {
                    _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                    //(*_DsviMemHandle.image_free)(newMatrix);
                    newMatrix = NULL;
                }else
                {
                    newMatrix->height = srcMatrix->height;
                    newMatrix->width  = srcMatrix->width;

                    int32_t srcIndex=0,kernelIndex=0,kx=0,ky=0,destIndex=0;

                    real_t sop =   0;
                    int32_t LatchXEnd           =   newMatrix->height;
                    int32_t LatchYEnd           =   newMatrix->width;
                    int32_t LatchX,LatchY;

                    for(LatchX=0;LatchX<LatchXEnd;LatchX++)
                    {
                        for(LatchY=0;LatchY<LatchYEnd;LatchY++)
                        {
                            //
                            if((LatchX<(kernel->height/2))||(LatchX>((srcMatrix->height-kernel->height)/2)))
                            {
                                newMatrix->el[destIndex] = srcMatrix->el[destIndex];
                                destIndex++;
                            }else
                            if((LatchY<(kernel->width/2))||(LatchY>((srcMatrix->width-kernel->width)/2)))
                            {
                                newMatrix->el[destIndex] = srcMatrix->el[destIndex];
                                destIndex++;
                            }else
                            {
                                sop=0;
                                for(kx=0;kx<kernel->height;kx++)

                                {
                                    for(ky=0;ky<kernel->width;ky++)
                                    {
                                        kernelIndex =   (kx*kernel->width)+ky;
                                        srcIndex    =   ((LatchX+kx)*(srcMatrix->width))+(LatchY+ky);
                                        sop+=(kernel->el[kernelIndex])*(srcMatrix->el[srcIndex]);
                                    }
                                }
                                newMatrix->el[destIndex++]    =   sop;
                            }
                        }
                    }
                }
            }
        }
    }

    if(status != NULL)
    {
        (*status) = _status;
    }

    return newMatrix;
}

dsvi_Image_t*  dsvi_image_getImageFromRealMatrix(const dsvi_RealMatrix_t* srcMatrix,dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    dsvi_Image_t* newImage = NULL;

    if(_status == DSVI_STATUS_OK)
    {
        if((srcMatrix==NULL)||(status==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            uint32_t el_i=0,size = srcMatrix->height*srcMatrix->width;

            ImageInfo_t newInfo = {0};
            newInfo.format  =   enDsviImagePixelFormat_DEFAULT;
            newInfo.width   =   srcMatrix->width;
            newInfo.height  =   srcMatrix->height;

            newImage =  dsvi_image_new(&newInfo,&_status);

            if(_status == DSVI_STATUS_OK)
            {
                real_t el_max=0.0,temp=0.0;

                for(el_i=0;el_i<size;el_i++)
                {
                    //check real_t type validity??
                    el_max = (el_max>srcMatrix->el[el_i])?  el_max:srcMatrix->el[el_i];
                }

                for(el_i=0;el_i<size;el_i++)
                {
                    temp = ((srcMatrix->el[el_i]*255)/el_max);
                    if(temp>255)
                    {
                        temp = 255;
                    }else
                    if(temp<0)
                    {
                        temp = 0;
                    }
                    newImage->data[el_i] = (uint8_t)temp;
                }
            }
        }
    }

    (*status) = DSVI_STATUS_OK;
    return newImage;
}

dsvi_status_t dsvi_image_deleteMatrix(dsvi_Matrix_t* srcMatrix)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(_status==DSVI_STATUS_OK)
    {
        if(srcMatrix==NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcMatrix->el==NULL)
            {
                _status = DSVI_STATUS_NULL_ARG;
            }else
            {
                (*_DsviMemHandle.image_free)(srcMatrix->el);
                srcMatrix->el=NULL;
            }
            (*_DsviMemHandle.image_free)(srcMatrix);
            srcMatrix=NULL;
        }
    }
    return _status;
}

dsvi_status_t dsvi_image_deleteRealMatrix(dsvi_RealMatrix_t* srcMatrix)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(_status==DSVI_STATUS_OK)
    {
        if(srcMatrix==NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcMatrix->el==NULL)
            {
                _status = DSVI_STATUS_NULL_ARG;
            }else
            {
                (*_DsviMemHandle.image_free)(srcMatrix->el);
                srcMatrix->el=NULL;
            }
            (*_DsviMemHandle.image_free)(srcMatrix);
            srcMatrix=NULL;
        }
    }
    return _status;
}
