#include <QStringList>
#include <QtTest>

#include "iosystem/cqueryparser.h"

class TestsCorelib : public QObject
{
    Q_OBJECT

public:
    TestsCorelib();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testQueryParser_data();
    void testQueryParser();
};

TestsCorelib::TestsCorelib()
{
}

void TestsCorelib::initTestCase()
{
}

void TestsCorelib::cleanupTestCase()
{
}

void TestsCorelib::testQueryParser_data()
{
    QTest::addColumn<QString>("query");
    QTest::addColumn<QStringList>("queryList");

    QTest::newRow("Empty query") << QStringLiteral("") << (QStringList());

    QTest::newRow("Single command") << QStringLiteral("foo") << (QStringList()
                                                                 << QStringLiteral("foo"));

    QTest::newRow("Mixed case single command") << QStringLiteral("fOo")
                                               << (QStringList()
                                                   << QStringLiteral("foo"));

    QTest::newRow("Single command framed spaces") << QStringLiteral(" foo  ")
                                                  << (QStringList()
                                                      << QStringLiteral("foo"));

    QTest::newRow("Quotes operand mixed with simple") << QStringLiteral("foo bar \"123 baz\" 8")
                                                      << (QStringList()
                                                          << QStringLiteral("foo")
                                                          << QStringLiteral("bar")
                                                          << QStringLiteral("123 baz")
                                                          << QStringLiteral("8"));

    QTest::newRow("Variable separators length") << QStringLiteral("foo  bar   \"123 baz\"    8")
                                                << (QStringList()
                                                    << QStringLiteral("foo")
                                                    << QStringLiteral("bar")
                                                    << QStringLiteral("123 baz")
                                                    << QStringLiteral("8"));

    QTest::newRow("All options") << QStringLiteral("  foO  bAr   \"123 baz\"    8     ")
                                 << (QStringList()
                                     << QStringLiteral("foo")
                                     << QStringLiteral("bar")
                                     << QStringLiteral("123 baz")
                                     << QStringLiteral("8"));
}

void TestsCorelib::testQueryParser()
{
    QFETCH(QString, query);
    QFETCH(QStringList, queryList);

    bool err = false;
    QString errDesc;
    QVERIFY2(CQueryParser::pars(query, err, errDesc) == queryList, "Failure");
}

QTEST_APPLESS_MAIN(TestsCorelib)

#include "tst_testscorelib.moc"