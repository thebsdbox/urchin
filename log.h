/* log.h
 *
 * Copyright (C) 2018 < Daniel Finneran, dan@thebsdbox.co.uk >
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the GPL license.  See the LICENSE file for details.
 */

void setLoglevel(int);

// Logging commands

int logDebug(); //5
int logInfo();  //4
int logWarn();  //3
int logError(); //2
int logFatal(); //1

void logInit(); //Needed to enable logging