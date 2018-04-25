/* log.c
 *
 * Copyright (C) 2018 < Daniel Finneran, dan@thebsdbox.co.uk >
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the GPL license.  See the LICENSE file for details.
 */

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"

int loglevel;
long seedTime;

void setLogLevel(int log)
{
    loglevel = log;
}

void logInit()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    seedTime = time(NULL);
    setLogLevel(4);
}

void logDebug(char *text)
{
    if (loglevel < 4) 
        return;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long newTimeCount = time(NULL);
    printf("\x1B[37mDEBG\x1B[0m[%04ld] %s\n", (newTimeCount - seedTime), text);
}

void logInfo(char *text)
{
    if (loglevel < 3) 
        return;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long newTimeCount = time(NULL);
    printf("\x1B[34mINFO\x1B[0m[%04ld] %s\n", (newTimeCount - seedTime), text);
}

void logWarn(char *text)
{
    if (loglevel < 2) 
        return;
    long newTimeCount = time(NULL);
    printf("\x1B[33mWARN\x1B[0m[%04ld] %s\n", (newTimeCount - seedTime), text);
}

void logError(char *text)
{
    if (loglevel < 1) 
        return;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long newTimeCount = time(NULL);
    printf("\x1B[31mERRO\x1B[0m[%04ld] %s\n", (newTimeCount - seedTime), text);
}

void logFatal(char *text)
{
    if (loglevel <= 0) 
        return;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long newTimeCount = time(NULL);
    printf("\x1B[36mERRO\x1B[0m[%04ld] %s\n", (newTimeCount - seedTime), text);
    exit(1);
}