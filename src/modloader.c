#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mod.h"

typedef struct Mod
{
    void *handle;

    const char *(*getName)(void);
    const char *(*getDesc)(void);

    char printLoaded;
    void (*print)(void);
} Mod;

int main()
{
    int modsCount = 0;
    Mod *mods;
    DIR *dirp;
    struct dirent *entry;

    // Count mods to load
    dirp = opendir("./mods/");
    while ((entry = readdir(dirp)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            char *fileType = strrchr(entry->d_name, '.');
            if (fileType != NULL)
            {
                if (strcmp(fileType, ".mod") == 0)
                {
                    modsCount++;
                }
            }
        }
    }

    // Allocate space for mods
    mods = (Mod *)malloc(sizeof(Mod) * modsCount);

    // Load all mods
    modsCount = 0;
    dirp = opendir("./mods/");
    // Loop over mods folder
    while ((entry = readdir(dirp)) != NULL)
    {
        // Get all regular files
        if (entry->d_type == DT_REG)
        {
            // Check contains '.'
            char *fileType = strrchr(entry->d_name, '.');
            if (fileType != NULL)
            {
                // Check if ends with ".mod"
                if (strcmp(fileType, ".mod") == 0)
                {
                    // Get full path
                    char path[250] = "./mods/";
                    strcat(path, entry->d_name);

                    // Get the handle
                    mods[modsCount].handle = dlopen(path, RTLD_DEEPBIND | RTLD_LAZY);
                    if (!mods[modsCount].handle)
                    {
                        fprintf(stderr, "Failed to load mod %s\n", entry->d_name);
                        exit(1);
                    }
                    dlerror();

                    // Load required functions
                    // Load getName
                    mods[modsCount].getName = dlsym(mods[modsCount].handle, "getName");
                    char *error;
                    if ((error = dlerror()) != NULL)
                    {
                        fprintf(stderr, "getName is required in mod %s\n", entry->d_name);
                        exit(1);
                    }

                    // Load geDesc
                    mods[modsCount].getDesc = dlsym(mods[modsCount].handle, "getDesc");
                    if ((error = dlerror()) != NULL)
                    {
                        fprintf(stderr, "getDesc is required in mod %s\n", entry->d_name);
                        exit(1);
                    }

                    // Move on to next mod
                    modsCount++;
                }
            }
        }
    }

    // Optionally load print function from all mods
    for (int i = 0; i < modsCount; i++)
    {
        mods[i].print = dlsym(mods[i].handle, "print");

        char *error;
        if ((error = dlerror()) != NULL)
        {
            mods[i].printLoaded = 0;
        }
        else
        {
            mods[i].printLoaded = 1;
        }
    }

    // Print out mod names and desc and print if available
    for (int i = 0; i < modsCount; i++)
    {
        printf("Name: %s\nDescription: %s\n", (*mods[i].getName)(), (*mods[i].getDesc)());
        if (mods[i].printLoaded)
        {
            (*mods[i].print)();
        }
    }

    // Close all mods
    for (int i = 0; i < modsCount; i++)
    {
        dlclose(mods[i].handle);
    }
    free(mods);

    return 0;
}