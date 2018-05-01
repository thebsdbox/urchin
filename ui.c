//
//  ui.c
//  urchin
//
//  Created by Daniel Finneran on 26/04/2018.
//  Copyright © 2018 Daniel Finneran. All rights reserved.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "httpd.h"
#include "log.h"
#include "httpui.h"
#include "mem.h"
#include "utils.h"
#include "fork.h"

char *handleGetData(httpRequest *request)
{
    char *updateText = NULL;
    if (request->URI == NULL) {
        updateText = "NULL";
    }
    
    char *parse = strdup(request->URI);
    char *URI = strtok(parse, "?");
    strtok(NULL, "=");
    char *query = strtok(NULL, "q");
    
    
    logInfof("GET request for %s\n", request->URI);

    if (stringMatch("/oom", URI)) {
        updateText = getMemoryConfiguration();
    }
    if (stringMatch("/poop", URI)) {
        updateText ="poop";
    }
    if (stringMatch("/mem", URI))
    {
        if (query) {
            char *eptr;
            long result = strtol(query, &eptr, 10);
            setMem(result);
            allocateMemory();
            logInfof("Allocating %s bytes of memory\n", query);
            updateText = query;
        }
    }
    if (stringMatch("/hostname", URI)) {
        updateText = readFile("/etc/hostname");
    }
    
    //Process GETs
    
    if (stringMatch("/child", URI)) {
        forkSelf(1);
        updateText = "BRAAAAAAAAINZ";
    }
    
    if (stringMatch("/becomeparent", URI)) {
        becomeParent();
    }
    
    if (updateText == NULL) {
        updateText = "¯\\_(ツ)_/¯";
    }
    
    unsigned long newBufferSize = strlen(updateText) + strlen((char *)htmlfiles_index_html);
    char buffer[newBufferSize];
    sprintf(buffer, (char *)htmlfiles_index_html, updateText );
    return strdup(buffer);
    
}

//char *getResponse(char *uri, char *query)
//{
//
//}
