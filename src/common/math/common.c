#include <dishanveshi/math/common.h>

int areEqual(float a, float b)
{
    return (ABSF((a-b)) < dsvi_epsilonF);
}
