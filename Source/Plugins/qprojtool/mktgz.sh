#!/bin/sh

if [ -f Makefile ]
then
   make distclean
fi


/bin/rm -rf bin Makefile .moc .obj .ui .qmake*

VERSION=`cat version.txt`

tar -cvz --exclude=*~ -X exclude.tar  -f ../qpt-source-${VERSION}.tgz .
