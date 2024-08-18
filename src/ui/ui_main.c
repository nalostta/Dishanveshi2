#include <stdio.h>
#include <webui.h>


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
    bool ret = webui_set_port(my_window, 1000);
    printf("hosting at: http://localhost:1000\n");
    webui_bind(my_window, "", events);

    webui_set_config(multi_client, true);
    webui_show(my_window, "<html><head><script src=\"webui.js\"></script></head> Hello World ! </html>");
    webui_wait();
    return 0;
}