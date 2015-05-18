
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
 * Filename: util.cpp
 * Created : 22 Jun 2013
 **/


#include "util.h"
#include <stdio.h>
#include <QString>
#include <QCoreApplication>

Util::Util()
{
}

QString Util::quoteWhenContainSpaces(const QString &str)
{
    QString ret(str);
    if (ret.contains(QChar(' '))        &&
        !ret.startsWith(QChar('\"'))    &&
        !ret.startsWith((QChar('\'')))
       )
    {
        ret.prepend(QChar('\"'));
        ret.append(QChar('\"'));
    }
    return  ret;
}


void Util::info(const QString& information, const QString& optionalLabel)
{
    Util::info(information.toLatin1().constData() , optionalLabel.toLatin1().constData());
}


void Util::info(const char *info, const char *label)
{
    const char *mylabel = "INFO";
    Util::msg(info,  (label && *label) ?
                     label : mylabel,
                     stdout
             );
}



void Util::error(const QString& information, const QString& optionalLabel)
{
    Util::error(information.toLatin1().constData() , optionalLabel.toLatin1().constData());
}


void Util::error(const char *info, const char *label)
{
    const char *mylabel = "ERROR";
    Util::msg(info,  (label && *label) ?
                     label : mylabel,
                     stderr
             );
}


void Util::msg(const char *msg, const char *label, FILE *stream)
{
    fprintf(stream, "\n[%s %s]: %s\n",  QCoreApplication::applicationName().toLatin1().constData(),
                                        label,
                                        msg);

    fflush(stream);
}
