#define DSVI_CONFIG_PUBLIC_IMAGE
#define DSVI_CONFIG_PUBLIC_HISTOGRAM

#include <dishanveshi/init.h>
#include <dishanveshi/image/image.h>
#include <dishanveshi/image/operations/histogram.h>
#include <dishanveshi/image/operations/queue.h>
#include <dishanveshi/image/operations/template.h>
#include <dishanveshi/image/operations/convolve.h>
#include <dishanveshi/image/operations/filter.h>
#include <bitmap.h>
#include <string.h>
#include <misc.h>

#define STRING_BUFFER_SIZE 100
#define IMAGE_NOISE_THRESHOLD 800
#define CUTOUT_THRESHOLD 10

/*
 *  The aim here is to process all the test images in the folder "bin/resources/test images" (except ../_old test images/)
 *  to get the gray-scale , histograms and labeled regions of all the test images in the sub-folders
 *  sub-folders :
 *      - CCF&W
 *      - multi_interface
 *      - reflective_floor
 *      - textured_floor
 *      - std_images
 */

dsvi_status_t processImage(char* filepath,char* filename,dsvi_histogramInfo_t* hInfo);
void process_test_fn();


int main()
{
    dsvi_status_t _status = dsvi_test_init();

    /*dsvi_histogramInfo_t hInfo = {0};
    uint8_t a[255];
    int j;
    for(j=0;j<255;j++) a[j]=j+1;
    hInfo.bins    =  255;
    hInfo.bounds  =  &a[0];
    */

    process_test_fn();
    return _status;
}

void process_test_fn()
{
    dsvi_status_t _status = DSVI_STATUS_OK;

    //----------common histogram variables-----------

    dsvi_histogramInfo_t hInfo = {0};

    uint8_t a[255];
    int j;
    for(j=0;j<255;j++) a[j]=j+1;

    hInfo.bins    =  255;
    hInfo.bounds  =  &a[0];
    //----------common histogram variables-----------

    _status = processImage("CCF&W/op/","c1",&hInfo);
    checkStatus("c1");
    printf("\n\n");
    _status = processImage("CCF&W/op/","c2",&hInfo);
    checkStatus("c2");
    printf("\n\n");
    _status = processImage("multi_interface/op/","m1",&hInfo);
    checkStatus("m1");
    printf("\n\n");
    _status = processImage("multi_interface/op/","m2",&hInfo);
    checkStatus("m2");
    printf("\n\n");
    //_status = processImage("multi_interface/op/","m3",&hInfo);
    //checkStatus("m3");
    printf("\n\n");
    _status = processImage("multi_interface/op/","m4",&hInfo);
    checkStatus("m4");
    printf("\n\n");
    _status = processImage("multi_interface/op/","m5",&hInfo);
    checkStatus("m5");
    printf("\n\n");

    _status = processImage("reflective_floor/op/","r1",&hInfo);
    checkStatus("r1");
    printf("\n\n");
    _status = processImage("reflective_floor/op/","r2",&hInfo);
    checkStatus("r2");
    printf("\n\n");
    _status = processImage("reflective_floor/op/","r3",&hInfo);
    checkStatus("r3");
    printf("\n\n");
    _status = processImage("reflective_floor/op/","r4",&hInfo);
    checkStatus("r4");
    printf("\n\n");
    _status = processImage("reflective_floor/op/","r5",&hInfo);
    checkStatus("r5");
    printf("\n\n");

    _status = processImage("std_images/op/","s1",&hInfo);
    checkStatus("s1");
    printf("\n\n");
    _status = processImage("std_images/op/","s2",&hInfo);
    checkStatus("s2");
    printf("\n\n");
    _status = processImage("std_images/op/","s3",&hInfo);
    checkStatus("s3");
    printf("\n\n");
    _status = processImage("std_images/op/","s4",&hInfo);
    checkStatus("s4");
    printf("\n\n");
    _status = processImage("std_images/op/","s5",&hInfo);
    checkStatus("s5");
    printf("\n\n");
    _status = processImage("std_images/op/","s6",&hInfo);
    checkStatus("s6");
    printf("\n\n");
    _status = processImage("std_images/op/","s7",&hInfo);
    checkStatus("s7");
    printf("\n\n");
    _status = processImage("std_images/op/","s8",&hInfo);
    checkStatus("s8");
    printf("\n\n");


    _status = processImage("textured_floor/op/","t1",&hInfo);
    checkStatus("t1");
    printf("\n\n");
    _status = processImage("textured_floor/op/","t2",&hInfo);
    checkStatus("t2");
    printf("\n\n");
    _status = processImage("textured_floor/op/","t3",&hInfo);
    checkStatus("t3");
    printf("\n\n");
    _status = processImage("textured_floor/op/","t4",&hInfo);
    checkStatus("t4");
    printf("\n\n");
    _status = processImage("textured_floor/op/","t5",&hInfo);
    checkStatus("t5");
    printf("\n\n");
    _status = processImage("textured_floor/op/","t6",&hInfo);
    checkStatus("t6");
    printf("\n\n");
    //_status = processImage("textured_floor/op/","t7",&hInfo);
    //checkStatus("t7");
    //printf("\n\n");
    return;
}

dsvi_status_t processImage(char* filepath,char* filename,dsvi_histogramInfo_t* hInfo)
{


    //init
    dsvi_status_t _status = DSVI_STATUS_OK;
    char srcfilestr[STRING_BUFFER_SIZE] = "bin/Resources/test_images/";
    strcat(srcfilestr,filepath);
    strcat(srcfilestr,filename);
    strcat(srcfilestr,".bmp");

    //basic op - read, convert to gray, write gray,clean-up
    dsvi_Image_t* srcImage = dsvi_image_bitmap_read(srcfilestr,&_status);
    checkStatus("%s\treading %s\%s.bmp",filename,filepath,filename);
    dsvi_Image_t* defImage =   dsvi_image_convertToDefault(srcImage,&_status);
    checkStatus("%s\tconverting Image to default format",filename);
    _status     =   dsvi_image_delete(srcImage);
    checkStatus("%s\tdeleting srcImage",filename);


    {
        dsvi_Image_t* defImage888    =   dsvi_image_convertTo888(defImage,&_status);
        checkStatus("%s\tconverting gray-scale def to 888",filename);

        char grayscalestr[STRING_BUFFER_SIZE]   = "bin/Resources/test_images/";
        strcat(grayscalestr,filepath);
        strcat(grayscalestr,filename);
        strcat(grayscalestr,"_grayscale.bmp");
        _status = dsvi_image_bitmap_write(defImage888,grayscalestr);
        checkStatus("%s\twriting gray-scale image",filename);

         _status     = dsvi_image_delete(defImage888);
        checkStatus("%s\tdeleting gray888",filename);
    }

    //get Histogram
    dsvi_histogram_t* histogram = dsvi_image_getHistogram(defImage,hInfo,&_status);
    checkStatus("%s\tget histogram",filename);

    {
        char histogramstr[STRING_BUFFER_SIZE]   = "bin/Resources/test_images/";
        strcat(histogramstr,filepath);
        strcat(histogramstr,filename);
        strcat(histogramstr,"_histogram.txt");
        FILE* histogramHandle = fopen(histogramstr,"w");

        if((histogramHandle!=NULL)&&(_status==DSVI_STATUS_OK))
        {
            int j=0;
            fprintf(histogramHandle,"file = %s\n",histogramstr);
            for(j=0;j<hInfo->bins;j++)
            {
                if(histogram->data[j]!=0)
                {
                    fprintf(histogramHandle,"%d:%d\n",j,histogram->data[j]);
                }
            }
            _status = fclose(histogramHandle);
            checkStatus("%s\tclosing histogram txt file",filename);
        }else
        {
            printf("\n%s\terror : writing histogram in txt file",filename);
        }
    }

    dsvi_Image_t* histImage  =   dsvi_image_histogramToImageFixedHeight(histogram,&_status);
    checkStatus("%s\thistogram to image",filename);

    {
        dsvi_Image_t* histImage888 = dsvi_image_convertTo888(histImage,&_status);
        checkStatus("%s\thistImage convert to 888",filename);

        char histimagestr[STRING_BUFFER_SIZE]   = "bin/Resources/test_images/";
        strcat(histimagestr,filepath);
        strcat(histimagestr,filename);
        strcat(histimagestr,"_histogram.bmp");
        _status = dsvi_image_bitmap_write(histImage888,histimagestr);
        checkStatus("%s\twriting histogram image",filename);

        _status = dsvi_image_delete(histImage888);
        checkStatus("%s\tdeleting histImage 888 format",filename);
    }



    //get noise values from the image histogram
    int lower=1,upper=254;

    while(histogram->data[lower++]<IMAGE_NOISE_THRESHOLD);
    while(histogram->data[upper--]<IMAGE_NOISE_THRESHOLD);


    //-----------------------------------------------------------------------------
    //application of adaptive median filter
    dsvi_Image_t* fImage = dsvi_image_medianFilterAdaptive(defImage,lower,upper,&_status);
    checkStatus("%s\tapplying median filter",filename);
    {
        dsvi_Image_t* fImage888 = dsvi_image_convertTo888(fImage,&_status);

        char str[STRING_BUFFER_SIZE]   = "bin/Resources/test_images/";
        strcat(str,filepath);
        strcat(str,filename);
        strcat(str,"_filtered.bmp");
        _status = dsvi_image_bitmap_write(fImage888,str);
        checkStatus("%s\twriting filtered image",filename);

        _status = dsvi_image_delete(fImage888);
        checkStatus("%s\tdeleting fImage888",filename);
    }
    //-----------------------------------------------------------------------------

    _status = dsvi_image_histogram_delete(histogram);
    checkStatus("%s\tdeleting histogram",filename);
    _status = dsvi_image_delete(histImage);
    checkStatus("%s\tdeleting histImage",filename);

    //floor cutout
    dsvi_template_t srcTemplate = {0};
    srcTemplate.templateShape  = enTrapezium;
    srcTemplate.x              = 40;
    srcTemplate.y              = 480;
    srcTemplate.parameters     = malloc(2* sizeof*srcTemplate.parameters);
    srcTemplate.parameters[0]  = 600;//base
    srcTemplate.parameters[1]  = 160;//height
    dsvi_Image_t* cutout = dsvi_image_getTemplate(fImage,&srcTemplate,0,&_status);
    checkStatus("%s\t[template]get floor",filename);

    {
        dsvi_Image_t* cutout888     = dsvi_image_convertTo888(cutout,&_status);
        checkStatus("%s\tconverting floor cutout image format to 888",filename);

        char cutout_str[STRING_BUFFER_SIZE] = "bin/Resources/test_images/";
        strcat(cutout_str,filepath);
        strcat(cutout_str,filename);
        strcat(cutout_str,"_cutout.bmp");
        _status = dsvi_image_bitmap_write(cutout888,cutout_str);
        checkStatus("%s\twriting cutout.bmp",filename);

        _status = dsvi_image_delete(cutout888);
        checkStatus("%s\tdeleting cutout888",filename);
    }

    dsvi_histogram_t* cutout_histogram = dsvi_image_getHistogram(cutout,hInfo,&_status);
    checkStatus("%s\thistogram of cutout",filename);
    {
        char cutoutHistogramStr[STRING_BUFFER_SIZE]   = "bin/Resources/test_images/";
        strcat(cutoutHistogramStr,filepath);
        strcat(cutoutHistogramStr,filename);
        strcat(cutoutHistogramStr,"_cutout_histogram.txt");

        FILE* cutoutHistogramHandle = fopen(cutoutHistogramStr,"w");

        if((cutoutHistogramHandle!=NULL)&&(_status==DSVI_STATUS_OK))
        {
            int j=0;
            fprintf(cutoutHistogramHandle,"file = %s\n",cutoutHistogramStr);
            for(j=0;j<hInfo->bins;j++)
            {
                if(cutout_histogram->data[j]!=0)
                {
                    fprintf(cutoutHistogramHandle,"%d:%d\n",j,cutout_histogram->data[j]);
                }
            }
            _status = fclose(cutoutHistogramHandle);
            checkStatus("%s\tclosing histogram txt file",filename);
        }else
        {
            printf("\n%s\terror : writing histogram in txt file\n",filename);
        }
    }
    //
    dsvi_Image_t* cutout_histImage = dsvi_image_histogramToImageFixedHeight(cutout_histogram,&_status);
    checkStatus("%s\tcutout : histogram to image",filename);
    dsvi_Image_t* cutout_histImage888 = dsvi_image_convertTo888(cutout_histImage,&_status);
    checkStatus("%s\tcutout histImage format conversion to 888",filename);
    {
        char cutoutHistImageStr[STRING_BUFFER_SIZE]   = "bin/Resources/test_images/";
        strcat(cutoutHistImageStr,filepath);
        strcat(cutoutHistImageStr,filename);
        strcat(cutoutHistImageStr,"_cutout_histogram.bmp");
        _status = dsvi_image_bitmap_write(cutout_histImage888,cutoutHistImageStr);
        checkStatus("%s\twriting histogram image",filename);
    }
    _status = dsvi_image_delete(cutout_histImage);
    checkStatus("%s\tdeleting cutout_histImage",filename)
    _status = dsvi_image_delete(cutout_histImage888);
    checkStatus("%s\tdeleting cutout_histImage888",filename);

    //extract boundary and threshold accordingly
    cutout_histogram->data[0] -= 25440;
    lower=0;
    upper=254;

    while(cutout_histogram->data[lower++]<CUTOUT_THRESHOLD);
    while(cutout_histogram->data[upper--]<CUTOUT_THRESHOLD);

    printf("\n[threshold Lower]:%d\n[threshold Upper]:%d\n",lower,upper);

    dsvi_Image_t* bwImage = dsvi_image_bandThreshold(defImage,lower,upper,&_status);

    {
        dsvi_Image_t* bwImage888 = dsvi_image_convertTo888(bwImage,&_status);

        char bwImageStr[STRING_BUFFER_SIZE]   = "bin/Resources/test_images/";
        strcat(bwImageStr,filepath);
        strcat(bwImageStr,filename);
        strcat(bwImageStr,"_bw.bmp");
        _status = dsvi_image_bitmap_write(bwImage888,bwImageStr);
        checkStatus("%s\twriting histogram image",filename);

        _status = dsvi_image_delete(bwImage888);
        checkStatus("%s\tdeleting bwImage888",filename);
    }

    _status = dsvi_image_delete(bwImage);
    checkStatus("%s\tdeleting bwImage",filename);
    _status = dsvi_image_histogram_delete(cutout_histogram);
    checkStatus("%s\tdeleting cutout-histogram",filename);
    _status = dsvi_image_delete(cutout);
    checkStatus("%s\tdeleting cutout image",filename);

    dsvi_Image_t* bwFImage = dsvi_image_bandThreshold(fImage,lower,upper,&_status);
    {
        dsvi_Image_t* bwFImage888 = dsvi_image_convertTo888(bwFImage,&_status);

        char str[STRING_BUFFER_SIZE]   = "bin/Resources/test_images/";
        strcat(str,filepath);
        strcat(str,filename);
        strcat(str,"_bw_filtered.bmp");
        _status = dsvi_image_bitmap_write(bwFImage888,str);
        checkStatus("%s\twriting filtered image",filename);

        _status = dsvi_image_delete(bwFImage888);
        checkStatus("%s\tdeleting bwFImage888",filename);
    }

    _status = dsvi_image_delete(bwFImage);
    checkStatus("%s\tdeleting bwFImage",filename);
    _status = dsvi_image_delete(fImage);
    checkStatus("%s\tdeleting fImage",filename);
    //
    _status = dsvi_image_delete(defImage);
    return _status;
}
