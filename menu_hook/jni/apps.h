#include <stdbool.h>

typedef struct
{
    const char* name;
    const char* exec;
    const char* icon;
    const char* icon_active;

} application;

//ugh, maybe don't be lazy and actually count the number of valid entries _before_ allocating?
extern application* apps[256];
extern int app_count;

void readApps(char* desktopFolder);
void writeMenuXML(char* original, char* destination, char* insertAfter, application* apps[], int numApps);