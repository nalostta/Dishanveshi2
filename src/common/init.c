#include <dishanveshi/init.h>

dsvi_status_t dsvi_test_init()
{
    //config image init functions
    DsviImageMemoryConfig_t image_allocator;
    image_allocator.image_alloc =   &malloc;
    image_allocator.image_free  =   &free;
    dsvi_status_t  _status      =   dsvi_image_Init(&image_allocator);
    return _status;
}
