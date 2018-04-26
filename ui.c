//
//  ui.c
//  urchin
//
//  Created by Daniel Finneran on 26/04/2018.
//  Copyright © 2018 Daniel Finneran. All rights reserved.
//

#include <string.h>
#include <stdio.h>

#include "httpd.h"
#include "log.h"
#include "httpui.h"
#include "mem.h"
#include "utils.h"

char *handleGetData(httpRequest *request)
{
    char *updateText = NULL;
    if (request->URI == NULL) {
        updateText = "NULL";
    }
    
    logInfof("GET request for %s\n", request->URI);

    if (stringMatch("/oom", request->URI)) {
        updateText = getMemoryConfiguration();

    }
    if (stringMatch("/poop", request->URI)) {
        updateText ="poop";
    }
    if (stringMatch("/hostname", request->URI)) {
        updateText = readFile("/etc/hostname");
    }
    if (updateText == NULL) {
        updateText = "¯\\_(ツ)_/¯";
    }
    
    unsigned long newBufferSize = strlen(updateText) + strlen((char *)htmlfiles_index_html);
    char buffer[newBufferSize];
    sprintf(buffer, (char *)htmlfiles_index_html, updateText );
    return strdup(buffer);
    
}
