#include <stdio.h>
#include <webui.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

void events(webui_event_t* e) 
{
    if(e->event_type == WEBUI_EVENT_CONNECTED)
        printf("Connected. \n");
    else if(e->event_type == WEBUI_EVENT_DISCONNECTED)
        printf("Disconnected. \n");
    else if(e->event_type == WEBUI_EVENT_MOUSE_CLICK)
        printf("Click. \n");
    //else if(e->event_type == WEBUI_EVENT_NAVIGATION)
    //    printf("Starting navigation to: %s \n", e->data);
}

int main()
{
    size_t my_window = webui_new_window();
    bool ret = webui_set_port(my_window, 1234);
    printf("hosting at: http://localhost:1234\n");
    webui_bind(my_window, "", events);

    char root_path[100];
    getcwd(root_path, 100);
    printf("Root path: %s\n", root_path);
    DIR* root_folder = opendir(root_path);

    if(root_folder==NULL)
    {
        printf("Error opening root folder\n");
        return 1;
    }else 
    {
        struct dirent* file;
        int flag = 1;
        do
        {
            file = readdir(root_folder);
            if(file==NULL)
            {
                flag=-1;
            }else if(strcmp(file->d_name, "res")==0)
            {
                flag = 0;
            }
        }while(flag==1);

        char res_path[100];
        strcpy(res_path, root_path);
        strcat(res_path, "/res");

        if(flag==-1)
        {
            mkdir(res_path, 0777);
        }

        //access res folder
        DIR* res_folder = opendir(res_path);

    }
    

    webui_set_config(multi_client, true);
    //webui_show(my_window, "<html><head><script src=\"webui.js\"></script></head> Hello World ! </html>");
    //webui_wait();
    return 0;
}