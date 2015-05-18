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
 * Filename: manager.h
 * Created : 2 Feb 2013
 **/


#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include "qprojcommandline.h"


class QMake;
class BackEndTool;
struct ConfigToolData;
class ConfigTool;


/*!
 * \brief The Manager class does the work after the command line being processed
 *
 *  It finds "qmake" using \ref QMake class.
 *  If qpt was called just called just to extract information from the project file
 *  \ref processLists() is called and done() is emitted.
 *
 *  if qpt was called as wrapper to any Static Analisis tool \ref processTool() is called
 *  and \ref BackEndTool class will execute and handle everything. The slots \ref toolFinished()
 *  and \ref toolFailed() should receive the result from \ref BackEndTool class.
 *
 *  It should emit done() or error() to say that the the  work has finished
 */
class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(Parameters &params);
    ~Manager();
signals:
    void     done();
    void     error();

public slots:
    void     start();
    void     toolFinished(int retCode);
    void     toolFailed(int);

private:
    void     showError(const QString& msg);
    void     processLists();
    void     processTool();
    void     printList(const QStringList &l);

private:
    Parameters&            m_params;
    QMake       *          m_qmake;
    BackEndTool *          m_backEndTool;
    QList<ConfigToolData*> m_configData;  
};

#endif // MANAGER_H
