By Carlos J. Mazieri 
carlos.mazieri@gmail.com

March 2013

            

The Qt Project Tool (qpt)


The "qpt" (Qt Project Tool) is a command line utility that aims to extract information

from Qt projects (qmake managed .pro files) making it easy to use project set of files and settings as input to 
other command line tools.

As example other tools can be: Static Analyzers, Code Counters and others.


When "qpt" is used with Static Analyzer tools it also can work as a Qt Creator plugin, 
making it easy and fast to
change source code inside Qt Creator.


So far it is known to work with the following  Static Analyzers:
           
    cppcheck http://cppcheck.sourceforge.net/
           
    krazy    http://www.englishbreakfastnetwork.org/krazy




It was written for Qt and uses Qt of course, works with Qt4 or Qt5, see http://qt-project.org



To build:
            
   $  qmake
            
   $  make



Documentation is available in the "doc" directory:

                qpt_manual.pdf





Acknowledgement

=================

Many thanks to Jürgen Querengässer <msg212@gmx.de>  who modified the code in order to have 0.7 version working on Windows.

He also provided a compiled 0.7 Windows version.
