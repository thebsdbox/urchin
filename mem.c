/* mem.c
 *
 * Copyright (C) 2018 < Daniel Finneran, dan@thebsdbox.co.uk >
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the GPL license.  See the LICENSE file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "log.h"

long urchinMemorySize;
char *urchinMemory;

void setMem(long mem) {
    urchinMemorySize = mem;
}

long getMemSize() {
    return urchinMemorySize;
}

int allocateMemory() {
     urchinMemory = malloc(sizeof(char) * urchinMemorySize);
    
    // if c is allocated then populate the memory
    if(urchinMemory)
    {
        logInfo("Operating System allowed allocation of memory\n");
        memset(urchinMemory, 1, sizeof(char) * urchinMemorySize);
        return 0;
    }
    else
    {
        logError("Operatings system denied memory allocation\n");
        return 1;
    }
}

void freeMemory() {
    free(urchinMemory);
}

char* getMemoryConfiguration() {
    char *oom;
    oom = readFile("/proc/sys/vm/overcommit_memory");
    if (oom == 0) {
        logWarn("Unable to read OOM settings\n");
        return "Couldn't Read file [/proc/sys/vm/overcommit_memory]";
    }
    if (oom[0] == '0' ) {
        return("OOM Enabled (Mode 0)\n");
    }
    if (oom[0] == '1' ) {
        return("OOM Enabled (Mode 1)\n");
    }
    if (oom[0] == '2' ) {
        return("OOM Disabled\n");
    }
    return "Unknown Memory Configuration in kernel"; // If Nil then there was an error reading the file
}
