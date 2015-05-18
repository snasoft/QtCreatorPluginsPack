/*
 *  Copyright (c) 2013 Carlos J. Mazieri <carlos.mazieri@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Filename: tool.cpp
 * Created : 26 Jan 2013
 **/


#include "manager.h"
#include "qprojcommandline.h"
#include "configtool.h"

#include <QCoreApplication>
#include <QDebug>
#include <QTemporaryFile>
#include <QTimer>
#include <QFileInfo>


int main(int argc, char *argv[])
{   
    QCoreApplication app(argc, argv);

    {
       QString appname(argv[0]);
       QFileInfo app(appname);
       QCoreApplication::setApplicationName(app.fileName());
    }

    QCoreApplication::setOrganizationName("QtProjectTool");
    QCoreApplication::setOrganizationDomain("sourceforge.net");
    QCoreApplication::setApplicationVersion(VERSION);

    Parameters  parameters;
    QProjCommandLine  commandLine(parameters);

    Manager manager(parameters);

    QObject::connect(&commandLine, SIGNAL(error()), &app, SLOT(quit()));
    QObject::connect(&commandLine, SIGNAL(done()),  &manager, SLOT(start()));
    QObject::connect(&manager,     SIGNAL(error()), &app, SLOT(quit()));
    QObject::connect(&manager,     SIGNAL(done()),  &app, SLOT(quit()));

    QTimer::singleShot(30, &commandLine, SLOT(parse()));

    int ret = app.exec();  
    return ret;
}
