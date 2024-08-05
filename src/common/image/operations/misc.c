#include <dishanveshi/image/operations/misc.h>

dsvi_status_t dsvi_printMatrix(dsvi_Matrix_t* matrix)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if(matrix==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        uint16_t width  =   matrix->width;
        uint16_t height =   matrix->height;
        uint32_t rowIndex=0,colIndex,index=0;

        for(rowIndex=0;rowIndex<height;rowIndex++)
        {
            for(colIndex=0;colIndex<width;colIndex++)
            {
                printf("\t%d",matrix->el[index++]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return _status;
}

dsvi_status_t dsvi_printKernel(dsvi_Kernel_t* kernel)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    if(kernel==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        uint16_t width  =   kernel->width;
        uint16_t height =   kernel->height;
        uint32_t rowIndex=0,colIndex,index=0;

        for(rowIndex=0;rowIndex<height;rowIndex++)
        {
            for(colIndex=0;colIndex<width;colIndex++)
            {
                printf("\t%d",kernel->el[index++]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return _status;
}
