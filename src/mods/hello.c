#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../mod.h"

const char *name = "Hello Mod";
const char *desc = "Hello Mod Description";

const char *getName(void)
{
    return name;
}

const char *getDesc(void)
{
    return desc;
}

void print(void)
{
    printf("Hello from the mod!\n");
}