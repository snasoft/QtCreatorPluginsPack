#include "functions.h"

#include <QtTest>

class BenchmarkTest : public QObject
{
    Q_OBJECT
    
public:
    BenchmarkTest();
    
private Q_SLOTS:
    void lineCountBenchmark();
};



BenchmarkTest::BenchmarkTest()
{
    qDebug() << "Working dir:" << QString(SRCDIR);
}



void BenchmarkTest::lineCountBenchmark()
{       
    unsigned long result = 0;

    QBENCHMARK {
        result = linesCount(QString(SRCDIR) + "/testfile");
    }
    QVERIFY(result == 3091);
}



QTEST_APPLESS_MAIN(BenchmarkTest)

#include "benchmarktest.moc"
