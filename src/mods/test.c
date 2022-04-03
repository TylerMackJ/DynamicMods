#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../mod.h"

const char *name = "Test Mod";
const char *desc = "Test Mod Description";

const char *getName(void)
{
    return name;
}

const char *getDesc(void)
{
    return desc;
}