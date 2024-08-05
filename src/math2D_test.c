//config
#define DSVI_CONFIG_PUBLIC_IMAGE
#define USE_MICRO_EPSILON

#include <stdio.h>
#include <dishanveshi/math/line.h>

int main()
{

    dsvi_status_t _status   =  DSVI_STATUS_OK;
    dsvi_LineR_t line1  = {0};
    dsvi_LineR_t line2  = {0};

    line1.head.x = 1;
    line1.head.y = 2;
    line1.tail.x = 4;
    line1.tail.y = 8;

    line2.head.x = 0;
    line2.head.y = 6;
    line2.tail.x = 6;
    line2.tail.y = 0;

    dsvi_PointR_t intsc =   {0};

    EnBoolean_t result=0;;
    _status = dsvi_math_lineSegment2D_intersects(&line1,&line2,&result);
    if(_status==DSVI_STATUS_OK&&result)
    {
         _status =   dsvi_math_lineSegment2D_intersect(&line1,&line2,&intsc);
        if(_status==DSVI_STATUS_OK)
        {
            printf("intersection found : (%f,%f)\n",intsc.x,intsc.y);
        }
    }


    printf("intersects : %d\n",result);
    printf("status     : %d",_status);
    return 0;
}

