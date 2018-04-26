// Urchin - Painful for a container to step on

/* main.c
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
#include <getopt.h>
#include <unistd.h>

#include "mem.h"
#include "utils.h"
#include "sig.h"
#include "fork.h"
#include "httpd.h"

#include "log.h"

static struct option long_options[] =
{
    {"mem", required_argument, NULL, 'n'},
    {"signal", required_argument, NULL, 's'},
    {"exec", required_argument, NULL, 'e'},
    {"help", optional_argument, NULL, 'h'},
    {"webport", required_argument, NULL, 'w'},

    {NULL, 0, NULL, 0}
};

void printUsage() {
    printf("Docker Urchin\n");
    printf("--------------\n");
    printf("\t-m <bytes to allocate>\n");
    printf("\t-s <signal stuff>\n");         //TODO
    printf("\t-e <exec/fork stuff>\n");      //TODO
    printf("\t-w <port for webserver>\n");   //TODO
    printf("\t-c <content type>\n");   //TODO

}

int main(int argc, char**argv)
{
    logInit();
    int ch, zombies, webEnabled = 0;
    char *argument= NULL, *contentType = NULL;

    if (argc >= 2) {
        if (stringMatch("version", argv[1])) {
            logInfo("\n -- Urchin -- \n -- v0.01 -- \n Contact dan@thebsdbox.co.uk\n\n");
            return 0;
        }
    }

    printf("Urchin has started as PID: %d\n", getpid());
    
    while ((ch = getopt_long(argc, argv, "m:s:e:w:h", long_options, NULL)) != -1)
    {
        // check to see if a single character or long option came through
        switch (ch)
        {
            // short option 'm' - memory options
            case 'm':
                argument = optarg;
                int m = atoi(argument);
                printf("Attempting to allocate %d bytes of memory\n", m);
                // Set the memory to allocate
                setMem(m);
                // Attempt to allocate the memory
                if (allocateMemory() == 1) {
                    return 1;
                }
                break;
                // short option 'w' - signal handling
            case 's':
                argument = optarg;

                return 0;
                break;
            case 'e':
                argument = optarg;
                zombies = atoi(argument);
                int spawned = forkSelf(zombies);
                if (spawned != 0) {
                    printf("Failed to create zombies!\n");
                }
                break;
           case 'w':
                // Set a port to bind to
                logInfo("Starting WebServer\n");
                argument = optarg;
                int port = atoi(argument);
                setPort(port);
        
                 if (contentType != NULL) {
                    setContentType(contentType);
                }
                // create a socket
                createINETSocket();
                // bind to that socket
                bindToINETSocketWithPort();
                // start listening on that socket
                startListener();
                webEnabled = 1;
                break;
            case 'h':
                printUsage();
                return 0;
                break;
            default:
                printf("\n");
                printUsage();
                return 0;
                break;
        }
    }
    printf("%s\n", getMemoryConfiguration());
    //setSignalHander();
    while(1) {
        if (webEnabled == 1) {
            // check for connections and act accordingly 
            acceptConnection();
        }
        sleep(1);
    };
    return 0;
}
