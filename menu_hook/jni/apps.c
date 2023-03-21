
#include "apps.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "inih/ini.h"

application* apps[256];
int app_count;

static int ini_parse_handler(void* user, const char* section, const char* name,
                   const char* value)
{
    application* papplcation = (application*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("Desktop Entry", "Name")) {
        papplcation->name = strdup(value);
    } else if (MATCH("Desktop Entry", "Exec")) {
        papplcation->exec = strdup(value);
    } else if (MATCH("Desktop Entry", "Icon")) {
        papplcation->icon = strdup(value);
    } else if (MATCH("Desktop Entry", "Icon_Active")) {
        papplcation->icon_active = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

bool endsWith (char* base, char* str) {
    int blen = strlen(base);
    int slen = strlen(str);
    return (blen >= slen) && (0 == strcmp(base + blen - slen, str));
}

void readApps(char* desktopFolder) {
    DIR *folder;
    struct dirent *entry;
    int files = 0;
    app_count = 0;

    folder = opendir(desktopFolder);
    if(folder == NULL)
    {
        printf("Unable to read applications directory: %s\n", desktopFolder);
        return;
    }

    while( (entry=readdir(folder)) )
    {
        files++;

        if(endsWith(entry->d_name, ".desktop")) {
            application app;
            memset(&app, 0, sizeof(application));

            char path[255];
            strcpy(path, desktopFolder);
            strcat(path, entry->d_name);
            if (ini_parse(path, ini_parse_handler, &app) < 0) {
                printf("Can't load '%s'\n", path);
                continue;
            }

            if(app.name != NULL && app.exec != NULL) {
                printf("Config loaded from '%s': name=%s, exec=%s, icon=%s icon_active=%s\n",
                    path, app.name, app.exec, app.icon, app.icon_active);
                apps[app_count] = malloc(sizeof(application));
                memcpy(apps[app_count], &app, sizeof(application));
                app_count++;
            }
        }

    }
    closedir(folder);
}

char* readFile(char* path) {
    char *source = NULL;
    FILE *fp = fopen(path, "r");
    if (fp != NULL) {
        /* Go to the end of the file. */
        if (fseek(fp, 0L, SEEK_END) == 0) {
            /* Get the size of the file. */
            long bufsize = ftell(fp);
            if (bufsize == -1) { /* Error */ }

            /* Allocate our buffer to that size. */
            source = malloc(sizeof(char) * (bufsize + 1));

            /* Go back to the start of the file. */
            if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

            /* Read the entire file into memory. */
            size_t newLen = fread(source, sizeof(char), bufsize, fp);
            if ( ferror( fp ) != 0 ) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0'; /* Just to be safe. */
            }
        }
        fclose(fp);
        return source;
    }
    return NULL;
}


void writeMenuXML(char* original, char* destination, char* insertAfter, application* apps[], int numApps) {
    //again with the memory management

    char* injection = malloc(numApps*(100+255+255+255));
    injection[0] = '\0';
    for(int i=0;i<numApps;i++){    
        sprintf(injection + (i == 0 ? 0 : strlen(injection)), 
            "\n                <menuitem widget.icon.image.name=\"../../..%s\" widget.icon.selimage.name=\"../../..%s\" widget.title.text=\"%s\" />", 
            apps[i]->icon, 
            (apps[i]->icon_active != NULL ? apps[i]->icon_active: apps[i]->icon), 
            apps[i]->name
        );
    }

    printf("%s\n", injection);

    char *originalFile = readFile(original);
    if(originalFile == NULL) {
        printf("Couldn't read original XML file: %s\n", original);
        return;
    }
    //printf("%s\n", originalFile);

    char* position = strstr(originalFile, insertAfter) + strlen(insertAfter);
    
    FILE *fp;
    fp = fopen(destination, "w");
    fwrite(originalFile, 1, (int)(position-originalFile), fp);
    fwrite(injection, 1, strlen(injection), fp);
    fwrite(position, 1, strlen(position), fp);

    fclose(fp);
    free(originalFile);
    free(injection);

}