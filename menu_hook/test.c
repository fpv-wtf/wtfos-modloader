#include <stdio.h>

#include "jni/apps.h"

int main(int argc, char* argv[])
{
    
    readApps("./examples/");
    printf("app_count: %d\n", app_count);
    printf("app 2: %s %s %s\n", apps[1]->name, apps[1]->exec, apps[1]->icon);

    writeMenuXML("./racing_top_menu.xml.orig", "./racing_top_menu.xml", "widget.menubutton.data = \"Exit\"\n                    />\n		-->", apps, app_count);

    return(0);
}

