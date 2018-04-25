/* log.h
 *
 * Copyright (C) 2018 < Daniel Finneran, dan@thebsdbox.co.uk >
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the GPL license.  See the LICENSE file for details.
 */

void setLoglevel(int log);
void logInit(int log);

// Logging commands

void logDebug(char *text); //5
void logInfo(char *text);  //4
void logWarn(char *text);  //3
void logError(char *text); //2
void logFatal(char *text); //1

void logInit(); //Needed to enable logging