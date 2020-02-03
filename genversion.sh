#!/bin/bash

echo "// Automatically generated. Do not edit."  > version.h
echo "#ifndef VERSION_H" >> version.h
echo "#define VERSION_H" >> version.h
echo "#define OJUDGE_VERSION_MAJOR $1" >> version.h
echo "#define OJUDGE_VERSION_MINOR $2" >> version.h
echo "#define OJUDGE_VERSION_RELEASE $3" >> version.h
echo "#define OJUDGE_VERSION_COMPILE `date -u +%Y%m%d%H%M%S`" >> version.h
echo "#endif // VERSION_H" >> version.h

