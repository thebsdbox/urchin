#!/bin/bash
echo "Creating a local docker image of urchin"
echo ""
echo "Dynamically building UI from ./htmlfiles"
xxd -i htmlfiles/index.html >> httpui.h
echo "Moving files to the dockerfile directory"
mkdir ./local_dockerfile/tar
find . -name "*.c" -o -name "*.h" | tar -cf ./local_dockerfile/tar/urchin.tar -T -
cd ./local_dockerfile
docker build -t urchin:local .
echo "Restoring local_dockerfile directory and include files"
rm -rf ./local_dockerfile/tar
cd ..
git checkout -- ./httpui.h
