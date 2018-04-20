#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int urchinMemorySize;
char *urchinMemory;

void setMem(int mem) {
    urchinMemorySize = mem;
}

int getMemSize() {
    return urchinMemorySize;
}

int allocateMemory() {
     urchinMemory = malloc(sizeof(char) * urchinMemorySize);
    
    // if c is allocated then populate the memory
    if(urchinMemory)
    {
        printf("Operating System allowed allocation of memory\n");
        memset(urchinMemory, 1, sizeof(char) * urchinMemorySize);
        return 0;
    }
    else
    {
        printf("Operatings system denied memory allocation\n");
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
        return "Couldn't Read file [/proc/sys/vm/overcommit_memory]";
    }
    if (oom[0] == '0' ) {
        printf("OOM Enabled (Mode 0)\n");
    }
    if (oom[0] == '1' ) {
        printf("OOM Enabled (Mode 1)\n");
    }
    if (oom[0] == '2' ) {
        printf("OOM Disabled\n");
    }
    if (stringMatch("2", &oom[0])) {
        return "Over Commited Memory Disabled.";
    }
    if (oom[0] == '2' ) {
        printf("check\n");
    }
    return "Unknown Memory"; // If Nil then there was an error reading the file
}
