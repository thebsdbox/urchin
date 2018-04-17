// Urchin - designed to be a pain to containers
// danf@docker.com / daniel.finneran@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "mem.h"
#include "utils.h"
#include "sig.h"
#include "fork.h"

static struct option long_options[] =
{
    {"mem", required_argument, NULL, 'n'},
    {"signal", required_argument, NULL, 's'},
    {"exec", required_argument, NULL, 'e'},
    {"help", optional_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
};

void printUsage() {
    printf("Docker Urchin\n");
    printf("--------------\n");
    printf("\t-m <bytes to allocate>\n");
    printf("\t-s <signal stuff>\n");         //TODO
    printf("\t-e <exec/fork stuff>\n");      //TODO
}

int main(int argc, char**argv)
{
    int ch, zombies;
    char *argument;

    if (argc >= 2) {
        if (stringMatch("version", argv[1])) {
            printf("\n -- Urchin -- \n -- v0.01 -- \n Contact dan@thebsdbox.co.uk\n\n");
            return 0;
        }
    }
    
    printf("Urchin has started as PID: %d\n", getpid());
    
    while ((ch = getopt_long(argc, argv, "m:s:e:h", long_options, NULL)) != -1)
    {
        // check to see if a single character or long option came through
        switch (ch)
        {
            // short option 'm' - memory options
            case 'm':
                argument = optarg;
                int m = atoi(argument);
                printf("Attempting to allocate %d bytes of memory\n", m);
                return 0;
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
    setSignalHander();
    while(1);
    return 0;
}
