#include <dishanveshi/math/point.h>
#include <math.h>

const real_t thresh_Infinity =  1e+20;

EnBoolean_t isPt_Valid(const dsvi_PointR_t* pt)
{
    if(!(isnan(pt->x)||isnan(pt->y))&&ABSF(pt->x)<thresh_Infinity&&ABSF(pt->y)<thresh_Infinity)
    {
        return enBoolean_True;
    }else
    {
        return enBoolean_False;
    }
}

dsvi_status_t dsvi_math_3ptCrossProduct(const dsvi_PointR_t* refPt,const dsvi_PointR_t* p1,const dsvi_PointR_t* p2,real_t* result)
{
    //check pt and line validity
    //get 3 points
    //do operation
    //return result
    dsvi_status_t _status   =   DSVI_STATUS_OK;
    real_t crossPdt =   0.0/0.0;  //initially erroneous value -> nan

    if(isPt_Valid(refPt)&&isPt_Valid(p1)&&isPt_Valid(p2))
    {
         crossPdt   =   (((p1->x-refPt->x)*(p2->y-refPt->y))-((p1->y-refPt->y)*(p2->x-refPt->x)));
         (*result)  =   crossPdt;
    }else
    {
        _status   =   DSVI_STATUS_INVALID_ARG;
    }
    return _status;
}
