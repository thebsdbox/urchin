#!/bin/bash
echo "Creating a local copy of urchin"
echo ""
echo "Dynamically building UI from ./htmlfiles"
xxd -i htmlfiles/index.html >> httpui.h
echo "Building urchin binary ./bin/urchin"
mkdir -p bin
gcc -Wall -o ./bin/urchin main.c mem.c utils.c sig.c fork.c log.c httpd.c ui.c
echo "Restoring include files"
git checkout -- httpui.h
