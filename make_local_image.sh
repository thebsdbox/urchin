#!/bin/bash
echo "Creating a local docker image of urchin"
echo ""
echo "Dynamically building UI from ./htmlfiles"
xxd -i htmlfiles/index.html >> httpui.h
echo "Moving files to the dockerfile directory"
cp *.c ./local_dockerfile
cp *.h ./local_dockerfile
cd ./local_dockerfile
docker build -t urchin:local .
echo "Restoring local_dockerfile directory and include files"
git checkout -- ./local_dockerfile
git checkout -- ./httpui.h
