#!/bin/sh
echo $(($(cat version/version.txt) + 1)) > version/version.txt
/bin/echo -n "static const int version = " > version/version.h
cat version/version.txt  | tr -d "\n" >> version/version.h
echo ";" >> version/version.h