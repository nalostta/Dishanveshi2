#include <dishanveshi/image/operations/queue.h>
#include <dishanveshi/image/operations/CCL.h>

#define checkPixForLabel(index,label) do{\
                            if((srcImage->data[index]>=threshold)&&(srcImage->label[index]==0)){\
                                srcImage->label[index]  =   label;\
                                _status = dsvi_imageOp_qPush(&head,index);}}while(0)

#define pixCol(temp_i) temp_i%srcImage->width
#define pixRow(temp_i) temp_i/srcImage->width
#define Equivalency_List_Buffer_Size 250
#define label2ColorImg_maxTh    250
#define label2ColorImg_minTh    50

const uint32_t regionBoundaryBufferSize = 101;
dsvi_status_t dsvi_regionBoundary_delete(dsvi_regionBoundary_t* srcBdry);

dsvi_status_t dsvi_imageOp_label(dsvi_Image_t* srcImage,uint8_t threshold)
{
    dsvi_status_t _status   =   dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(_status == DSVI_STATUS_OK)
    {
        if(srcImage == NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format != enDsviImagePixelFormat_DEFAULT)
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                uint32_t imSize = srcImage->width*srcImage->height;
                srcImage->label     = (dsvi_Label_t*)(*_DsviMemHandle.image_alloc)(imSize*sizeof(dsvi_Label_t));

                if(srcImage->label==NULL)
                {
                    _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    dsvi_Label_t label=0;

                    uint16_t row_i=0,col_i=0;
                    uint32_t pix_i=0,temp_i=0;
                    uint8_t  firstRow=0,lastRow=0,firstCol=0,lastCol=0;

                    dsvi_Queue_t* head = NULL;

                    for(pix_i=0;pix_i<imSize;pix_i++)srcImage->label[pix_i]=0;

                    for(row_i=0;row_i<srcImage->height;row_i++)
                    {
                        pix_i = row_i*srcImage->width;
                        for(col_i=0;col_i<srcImage->width;col_i++)
                        {
                            //printf("0");
                            if(label+1==0)
                            {
                                return DSVI_STATUS_ERROR;
                            }

                            if((srcImage->data[pix_i]>=threshold)&&(srcImage->label[pix_i]==0))
                            {
                                label++;
                            }

                            //section 1
                            checkPixForLabel(pix_i,label);

                            //section 2
                            while(head != NULL)
                            {
                                temp_i = dsvi_imageOp_qPop(&head,&_status);
                                //sequence
                                firstRow    =   pixRow(temp_i)==0;
                                firstCol    =   pixCol(temp_i)==0;
                                lastRow     =   pixRow(temp_i)==(srcImage->height-1);
                                lastCol     =   pixCol(temp_i)==(srcImage->width-1);

                                //NORTH PIXEL
                                //NORTH-WEST PIXEL
                                if((!firstRow)&&(!firstCol))
                                {
                                    checkPixForLabel((temp_i-srcImage->width-1),label);
                                }

                                //WEST PIXEL
                                if(!firstCol)
                                {
                                    checkPixForLabel(temp_i-1,label);
                                }

                                //SOUTH-WEST PIXEL
                                if((!firstCol)&&(!lastRow))
                                {
                                    checkPixForLabel((temp_i+srcImage->width-1),label);
                                }
                                //SOUTH PIXEL
                                if(!lastRow)
                                {
                                    checkPixForLabel((temp_i+srcImage->width),label);
                                }
                                //SOUTH-EAST PIXEL
                                if((!lastCol)&&(!lastRow))
                                {
                                    checkPixForLabel((temp_i+srcImage->width+1),label);
                                }
                                //EAST PIXEL
                                if(!lastCol)
                                {
                                    checkPixForLabel((temp_i+1),label);
                                }
                                //NORTH EAST PIXEL
                                if((!lastCol)&&(!firstRow))
                                {
                                    checkPixForLabel((temp_i-srcImage->width+1),label);
                                }
                            }
                            pix_i++;
                        }
                    }
                }
            }
        }
    }

    return _status;
}

dsvi_Image_t* dsvi_imageOp_debugLabels(const dsvi_Image_t* srcImage,dsvi_status_t* status)
{
    dsvi_status_t _status  = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;

    if(_status == DSVI_STATUS_OK)
    {
        if(srcImage == NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            uint16_t row_i=0,col_i=0;
            uint32_t pix_i=0,labelStep=0;

            for(row_i=0;row_i<srcImage->height;row_i++)
            {
                for(col_i=0;col_i<srcImage->width;col_i++)
                {
                    labelStep = ((labelStep>srcImage->label[pix_i])?   labelStep:srcImage->label[pix_i]);
                    pix_i++;
                }
            }
            uint32_t k   =   4294967290;
            labelStep    =   (k/(labelStep+1))+10;

            if(labelStep == 0)
            {
                _status =   DSVI_STATUS_NOT_SUPPORTED;
            }else
            {


                ImageInfo_t newInfo = {0};
                newInfo.height  =   srcImage->height;
                newInfo.width   =   srcImage->width;
                newInfo.format  =   srcImage->format;

                newImage = dsvi_image_new(&newInfo,&_status);

                if(_status == DSVI_STATUS_OK)
                {
                    pix_i=0;
                    for(row_i=0;row_i<srcImage->height;row_i++)
                    {
                        for(col_i=0;col_i<srcImage->width;col_i++)
                        {
                            newImage->data[pix_i]   =   srcImage->label[pix_i]*labelStep; //issue here
                            pix_i++;
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

    return newImage;
}

int dsvi_imageOp_checkNeigbhorForegnd_startEastClkwise(dsvi_Image_t* srcImage,uint16_t x,uint16_t y,uint8_t threshold,uint8_t prevNbr,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    int relPosn = -1;
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
            if(prevNbr>7||prevNbr<0)
            {
                _status = DSVI_STATUS_INVALID_ARG;
            }else
            {
                //check boundary conditions
                if((x>srcImage->width)||(y>srcImage->height))
                {
                    _status = DSVI_STATUS_INVALID_OPERATION;
                }else
                {
                    uint32_t el_i;
                    int nbrStatus[8] = {0};
                    //1. try east
                    //check whether a point in east exists
                    if((x>0)&&(y>0))
                    {
                        el_i = (((y-1)*srcImage->width)+x-1);
                        if(srcImage->data[el_i]>=threshold)
                        {
                            nbrStatus[1] = 1;
                        }
                        printf("%d",srcImage->data[el_i]);
                    }else
                    {
                        printf("%d",-2);
                    }

                    //north
                    if(x>0)
                    {
                        el_i = (((y-1)*srcImage->width)+x);
                        if(srcImage->data[el_i]>=threshold)
                        {
                            nbrStatus[2] = 1;
                        }
                        printf(" %d",srcImage->data[el_i]);
                    }else
                    {
                        printf(" %d",-2);
                    }

                    //
                    if((x<srcImage->width)&&(y>0))
                    {
                        el_i = (((y-1)*srcImage->width)+x+1);
                        if(srcImage->data[el_i]>=threshold)
                        {
                            nbrStatus[3] = 1;
                        }
                        printf(" %d\n",srcImage->data[el_i]);
                    }else
                    {
                        printf(" %d\n",-2);
                    }


                    //west
                    if(x!=0)
                    {
                        //check if the east pixel is in the foreground
                        el_i = ((y*srcImage->width)+x-1);
                        if(srcImage->data[el_i]>=threshold)
                        {
                            nbrStatus[0] = 1;
                        }
                        printf("%d x",srcImage->data[el_i]);
                    }else
                    {
                        printf("%d x",-2);
                    }
                    //
                    if(x<srcImage->width)
                    {
                        el_i = ((y*srcImage->width)+x+1);
                        if(srcImage->data[el_i]>=threshold)
                        {
                            nbrStatus[4] = 1;
                        }
                        printf(" %d\n",srcImage->data[el_i]);
                    }else
                    {
                        printf(" %d\n",2);
                    }

                    if((relPosn<0)&&(y<srcImage->height))
                    {
                        el_i = (((y+1)*srcImage->width)+x);
                        if(srcImage->data[el_i]>=threshold)
                        {
                            nbrStatus[6] = 1;
                        }
                        printf("%d",srcImage->data[el_i]);
                    }else
                    {
                        printf("%d",-2);
                    }

                    if((x<srcImage->width)&&(y<srcImage->height))
                    {
                        el_i = (((y+1)*srcImage->width)+x+1);
                        if(srcImage->data[el_i]>=threshold)
                        {
                            nbrStatus[5] = 1;
                        }
                        printf(" %d",srcImage->data[el_i]);
                    }else
                    {
                        printf(" %d",-2);
                    }

                    if((relPosn<0)&&(x>0)&&(y<srcImage->height))
                    {
                        el_i = (((y+1)*srcImage->width)+x-1);
                        if(srcImage->data[el_i]>=threshold)
                        {
                            nbrStatus[7] = 1;
                        }
                        printf(" %d\n",srcImage->data[el_i]);
                    }else
                    {
                        printf(" %d\n",-2);
                    }

                    uint8_t curr_i = ((prevNbr==7)? 0:prevNbr+1);
                    uint8_t next_i = ((curr_i==7)?  0:curr_i+1);

                    uint8_t curr_el = nbrStatus[curr_i];
                    uint8_t next_el = nbrStatus[next_i];

                    printf("\n\n[debug]\n%d %d %d\n%d x %d\n%d %d %d\n\n",nbrStatus[1],nbrStatus[2],nbrStatus[3],nbrStatus[4],nbrStatus[5],nbrStatus[6],nbrStatus[7],nbrStatus[0]);

                    while(curr_el==next_el)
                    {
                        if(curr_i==prevNbr)
                        {
                            _status = DSVI_STATUS_ERROR;
                            break;
                        }
                        curr_i = next_i;
                        next_i = ((curr_i==7)?  0:curr_i+1);

                        curr_el = nbrStatus[curr_i];
                        next_el = nbrStatus[next_i];
                    }

                    if(_status==DSVI_STATUS_OK)
                    {
                        if(curr_el&&(!next_el))
                        {
                            return curr_i;
                        }else
                        if(next_el&&(!curr_el))
                        {
                            return next_i;
                        }else
                        {
                            return -1;
                        }
                    }
                }
            }
        }
    }
    if(status!=NULL)
    {
        *status = _status;
    }

    return relPosn;
}

dsvi_regionBoundary_t* dsvi_regionBoundary_fixed_new(dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    dsvi_regionBoundary_t* newBoundary = NULL;
    extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(_status == DSVI_STATUS_OK)
    {
        newBoundary = (*_DsviMemHandle.image_alloc)(sizeof(dsvi_regionBoundary_t));
        if(newBoundary==NULL)
        {
            _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
        }else
        {
            newBoundary->counts = (*_DsviMemHandle.image_alloc)(regionBoundaryBufferSize*sizeof*(newBoundary->counts));
            if(newBoundary->counts==NULL)
            {
                _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                dsvi_regionBoundary_delete(newBoundary);
            }else
            {
                newBoundary->nextRelative = (*_DsviMemHandle.image_alloc)(regionBoundaryBufferSize*sizeof*(newBoundary->nextRelative));
                if(newBoundary->nextRelative==NULL)
                {
                    _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                    dsvi_regionBoundary_delete(newBoundary);
                }
            }
        }
    }
    if(status!=NULL)
    {
        *status = _status;
    }
    return newBoundary;
}

dsvi_status_t dsvi_regionBoundary_delete(dsvi_regionBoundary_t* srcBdry)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    if(_status==DSVI_STATUS_OK)
    {
        if(srcBdry!=NULL)
        {
            if(srcBdry->counts!=NULL)
            {
                (*_DsviMemHandle.image_free)(srcBdry->counts);
                srcBdry->counts = NULL;
            }
            if(srcBdry->nextRelative!=NULL)
            {
                (*_DsviMemHandle.image_free)(srcBdry->nextRelative);
                srcBdry->nextRelative = NULL;
            }
            (*_DsviMemHandle.image_free)(srcBdry);
            srcBdry = NULL;
        }
    }
    return _status;
}

dsvi_regionBoundary_t* dsvi_imageOp_getRegionBoundary(dsvi_Image_t* srcImage,uint16_t originX,uint16_t originY,uint8_t threshold,dsvi_status_t* status)
{
    dsvi_status_t _status  = dsvi_image_getMemConfig();
    dsvi_regionBoundary_t* newBdry = NULL;
    if(_status == DSVI_STATUS_OK)
    {
        if(srcImage==NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if((originX>srcImage->width)||(originY>srcImage->height))
            {
                _status = DSVI_STATUS_INVALID_ARG;
            }else
            {
                newBdry = dsvi_regionBoundary_fixed_new(&_status);
                if(_status==DSVI_STATUS_OK)
                {
                    //get boundary
                    uint16_t x = originY;
                    uint16_t y = originX;

                    uint32_t usedBuffer_i = -1;
                    int8_t nextRelative;
                    int8_t prevRelative = 4;
                    uint32_t count = 0;

                    do
                    {
                        if(usedBuffer_i==regionBoundaryBufferSize)
                        {
                            _status = DSVI_STATUS_MEMORY_ERROR;
                            dsvi_regionBoundary_delete(newBdry);
                            break;
                        }
                        nextRelative = dsvi_imageOp_checkNeigbhorForegnd_startEastClkwise(srcImage,x,y,threshold,prevRelative,&_status);
                        if(nextRelative<0||nextRelative>7)
                        {
                            _status = DSVI_STATUS_ERROR;
                            dsvi_regionBoundary_delete(newBdry);
                            break;
                        }else
                        {
                            if(nextRelative!=prevRelative)
                            {
                                usedBuffer_i++;
                                count = 0;
                                newBdry->nextRelative[usedBuffer_i] = nextRelative;
                                newBdry->counts[usedBuffer_i] = count;
                                prevRelative = nextRelative;
                            }else
                            {
                                count++;
                            }
                            switch(nextRelative)
                            {
                            case 0:
                                x--;
                                break;
                            case 1:
                                x--;
                                y--;
                                break;
                            case 2:
                                y--;
                                break;
                            case 3:
                                x++;
                                y--;
                                break;
                            case 4:
                                x++;
                                break;
                            case 5:
                                x++;
                                y++;
                                break;
                            case 6:
                                y++;
                                break;
                            case 7:
                                x--;
                                y++;
                                break;
                            default :
                                _status = DSVI_STATUS_ERROR; //just a formality, prev if condition checks for this
                                break;
                            }
                        }
                    }while((x!=originX)&&(y!=originY));
                    newBdry->length = usedBuffer_i;
                }
            }
        }
    }
    if(_status!=DSVI_STATUS_OK)
    {
        dsvi_regionBoundary_delete(newBdry);
    }

    if(status!=NULL)
    {
        *status = _status;
    }
    return newBdry;
}

dsvi_status_t dsvi_imageOp_writeRegionBoundaryToFile(dsvi_regionBoundary_t* srcBdry,const char* path)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if((path==NULL)||(srcBdry==NULL))
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        if(srcBdry->length==0)
        {
            _status = DSVI_STATUS_NOT_IMPLEMENTED;
        }else
        {
            if((srcBdry->nextRelative==NULL)||(srcBdry->counts==NULL))
            {
                _status = DSVI_STATUS_INVALID_ARG;
            }else
            {
                FILE* fp = fopen(path,"w");
                if(fp==NULL)
                {
                    _status = DSVI_STATUS_NULL_ARG;
                }else
                {
                    uint32_t line_i;
                    for(line_i=0;line_i<srcBdry->length;line_i++)
                    {
                        fprintf(fp,"%d:%d\n",srcBdry->nextRelative[line_i],srcBdry->counts[line_i]);
                    }
                    fclose(fp);
                }
            }
        }
    }
    return _status;
}

dsvi_status_t dsvi_image_labelRegion(dsvi_Image_t* srcImage,uint8_t threshold)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    if(_status==DSVI_STATUS_OK)
    {
        if(srcImage==NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->format != enDsviImagePixelFormat_DEFAULT)
            {
                _status = DSVI_STATUS_NOT_SUPPORTED;
            }else
            {
                uint8_t* eqList = (*_DsviMemHandle.image_alloc)(Equivalency_List_Buffer_Size*sizeof*eqList);

                if(eqList==NULL)
                {
                    _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    if(srcImage->label!=NULL)
                    {
                        (*_DsviMemHandle.image_free)(srcImage->label);
                        srcImage->label=NULL;
                    }

                    uint32_t imSize = (srcImage->width*srcImage->height);
                    srcImage->label = (*_DsviMemHandle.image_alloc)(imSize*sizeof(dsvi_Label_t)); //302736

                    if(srcImage->label==NULL)
                    {
                        _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                    }else
                    {
                        uint16_t row_i,col_i;
                        uint32_t pix_i;
                        uint8_t northNbr,westNbr;
                        dsvi_Label_t label=1;
                        dsvi_Label_t tempLabel=0;

                        for(pix_i=0;pix_i<(srcImage->width*srcImage->height);pix_i++)
                        {
                            srcImage->label[pix_i] = 0;
                        }

                        int i;
                        for(i=0;i<Equivalency_List_Buffer_Size;i++)
                        {
                           eqList[i] = i;
                        }

                        for(row_i=0;row_i<srcImage->height;row_i++)
                        {
                            for(col_i=0;col_i<srcImage->width;col_i++)
                            {
                                pix_i = (row_i*srcImage->width)+col_i;

                                if(srcImage->data[pix_i]>=threshold)
                                {
                                    northNbr = ((row_i>0)?  srcImage->label[pix_i-srcImage->width]:0);
                                    westNbr  = ((col_i>0)?  srcImage->label[pix_i-1]:0);

                                    if((northNbr<=0)&&(westNbr<=0))
                                    {
                                        srcImage->label[pix_i] = label;
                                        label++;
                                    }else
                                    if((northNbr<=0)&&(westNbr>0))
                                    {
                                        srcImage->label[pix_i] = srcImage->label[pix_i-1];
                                    }else
                                    if((northNbr>0)&&(westNbr<=0))
                                    {
                                        srcImage->label[pix_i] = srcImage->label[pix_i-srcImage->width];
                                    }else
                                    {
                                        if(westNbr<=northNbr)
                                        {
                                            if((northNbr<Equivalency_List_Buffer_Size)&&(westNbr<Equivalency_List_Buffer_Size))
                                            {
                                                tempLabel = westNbr;
                                                int limit = 0;
                                                while((tempLabel!=eqList[tempLabel])||(limit++>250))
                                                {
                                                    tempLabel = eqList[tempLabel];
                                                }
                                                eqList[northNbr] = tempLabel;
                                            }else
                                            {
                                                _status = DSVI_STATUS_ERROR;
                                                break;
                                            }
                                            srcImage->label[pix_i] = westNbr;
                                        }else
                                        {
                                            if((northNbr<Equivalency_List_Buffer_Size)&&(westNbr<Equivalency_List_Buffer_Size))
                                            {
                                                int limit = 0;
                                                tempLabel = northNbr;
                                                while((tempLabel!=eqList[tempLabel])||(limit++>250))
                                                {
                                                    tempLabel = eqList[tempLabel];
                                                }
                                                eqList[westNbr] = tempLabel;
                                            }else
                                            {
                                                _status = DSVI_STATUS_ERROR;
                                                break;
                                            }
                                            srcImage->label[pix_i] = northNbr;
                                        }
                                    }
                                }
                            }
                        }
                        printf("[debug] label max : %d\n\n",label);

                        for(row_i=0;row_i<srcImage->height;row_i++)
                        {
                            for(col_i=0;col_i<srcImage->width;col_i++)
                            {
                                pix_i = ((row_i*srcImage->width)+col_i);
                                label = srcImage->label[pix_i];
                                int limit = 0;
                                while((label!=eqList[label])||(limit++>200))
                                {
                                    label = eqList[label];
                                }
                                srcImage->label[pix_i] = label;
                            }
                        }
                        //debug code
                        /*printf("\n[debug] equivalency list :\n\n");
                        for(i=0;i<Equivalency_List_Buffer_Size;i++)
                        {
                            printf("%d : %d\n",i,eqList[i]);
                        }*/

                    }
                    (*_DsviMemHandle.image_free)(eqList);
                }

            }
        }
    }
    return _status;
}

dsvi_Image_t* dsvi_image_labeledRegionAsImage(const dsvi_Image_t* srcImage,uint8_t channel,dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    dsvi_Image_t* newImage = NULL;
    if(_status==DSVI_STATUS_OK)
    {
        if((srcImage==NULL)||(status==NULL))
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if(srcImage->label==NULL)
            {
                _status = DSVI_STATUS_INVALID_OPERATION;
            }else
            {
                if(srcImage->format!=enDsviImagePixelFormat_DEFAULT)
                {
                    _status = DSVI_STATUS_NOT_SUPPORTED;
                }else
                {
                    ImageInfo_t newInfo = {0};
                    newInfo.height = srcImage->height;
                    newInfo.width  = srcImage->width;
                    newInfo.format = enDsviImagePixelFormat_RGB888;

                    newImage = dsvi_image_new(&newInfo,&_status);

                    if(_status==DSVI_STATUS_OK)
                    {
                        channel = channel%4;

                        uint16_t row_i,col_i;
                        uint32_t src_i,dest_i;
                        dsvi_Label_t labelVal;

                        for(row_i=0;row_i<srcImage->height;row_i++)
                        {
                            src_i = (row_i*srcImage->width);
                            dest_i = src_i*3;
                            for(col_i=0;col_i<srcImage->width;col_i++)
                            {
                                if(srcImage->label[src_i]==0)
                                {
                                    newImage->data[dest_i+0] = 0;
                                    newImage->data[dest_i+1] = 0;
                                    newImage->data[dest_i+2] = 0;
                                }else
                                {
                                    //
                                    labelVal = (((srcImage->label[src_i]%11)*15)+100);
                                    //

                                    switch(channel)
                                    {
                                    case 0:
                                        newImage->data[dest_i+0] = labelVal;
                                        newImage->data[dest_i+1] = 0;
                                        newImage->data[dest_i+2] = 0;
                                        break;
                                    case 1:
                                        newImage->data[dest_i+0] = 0;
                                        newImage->data[dest_i+1] = labelVal;
                                        newImage->data[dest_i+2] = 0;
                                        break;
                                    case 2:
                                        newImage->data[dest_i+0] = 0;
                                        newImage->data[dest_i+1] = 0;
                                        newImage->data[dest_i+2] = labelVal;
                                        break;
                                    default:
                                        newImage->data[dest_i+0] = labelVal;
                                        newImage->data[dest_i+1] = labelVal;
                                        newImage->data[dest_i+2] = labelVal;
                                        break;
                                    }
                                }
                                src_i++;
                                dest_i+=3;
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

#undef Equivalency_List_Buffer_Size
#undef label2ColorImg_maxTh
#undef label2ColorImg_minTh
#undef checkPixForLabel
#undef pixCol
#undef pixRow
