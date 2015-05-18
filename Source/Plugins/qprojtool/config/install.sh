#!/bin/sh

## Qt5
if [ -d ~/.config/QtProject/qtcreator ]
then
   mkdir -p ~/.config/QtProject/qtcreator/externaltools
   /bin/cp *.xml  ~/.config/QtProject/qtcreator/externaltools
fi

#Qt4
if [ -d ~/.config/Nokia/qtcreator ]
then
    mkdir -p ~/.config/Nokia/qtcreator/externaltools
   /bin/cp *.xml ~/.config/Nokia/qtcreator/externaltools
fi


