#include "misc.h"
#include <stdlib.h>
#include <string.h>

unsigned int Str2Int(const char* argv)
{
    unsigned int char_index=0,acc=0;
    while((argv[char_index]!=0) && (char_index<DSVI_STR_BUFFER_SIZE))
    {
        acc*=10;
        acc+=(argv[char_index++]-48);
    }
    return acc;
}

unsigned int dsvi_bufferRead_LE(const char* argv,unsigned int NumBytes)
{
    int char_index=0;
    int acc=0,shift=0;
    while((char_index<DSVI_STR_BUFFER_SIZE) && (char_index<NumBytes))
    {
        acc  += ((argv[char_index++])<<shift)&(0xff<<shift);
        shift+=8;
    }
    return acc;
}

void dsvi_bufferWrite_LE(char* dest,int srcNum,unsigned int numBytes)
{
    int index;
    for(index=0;index<numBytes;index++)
    {
        dest[index]=(char)srcNum;
        srcNum=(srcNum>>8);
    }
    return;
}

real_t dsvi_Image_getGrey(uint8_t* pix,uint8_t bitsPerPixel,real_t weightR,real_t weightG,real_t weightB,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    real_t grayVal=0;
    if(pix==NULL)
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        if(bitsPerPixel==24) //RGB565
        {
            grayVal =   pix[0]*weightB+pix[1]*weightG+pix[2]*weightR;
        }else
        if(bitsPerPixel==16) //RGB565
        {
            uint8_t valR = (pix[0]>>3);
            uint8_t valG = ((pix[0]&0x07)<<2)|(pix[1]>>6);
            uint8_t valB = (pix[1]&0x1f);

            grayVal      =  (valR*weightR + valG*weightG + valB*weightB);
        }else
        {
            _status      =  DSVI_STATUS_NOT_SUPPORTED;
        }
    }
    if(status!=NULL)
    {
        (*status) = _status;
    }
    if(_status==DSVI_STATUS_OK)
    {
        return grayVal;
    }else
    {
        return 0;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------
uint32_t dsvi_path_new(dsvi_Path_t** destPath,const char* srcStr,char token, dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    uint32_t n = 0;

    if((destPath==NULL)||(srcStr==NULL))
    {
        _status =  DSVI_STATUS_NULL_ARG;
    }else
    {
        dsvi_Path_t* trav = (*destPath);
        if(trav!=NULL)
        {
            dsvi_path_delete(&trav);
        }
        trav=NULL;
        //
        uint32_t src_i=0,buf_i=0;
        uint32_t srcStrLen = strlen(srcStr);
        char buffer[30];

        do
        {
            if((srcStr[src_i]==token)||(src_i==srcStrLen))
            {
                _status = dsvi_path_addDir_ex(destPath,(const char*)buffer,buf_i); //check for buf_i==0
                if(_status==DSVI_STATUS_OK)
                {
                    _status = dsvi_path_addfrontSlash(destPath);
                }

                int i;
                for(i=0;i<=buf_i;i++)
                {
                    buffer[i]=0;
                }

                buf_i=0;
                src_i++;
                n++;
            }else
            {
                buffer[buf_i] = srcStr[src_i];
                buf_i++;
                src_i++;

                if(buf_i>30)
                {
                    _status = DSVI_STATUS_ERROR;
                    break;
                }
            }
        }while(src_i<=srcStrLen);
    }
    if(status!=NULL)
    {
        (*status) = _status;
    }
    return n;
}

dsvi_status_t dsvi_path_delete(dsvi_Path_t** srcPath)
{
    if(srcPath==NULL)
    {
        return DSVI_STATUS_NULL_ARG;
    }else
    {
        dsvi_Path_t* trav = *srcPath;
        dsvi_Path_t* prev = NULL;
        while(trav!=NULL)
        {
            free(trav->el);
            prev = trav;
            trav = trav->next;
            free(prev);
        }
        (*srcPath) = NULL;
    }
    return DSVI_STATUS_OK;
}

dsvi_status_t dsvi_path_addDir(dsvi_Path_t** basePath,const char* dirName)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if(dirName!=NULL)
    {
        uint32_t len = strlen(dirName);
        _status = dsvi_path_addDir_ex(basePath,dirName,len);
    }
    return _status;
}

dsvi_status_t dsvi_path_addDir_ex(dsvi_Path_t** basePath,const char* dirName,uint32_t dirLen)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if((basePath==NULL)||(dirName==NULL))
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        if(dirLen<1)
        {
            _status = DSVI_STATUS_NOT_IMPLEMENTED;
        }else
        {
            dsvi_Path_t* trav = (*basePath);

            if((*basePath)==NULL)
            {
                //start allocating
                trav = calloc(1,sizeof*trav);
                if(trav==NULL)
                {
                    _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                }else
                {
                    (*basePath) = trav;
                }
            }else
            {
                while(trav->next!=NULL)
                {
                    trav=trav->next;
                }
                trav->next = calloc(1,sizeof*trav);
                trav = trav->next;
            }

            if(trav==NULL)
            {
                _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
            }else
            {
                trav->el  = calloc(dirLen+1,sizeof*trav->el);
                if(trav->el==NULL)
                {
                    dsvi_status_t _status = dsvi_path_removeLast(basePath);
                    if(_status==DSVI_STATUS_OK)
                    {
                        _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
                    }else
                    {
                        _status = DSVI_STATUS_ERROR;
                    }
                }else
                {
                    trav->len = dirLen;
                    memcpy(trav->el,dirName,dirLen);
                    trav->el[dirLen] = '\0';
                }
            }
        }
    }
    return _status;
}

dsvi_Path_t* dsvi_path_new_byJoin(dsvi_Path_t* basePath,dsvi_Path_t* relPath,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    dsvi_Path_t* newPath = NULL;

    if((basePath == NULL)&&(relPath!=NULL))
    {
        //return a clone of relPath
        newPath = dsvi_path_clone(relPath,&_status);
    }else
    if((basePath!=NULL)&&(relPath==NULL))
    {
        newPath = dsvi_path_clone(basePath,&_status);
    }else
    {
        newPath = dsvi_path_clone(basePath,&_status);
        if(_status!=DSVI_STATUS_OK)
        {
            dsvi_path_delete(&newPath);
        }else
        {
            dsvi_Path_t* relTemp = dsvi_path_clone(relPath,&_status);
            if(_status == DSVI_STATUS_OK)
            {
                dsvi_Path_t* trav = newPath;
                while(trav->next!=NULL)
                {
                    trav=trav->next;
                }
                trav->next = relTemp;
            }
        }
    }

    if(status!=NULL)
    {
        (*status) = _status;
    }
    return newPath;
}

dsvi_Path_t* dsvi_path_clone(dsvi_Path_t* basePath,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    dsvi_Path_t* clone    = NULL;
    if(basePath==NULL)
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        dsvi_Path_t* srcTrav  = basePath;
        do
        {
            dsvi_path_addDir_ex(&clone,srcTrav->el,srcTrav->len);
            srcTrav  = srcTrav->next;

        }while(srcTrav!=NULL);
    }
    if(status!=NULL)
    {
        (*status) = _status;
    }
    return clone;
}


dsvi_status_t dsvi_path_removeNth(dsvi_Path_t* srcPath,uint32_t index)
{
    return DSVI_STATUS_NOT_IMPLEMENTED;
}

dsvi_status_t dsvi_path_addbackSlash(dsvi_Path_t** basePath)
{
    dsvi_status_t _status = dsvi_path_addDir_ex(basePath,"\\",1);
    return _status;
}

dsvi_status_t dsvi_path_addfrontSlash(dsvi_Path_t** basePath)
{
    dsvi_status_t _status = dsvi_path_addDir_ex(basePath,"/",1);
    return _status;
}

dsvi_status_t dsvi_path_removeLast(dsvi_Path_t** srcPath)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if(srcPath==NULL)
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        dsvi_Path_t* trav = (*srcPath);
        if(trav==NULL)
        {
            _status = DSVI_STATUS_INVALID_OPERATION;
        }else
        {
            if(trav->next==NULL)
            {
                free(trav->el);
                trav->len=0;
                free(trav);
                (*srcPath) = NULL;
            }else
            {
                while(trav->next!=NULL)
                {
                    trav=trav->next;
                }
                dsvi_Path_t* prev = (*srcPath);
                while(prev->next!=trav)
                {
                    prev=prev->next;
                }
                free(trav->el);
                free(trav);
                prev->next=NULL;
            }
        }
    }
    return _status;
}

dsvi_status_t dsvi_path_printPath(const dsvi_Path_t* srcPath)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if(srcPath==NULL)
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        const dsvi_Path_t* trav = srcPath;

        fprintf(stdout,"\n[fn]path = ");
        while(trav!=NULL)
        {
            fprintf(stdout,"%s",trav->el);
            trav=trav->next;
        }
        fprintf(stdout,"\n");
    }
    return _status;
}

dsvi_status_t dsvi_path_addExtension(dsvi_Path_t** basePath,const char* ext)
{
    dsvi_status_t _status = DSVI_STATUS_OK;

    if((basePath==NULL)||(ext==NULL))
    {
        _status = DSVI_STATUS_OK;
    }else
    {
        if(*basePath==NULL)
        {
            _status = DSVI_STATUS_INVALID_OPERATION;
        }else
        {
            int len = strlen(ext)+1;
            char* temp = calloc((len+1),sizeof*temp);
            temp[0] = '#';
            memcpy(&temp[1],ext,(len));
            _status = dsvi_path_addDir_ex(basePath,temp,len);
        }
    }
    return _status;
}

char* dsvi_path_getStr(const dsvi_Path_t* srcPath,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    char* string = NULL;

    if(srcPath==NULL)
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        const dsvi_Path_t* trav = srcPath;
        uint32_t len = 0;

        while(trav!=NULL)
        {
            len += (trav->len+1);
            if(trav->el[0]=='#')
            {
                len--;
            }
            trav = trav->next;
        }
        string = calloc(len+1,sizeof*string);

        len = 0;
        if(string==NULL)
        {
            _status = DSVI_STATUS_MEMORY_ALLOCATION_FAILED;
        }else
        {
            trav = srcPath;
            while(trav!=NULL)
            {

                if(trav->el[0]=='#')
                {
                    memcpy(&string[len],&trav->el[1],(trav->len-1));
                    len+=trav->len-1;
                }else
                {
                    memcpy(&string[len],trav->el,trav->len);
                    len+=trav->len;
                }
                //
                trav = trav->next;
            }
        }
    }
    if(status!=NULL)
    {
        (*status) = _status;
    }

    return string;
}

dsvi_Path_t* dsvi_path_getFullPath(dsvi_Path_t* basePath,dsvi_Path_t* relPath,dsvi_Path_t* fileName,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    dsvi_Path_t* newPath = NULL;
    dsvi_Path_t* trav     = NULL;

    if((basePath==NULL)&&(relPath!=NULL))
    {
        newPath = dsvi_path_clone(relPath,&_status);
    }else
    if((basePath!=NULL)&&(relPath==NULL))
    {
        newPath = dsvi_path_clone(basePath,&_status);
    }else
    if((basePath!=NULL)&&(relPath!=NULL))
    {
        newPath = dsvi_path_clone(basePath,&_status);
        if(_status==DSVI_STATUS_OK)
        {
            trav = newPath;
            while(trav->next!=NULL)
            {
                trav = trav->next;
            }
            trav->next = dsvi_path_clone(relPath,&_status);
        }
    }

    dsvi_path_printPath(newPath);

    if(_status==DSVI_STATUS_OK)
    {
        if(newPath!=NULL)
        {
            if(trav->next!=NULL)
            {
                while(trav->next!=NULL)
                {
                    trav=trav->next;
                }
            }
            trav->next = dsvi_path_clone(fileName,&_status);
        }else
        {
            newPath = dsvi_path_clone(fileName,&_status);
        }
    }

    if(status!=NULL)
    {
        (*status) = _status;
    }
    return newPath;
}

dsvi_status_t dsvi_path_gotoLastDirectory(dsvi_Path_t** srcPath)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if(srcPath==NULL)
    {
        _status = DSVI_STATUS_NULL_ARG;
    }else
    {
        dsvi_Path_t* trav = (*srcPath);
        if(trav==NULL)
        {
            _status = DSVI_STATUS_INVALID_OPERATION;
        }else
        {
            if(trav->next==NULL)
            {
                if((trav->el[0]!='/')&&(trav->len!=1))
                {
                    dsvi_path_delete(&trav);
                    (*srcPath)=NULL;
                }
            }else
            {
                dsvi_Path_t* prev = NULL;
                while((*srcPath)!=NULL)
                {
                    trav = (*srcPath);
                    prev = (*srcPath);
                    while(trav->next!=NULL)
                    {
                        trav=trav->next;
                    }
                    while(prev->next!=trav)
                    {
                        prev=prev->next;
                    }
                    if(trav->el[0]=='/'&&trav->len==1)
                    {
                        break;
                    }else
                    {
                        dsvi_path_delete(&trav);
                        prev->next = NULL;
                    }
                }
            }
        }
    }
    return _status;
}
