#ifndef DSVI_MISC_FNS_
#define DSVI_MISC_FNS_

#include <dishanveshi/types.h>
#include <stdio.h>

#define checkStatus(M, ...) if(_status==DSVI_STATUS_OK){printf( M ": OK\n",##__VA_ARGS__);}else{printf( M ": Error : %d\n",##__VA_ARGS__,_status);}

#ifndef DSVI_STR_BUFFER_SIZE
#error "please check DSVI_STR_BUFFER_SIZE"
#endif

struct dsvi_path_t
{
    char* el;
    uint32_t len;
    struct dsvi_path_t* next;
};

typedef struct dsvi_path_t dsvi_Path_t;

unsigned int Str2Int(const char* argv);
unsigned int dsvi_bufferRead_LE(const char* argv,unsigned int NumBytes);
void dsvi_bufferWrite_LE(char* dest,int srcNum,unsigned int numBytes);

//pixel operations
real_t dsvi_Image_getGrey(uint8_t* pix,uint8_t bitsPerPixel,real_t weightR,real_t weightG,real_t weightB,dsvi_status_t* status);


//path module
uint32_t dsvi_path_new(dsvi_Path_t** destPath,const char* srcStr,char token, dsvi_status_t* status); //2
dsvi_status_t dsvi_path_delete(dsvi_Path_t** srcPath);                                             //2

dsvi_status_t   dsvi_path_addDir(dsvi_Path_t** basePath,const char* dirName);                        //1
dsvi_status_t   dsvi_path_addDir_ex(dsvi_Path_t** basePath,const char* dirName,uint32_t dirLen);     //2
dsvi_status_t   dsvi_path_addbackSlash(dsvi_Path_t** basePath);                                      //1
dsvi_status_t   dsvi_path_addfrontSlash(dsvi_Path_t** basePath);                                     //2
dsvi_status_t   dsvi_path_addExtension(dsvi_Path_t** basePath,const char* ext);                      //1
dsvi_status_t   dsvi_path_removeLast(dsvi_Path_t** srcPath);                                         //2
dsvi_status_t   dsvi_path_gotoLastDirectory(dsvi_Path_t** srcPath);
dsvi_status_t   dsvi_path_printPath(const dsvi_Path_t* srcPath); //2

dsvi_Path_t*    dsvi_path_new_byJoin(dsvi_Path_t* basePath,dsvi_Path_t* relPath,dsvi_status_t* status); //2
dsvi_Path_t*    dsvi_path_getFullPath(dsvi_Path_t* basePath,dsvi_Path_t* relPath,dsvi_Path_t* fileName,dsvi_status_t* status); //
dsvi_Path_t*    dsvi_path_clone(dsvi_Path_t* basePath,dsvi_status_t* status);                           //2

char* dsvi_path_getStr(const dsvi_Path_t* srcPath,dsvi_status_t* status);                            //2

#endif // _MISC_FNS_

