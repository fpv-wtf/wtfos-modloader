#include <stdlib.h>
#include <stdio.h> 

void __attribute__((constructor)) modloader_final_init() {
    //we do this so that dji-s excsessive spawns don't get preloaded
    unsetenv("LD_PRELOAD");
    printf("modloader finished, cleared LD_PRELOAD\n");
}


