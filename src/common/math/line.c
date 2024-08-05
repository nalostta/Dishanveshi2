#include <dishanveshi/math/line.h>
#include <math.h>

#define XOFH(l)  l->head.x
#define XOFT(l)  l->tail.x
#define YOFH(l)  l->head.y
#define YOFT(l)  l->tail.y

EnBoolean_t isLine_Valid(dsvi_LineR_t* line)
{
    if(isPt_Valid(&line->head)&&isPt_Valid(&line->tail))
    {
        return enBoolean_True;
    }else
    {
        return enBoolean_False;
    }
}

real_t dsvi_math_getSlope( dsvi_Line_t* line,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    real_t slope = 0;

    if(line==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        if(isLine_MT(line))
        {
            _status =   DSVI_STATUS_INVALID_OPERATION;
        }else
        {
            slope   =   ((line->head.y-line->tail.y)/(line->head.x-line->tail.x));
        }
    }

    if(status!=NULL)
    {
        (*status)=_status;
    }

    if(_status==DSVI_STATUS_OK)
    {
        return slope;
    }else
    {
        return 0.0/0.0; //to be reviewed
    }
}

real_t dsvi_math_getSlopeR( dsvi_LineR_t* line,dsvi_status_t* status)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    real_t slope = 0;

    if(line==NULL)
    {
        _status =   DSVI_STATUS_NULL_ARG;
    }else
    {
        if(!(isLine_Valid(line)))
        {
            _status =   DSVI_STATUS_INVALID_ARG;
        }else
        {
            if(isLine_MT(line))
            {
                _status =   DSVI_STATUS_INVALID_OPERATION;
            }else
            {
                slope   =   ((line->head.y-line->tail.y)/(line->head.x-line->tail.x));
            }
        }
    }

    if(status!=NULL)
    {
        (*status)=_status;
    }

    if(_status==DSVI_STATUS_OK)
    {
        return slope;
    }else
    {
        return 0.0/0.0; //to be reviewed
    }
}

EnBoolean_t dsvi_math_checkXBoundaryOverlap( dsvi_LineR_t* line1, dsvi_LineR_t* line2,dsvi_status_t* status)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;
    EnBoolean_t boolOverlap     =   enBoolean_True;

    if(!(isLine_Valid(line1)&&isLine_Valid(line2))||(isLine_MT(line1)&&isLine_MT(line2)))
    {
        _status =   DSVI_STATUS_INVALID_ARG;
    }else
    {
        real_t leftBound    =   MinOf2(line1->head.x,line1->tail.x);
        real_t rightBound   =   MaxOf2(line1->head.x,line1->tail.x);

        if((line2->head.x>rightBound&&line2->tail.x>rightBound)||(line2->head.x<leftBound&&line2->tail.x<leftBound))
        {
            boolOverlap = enBoolean_False;
        }
    }
    if(status!=NULL)
    {
        (*status)   =   _status;
    }
    return boolOverlap;
}

real_t dsvi_math_getYIntersect(dsvi_LineR_t* line,dsvi_status_t* status)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;

    if(!isLine_Valid(line)||isLine_MT(line)) //also chec kthe slope!!!!!!!!
    {
        _status =    DSVI_STATUS_INVALID_ARG;
    }else
    {
        _status =   DSVI_STATUS_OK;
    }

    if(status!=NULL)
    {
        (*status)   =   _status;
    }
    return 0.0;
}

dsvi_status_t dsvi_math_lineSegment2D_intersects( dsvi_LineR_t* l1,  dsvi_LineR_t* l2,EnBoolean_t* result)
{
    dsvi_status_t _status   =   DSVI_STATUS_OK;

    if(isLine_Valid(l1)&&isLine_Valid(l2)&&!isLine_MT(l1)&&!isLine_MT(l2))
    {
        if((MaxOf2(l1->head.x,l1->tail.x)<MinOf2(l2->head.x,l2->tail.x))||(MinOf2(l1->head.x,l1->tail.x)>MaxOf2(l2->head.x,l2->tail.x))||(MaxOf2(l1->head.y,l1->tail.y)<MinOf2(l2->head.y,l2->tail.y))||(MinOf2(l1->head.y,l1->tail.y)>MaxOf2(l2->head.y,l2->tail.y)))
        {
            //8 cases eliminated
            (*result)   =   enBoolean_False;
        }else
        {
            //get cross pdt
            //crossPdt   =   (((p1->x-refPt->x)*(p2->y-refPt->y))-((p1->y-refPt->y)*(p2->x-refPt->x)));
            real_t crossprod2   =   (l1->head.x-l1->tail.x);
            real_t crossprod1   =   (crossprod2*(l2->head.y-l1->tail.y))-((l1->head.y-l1->tail.y)*(l2->head.x-l1->tail.x));
            crossprod2          =   (crossprod2*(l2->tail.y-l1->tail.y))-((l1->head.y-l1->tail.y)*(l2->tail.x-l1->tail.x));
            //4m + 7s

            if(crossprod1*crossprod2>0.0)
            {
                (*result)   =   enBoolean_False;
            }else
            {
                (*result)   =   enBoolean_True;
            }
        }
    }else
    {
        _status =   DSVI_STATUS_INVALID_ARG;
    }
    return _status;
}


dsvi_status_t dsvi_math_lineSegment2D_intersect(dsvi_LineR_t* l1,dsvi_LineR_t* l2,dsvi_PointR_t* result)
{
    dsvi_status_t _status = DSVI_STATUS_OK;
    dsvi_PointR_t intscn  = {0};

    if(isLine_Valid(l1)&isLine_Valid(l2))
    {
        real_t t    =   (XOFH(l1)*(YOFT(l2)-YOFH(l2))+XOFT(l2)*(YOFH(l2)-YOFH(l1))+XOFH(l2)*(YOFH(l1)-YOFT(l2))); //3m + 3s
        real_t t2   =   (((XOFT(l1)-XOFH(l1))*(YOFH(l2)-YOFT(l2)))-((XOFH(l2)-XOFT(l2))*(YOFT(l1)-YOFH(l1)))); //2m + 5s

        t   =   t/t2;  //1 d

        //total cost = 5m + 8s + 1d

        intscn.x    =   XOFH(l1)+t*(XOFT(l1)-XOFH(l1));
        intscn.y    =   YOFH(l1)+t*(YOFT(l1)-YOFH(l1));

        //cumulative total cost = 7m + 12s + 1d
    }else
    {
        _status =   DSVI_STATUS_INVALID_ARG;
    }

    (*result) = intscn;

    return _status;
}

#undef XOFH
#undef XOFT
#undef yOFH
#undef yOFT

