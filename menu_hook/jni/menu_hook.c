#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h>
#include <unistd.h>
#include <dlfcn.h>

#include "apps.h"

#define DESKTOP_DIR "/opt/share/applications/"
#define ORIG_MENU_XML "/system/gui/xml/racing_top_menu.xml.orig"
#define TARGET_MENU_XML "/system/gui/xml/racing_top_menu.xml"

#define XML_INSERT_AFTER "widget.menubutton.data = \"Exit\"\n                    />\n		-->"

static void *guiLib = 0;
static uint32_t (* getSelected)(void* this) = 0;

void __attribute__((constructor)) menu_hook_init() {
    readApps(DESKTOP_DIR);
    printf("menu_hook app_count: %d\n", app_count);
    //printf("app 2: %s %s %s\n", apps[1]->name, apps[1]->exec, apps[1]->icon);
    if (access(ORIG_MENU_XML, F_OK) != 0) {
        printf("Could not open original menu xml file: %s", ORIG_MENU_XML);
        return;
    }
    
    writeMenuXML(ORIG_MENU_XML, TARGET_MENU_XML, XML_INSERT_AFTER, apps, app_count);

}

uint32_t _ZN18GlassRacingTopMenu11onMenuInputEP9MMSWidgetP13MMSInputEvent(void *this,void *menu_widget,void *input_event){
    if(!guiLib){
        guiLib = dlopen("/system/lib/libmmscore.so", 1);
    }

    if (getSelected == 0){
        getSelected = dlsym (guiLib, "_ZN13MMSMenuWidget11getSelectedEv");
        if (getSelected == 0)
        {
            printf("dlsym: %s\n", dlerror());
        }
    }
    /* 	
        //explore the event
        for(int i=0; i < 16; i++){
            printf("%x ", *(int *)((int)input_event + i));
        }
    */	


	//(input_event + 4) == 3 //joystick button pressed.
	if(((*(int *)input_event == 1) && (*(int *)(input_event + 4) == 3))){
		int selected = getSelected(menu_widget);
    	//printf("SELECTED %d\n", selected);
        if(selected > 3 && selected < 4 + app_count) {
            int idx = selected - 4;
            char cmd[255];
            sprintf(&cmd, "sh -c '. /etc/mkshrc && %s'", apps[idx]->exec);
            printf("menu_hook executing command %s", cmd);
            system(&cmd);
        }
	}

	
	//original code. Runs only on exit of top menu by pressing the back button.
	if ((*(int *)input_event == 1) && (*(int *)(input_event + 4) == 4)) {
		*(uint32_t *)(this + 0x10c) = 0x33;
		return 1;
	}
	return 0;	
}