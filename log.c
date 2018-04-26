/* log.c
 *
 * Copyright (C) 2018 < Daniel Finneran, dan@thebsdbox.co.uk >
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the GPL license.  See the LICENSE file for details.
 */

#include <sys/time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "log.h"

int loglevel;
long seedTime;
#define STDOUT 1

void setLogLevel(int log)
{
    loglevel = log;
}

void logInit(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    seedTime = time(NULL);
    setLogLevel(4);
}

char *convert(unsigned int num, int base)
{
    static char Representation[]= "0123456789ABCDEF";
    static char buffer[50];
    char *ptr;
    
    ptr = &buffer[49];
    *ptr = '\0';
    
    do
    {
        *--ptr = Representation[num%base];
        num /= base;
    }while(num != 0);
    
    return(ptr);
}

void logDebug(char *text)
{
    if (loglevel < 4) 
        return;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long newTimeCount = time(NULL);
    printf("\x1B[37mDEBG\x1B[0m[%04ld] %s", (newTimeCount - seedTime), text);
}

void logInfo(char *text)
{
    if (loglevel < 3) 
        return;
    long newTimeCount = time(NULL);
    printf("\x1B[34mINFO\x1B[0m[%04ld] %s", (newTimeCount - seedTime), text);
}

void logInfof(char* format,...)
{
    if (loglevel < 3)
        return;
    char *traverse;
    unsigned int i;
    char *s;
    long newTimeCount = time(NULL);
    printf("\x1B[34mINFO\x1B[0m[%04ld] ", (newTimeCount - seedTime));

    va_list arg;
    va_start(arg, format);
    unsigned long stringLength = strlen(format);
    for(traverse = format; *traverse != '\0'; traverse++)
    {
        if (stringLength == 0)
            break;
        while( *traverse != '%')
        {
            if (*traverse == '\0') // Check for end of format
                break;
            putchar(*traverse);
            traverse++;
            stringLength--;
            if (stringLength == 0)
                break;
        }

        if (*traverse == '\0') // If end of format end 
            break;
        traverse++;
        

        switch(*traverse)
        {
            case 'c' : i = va_arg(arg,int);     //Fetch char argument
                putchar(i);
                break;
                
            case 'd' : i = va_arg(arg,int);         //Fetch Decimal/Integer argument
                if((signed int)i<0)
                {
                    i = -i;
                    putchar('-');
                }
                puts(convert(i,10));
                break;
                
            case 'o': i = va_arg(arg,unsigned int); //Fetch Octal representation
                puts(convert(i,8));
                break;
                
            case 's': s = va_arg(arg,char *);       //Fetch string
                if (s != NULL) {
                    fputs(s, stdout);
                }
                break;
                
            case 'x': i = va_arg(arg,unsigned int); //Fetch Hexadecimal representation
                puts(convert(i,16));
                break;
        }
        stringLength--;
    }
    
    va_end(arg);
}


void logWarn(char *text)
{
    if (loglevel < 2) 
        return;
    long newTimeCount = time(NULL);
    printf("\x1B[33mWARN\x1B[0m[%04ld] %s", (newTimeCount - seedTime), text);
}

void logError(char *text)
{
    if (loglevel < 1) 
        return;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long newTimeCount = time(NULL);
    printf("\x1B[31mERRO\x1B[0m[%04ld] %s", (newTimeCount - seedTime), text);
}

void logFatal(char *text)
{
    if (loglevel <= 0) 
        return;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long newTimeCount = time(NULL);
    printf("\x1B[36mERRO\x1B[0m[%04ld] %s", (newTimeCount - seedTime), text);
    exit(1);
}
