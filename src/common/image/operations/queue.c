#include <dishanveshi/image/operations/queue.h>

dsvi_status_t dsvi_imageOp_qPush(dsvi_Queue_t** headRef,uint32_t data)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;

    if(_status == DSVI_STATUS_OK)
    {
        if(headRef == NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            dsvi_Queue_t* temp = (*_DsviMemHandle.image_alloc)(sizeof(dsvi_Queue_t));

            if(temp==NULL)
            {
                _status =   DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
            }else
            {
                temp->data  =   data;
                temp->next  =   (*headRef);
                (*headRef)  =   temp;
            }
        }
    }

     return _status;
}

uint32_t dsvi_imageOp_qPop(dsvi_Queue_t** headRef,dsvi_status_t* status)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    uint32_t tempData     = 0;

    if(_status == DSVI_STATUS_OK)
    {
        if(headRef==NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            if((*headRef)==NULL)
            {
                _status = DSVI_STATUS_INVALID_OPERATION;
            }else
            {
                tempData = (*headRef)->data;
                dsvi_Queue_t* temp = (*headRef);
                (*headRef)  =   temp->next;
                (*_DsviMemHandle.image_free)(temp);
            }
        }
    }
    return tempData;
}

dsvi_status_t dsvi_imageOp_qSortedInsert(dsvi_Queue_t** headRef,uint32_t data)
{
    dsvi_status_t _status = dsvi_image_getMemConfig();
    extern DsviImageMemoryConfig_t _DsviMemHandle;
    dsvi_Queue_t* temp = NULL;

    if(_status == DSVI_STATUS_OK)
    {
        if(headRef == NULL)
        {
            _status = DSVI_STATUS_NULL_ARG;
        }else
        {
            temp = (*_DsviMemHandle.image_alloc)(sizeof*temp);

            if(temp == NULL)
            {
                _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
            }else
            {
                temp->data = data;
                if((*headRef)==NULL)
                {
                    temp->next = NULL;
                    (*headRef) = temp;
                }else
                {
                    //build sorted queue
                    dsvi_imageOp_qPush(headRef,-999999);
                    dsvi_Queue_t* parser = (*headRef);
                    while(parser->data<=temp->data)
                    {
                        parser = parser->next;
                        if(parser==NULL)
                        {
                            break;
                        }
                    }
                    temp->next = parser;
                    parser = (*headRef);

                    while(parser->next!=temp->next)
                    {
                        parser=parser->next;
                    }
                    parser->next = temp;
                    dsvi_imageOp_qPop(headRef,&_status);

                    //
                }
            }
        }
    }
    return _status;
}

