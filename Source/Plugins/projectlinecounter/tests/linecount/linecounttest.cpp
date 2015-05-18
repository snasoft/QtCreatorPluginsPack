#include "functions.h"

#include <QtTest>

class LineCountTest : public QObject
{
    Q_OBJECT
    
public:
    LineCountTest();
    
private Q_SLOTS:
    void lineCountTest();
};




LineCountTest::LineCountTest()
{
    qDebug() << "Working dir:" << QString(SRCDIR);
}



void LineCountTest::lineCountTest()
{           
    //Test lines count in file with Windows end of lines
    QString path = QString(SRCDIR) + "/testfile1";
    QCOMPARE(linesCount(path), (unsigned long)3091);

    //Test lines count in file with Unix end of lines
    path = QString(SRCDIR) + "/testfile2";
    QCOMPARE(linesCount(path), (unsigned long)3091);
}



QTEST_APPLESS_MAIN(LineCountTest)

#include "linecounttest.moc"
