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
 * Filename: util.h
 * Created : 22 Jun 2013
 **/


#ifndef UTIL_H
#define UTIL_H

#include <QString>

class Util
{
private:
    Util();
public:
    static QString quoteWhenContainSpaces(const QString& str);
    static void    info(const QString& information, const QString& optionalLabel = QLatin1String(0) );
    static void    info(const char *info, const char *label = 0);
    static void    error(const QString& information, const QString& optionalLabel = QLatin1String(0) );
    static void    error(const char *info, const char *label = 0);
private:
    static void    msg(const char *msg, const char *label, FILE *stream);
};

#endif // UTIL_H
