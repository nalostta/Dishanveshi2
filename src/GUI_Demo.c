#define DSVI_CONFIG_PUBLIC_IMAGE
#define DSVI_CONFIG_PUBLIC_HISTOGRAM
//
#include <stdio.h>
#include <app_sysinit.h>

#include <pcf/ui/ui.h>
#include <pcf/graphics/graphics.h>

#include <pcf/ui/mainform/mainform.h>
#include <pcf/ui/control/control.h>
#include <pcf/ui/button/button.h>
#define enBoolIsDefined
//
#include <dishanveshi/init.h>
#include <dishanveshi/image/image.h>
#include <dishanveshi/GUI/GUI.h>
#include <bitmap.h>
#include <string.h>
#include <misc.h>


int main()
{
    int failed = app_sysinit();
    if(failed) {
        printf("System failed to initialize \n");
        return failed;
    }
    printf("System Initialized\n");

    dsvi_status_t _status = dsvi_test_init();
    checkStatus("[DSVI] : init");

    UiMainForm_t* mainWindow = dsvi_gui_createMainWindow(&_status);
    checkStatus("[creating GUI] : creating main window");
    dsvi_gui_addMainLabel(mainWindow);
    dsvi_gui_addImgPathEditBox(mainWindow);
    dsvi_gui_addAllButtons(mainWindow);
    dsvi_gui_addPreviews(mainWindow);
    dsvi_gui_addImgFilesList(mainWindow);
    dsvi_gui_addChList(mainWindow);
    dsvi_gui_addThshldBar(mainWindow);
    dsvi_gui_addAdvFn(mainWindow);
    int32_t exitcode;
    pcf_ui_mainform_run(mainWindow,&exitcode);
    return exitcode;
}
