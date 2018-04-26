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

char *handleGetData(httpRequest *request)
{
    char *mem = getMemoryConfiguration();
    unsigned long newBufferSize = strlen(mem) + strlen((char *)htmlfiles_index_html);
    char buffer[newBufferSize];
    sprintf(buffer, (char *)htmlfiles_index_html, mem );
    logInfof("GET request for %s\n", request->URI);
    return strdup(buffer);
}
