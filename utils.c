/* utils.c
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

int stringMatch(char *string1, char *string2)
{
    if ((string1) && (string2)) {
        while (*string1 == *string2) {
            // if both points in strings are null break out of loop
            if (*string1 == '\0' || *string2 == '\0') {
                break;
            }
            // move onto next character comparison
            string1++;
            string2++;
        }
        // Have we reached the end of the string whilst both have been the same value
        if (*string1 == '\0' && *string2 == '\0')
            return 1; // Return true (compiler)
        else
            return 0; // Return false
    }
    return 0;
}

char *readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c;
    
    if (file == NULL) return NULL; //could not open file
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = malloc(f_size);
    
    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char)c;
    }
    
    code[n] = '\0';
    
    return code;
}
