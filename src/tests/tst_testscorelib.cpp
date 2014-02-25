#include <QStringList>
#include <QtTest>

#include "command/ccmdparser.h"
#include "command/ccommand.h"
#include "command/ccommanddsp.h"

#include "cmockcmdreceiver.h"

using namespace gccore;

class TestsCorelib : public QObject
{
    Q_OBJECT
private:
    CMockCmdReceiver *m_mockCmdReceiver;
    CCommand *m_cmdNoArgs;
    CCommand *m_cmd2Args;
    CCommandDsp *m_cmdDispatcher;

    void initCommandsTestColumns();
    void commandsTestSet1();
    void commandsTestSet2();
public:
    TestsCorelib();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testQueryParser_data();
    void testQueryParser();

    void testCommand_data();
    void testCommand();

    void testCommandDsp_data();
    void testCommandDsp();
};

void TestsCorelib::initCommandsTestColumns()
{
    QTest::addColumn<CCommand*>("cmd");
    QTest::addColumn<QString>("cmdString");
    QTest::addColumn<int>("errorCode");
    QTest::addColumn<QString>("respons");
    QTest::addColumn<bool>("result");
}

void TestsCorelib::commandsTestSet1()
{
    QTest::newRow("Not args") << m_cmdNoArgs << "cmdNoArgs" << 0 << QString("Success") << true;
    QTest::newRow("Not args falt") << m_cmdNoArgs << "cmdNoArgs 1" << 4 << QString() << false;
    QTest::newRow("2 args (QString, int)") << m_cmd2Args << "cmd2Args \"arg1\" 2" << 0
                                           << QString("Success: arg1 2") << true;
    QTest::newRow("2 args (QString, int) falt (num param)") << m_cmd2Args << "cmd2Args \"arg1\""
                                                            << 4 << QString() << false;
    QTest::newRow("2 args (QString, int) falt (type param)") << m_cmd2Args
                                                             << "cmd2Args 2 \"arg1\"" << 5
                                                             << QString() << false;
}

void TestsCorelib::commandsTestSet2()
{
    QTest::newRow("3 args (bool, qreal, QString)") << m_cmdDispatcher->command("cmd3Args")
                                                   << QStringLiteral("cmd3Args true 0.01"
                                                                     " \"Some string\"")
                                                   << (int)ERR_NOT_ERROR
                                                   << QStringLiteral("Success: true 0.01"
                                                                     " Some string")
                                                   << true;
}

TestsCorelib::TestsCorelib()
{
    m_mockCmdReceiver = nullptr;

    m_cmdNoArgs = nullptr;
    m_cmd2Args = nullptr;

    m_cmdDispatcher = nullptr;
}

void TestsCorelib::initTestCase()
{
    m_mockCmdReceiver = new CMockCmdReceiver(this);

    m_cmdNoArgs = new CCommand(m_mockCmdReceiver, SLOT(testCmdNoArgs(gccore::CCommand*)),
                               QStringLiteral("cmdNoArgs"), this);
    m_cmd2Args = new CCommand(m_mockCmdReceiver, SLOT(testCmd2Args(gccore::CCommand*,QString,int)),
                              QStringLiteral("cmd2Args"), this);

    m_cmdDispatcher = new CCommandDsp(this);
    m_cmdDispatcher->registerCommand(m_cmdNoArgs);
    m_cmdDispatcher->registerCommand(m_cmd2Args);

    m_cmdDispatcher->registerCommand(m_mockCmdReceiver,
                                     SLOT(testCmd3Args(gccore::CCommand*,bool,qreal,QString)),
                                     QStringLiteral("cmd3Args"));
}

void TestsCorelib::cleanupTestCase()
{
    delete m_mockCmdReceiver;
    m_mockCmdReceiver = nullptr;

    delete m_cmdNoArgs;
    m_cmdNoArgs = nullptr;
    delete m_cmd2Args;
    m_cmd2Args = nullptr;

    delete m_cmdDispatcher;
    m_cmdDispatcher = nullptr;
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

    QTest::newRow("Variable case in string") << QStringLiteral("foo bar \"123 BaZ\" 8")
                                             << (QStringList()
                                                 << QStringLiteral("foo")
                                                 << QStringLiteral("bar")
                                                 << QStringLiteral("123 BaZ")
                                                 << QStringLiteral("8"));

    QTest::newRow("Variable spaces in string") << QStringLiteral("foo bar \"  123   baz \" 8")
                                               << (QStringList()
                                                   << QStringLiteral("foo")
                                                   << QStringLiteral("bar")
                                                   << QStringLiteral("  123   baz ")
                                                   << QStringLiteral("8"));

    QTest::newRow("No space before string") << QStringLiteral("foo bar\"123 baz\" 8")
                                            << (QStringList()
                                                << QStringLiteral("foo")
                                                << QStringLiteral("bar")
                                                << QStringLiteral("123 baz")
                                                << QStringLiteral("8"));

    QTest::newRow("No space after string") << QStringLiteral("foo bar \"123 baz\"8")
                                           << (QStringList()
                                               << QStringLiteral("foo")
                                               << QStringLiteral("bar")
                                               << QStringLiteral("123 baz")
                                               << QStringLiteral("8"));

    QTest::newRow("2 string") << QStringLiteral("foo bar \"123 baz\" 8 \"qwerty\"")
                              << (QStringList()
                                  << QStringLiteral("foo")
                                  << QStringLiteral("bar")
                                  << QStringLiteral("123 baz")
                                  << QStringLiteral("8")
                                  << QStringLiteral("qwerty"));

    QTest::newRow("No spaces before|after 2 string") << QStringLiteral("foo \"123 baz\"8\"qwerty\"")
                                                     << (QStringList()
                                                         << QStringLiteral("foo")
                                                         << QStringLiteral("123 baz")
                                                         << QStringLiteral("8")
                                                         << QStringLiteral("qwerty"));

    QTest::newRow("All options") << QStringLiteral("  foO  bAr   \"123  bAz  \"    8     "
                                                   "\" QuErTy \"")
                                 << (QStringList()
                                     << QStringLiteral("foo")
                                     << QStringLiteral("bar")
                                     << QStringLiteral("123  bAz  ")
                                     << QStringLiteral("8")
                                     << QStringLiteral(" QuErTy "));
}

void TestsCorelib::testQueryParser()
{
    QFETCH(QString, query);
    QFETCH(QStringList, queryList);

    bool err = false;
    QString errDesc;
    QVERIFY2(CCmdParser::pars(query, err, errDesc) == queryList, "Failure");
}

void TestsCorelib::testCommand_data()
{
    initCommandsTestColumns();
    commandsTestSet1();
}

void TestsCorelib::testCommand()
{
    QFETCH(CCommand*, cmd);
    QFETCH(QString, cmdString);
    QFETCH(int, errorCode);
    QFETCH(QString, respons);
    QFETCH(bool, result);

    bool parsError = 0;
    QString parsErrDesc;
    int tstErrCode = ERR_NOT_ERROR;
    QString tstError = ERR_NOT_ERROR_STR;

    QStringList cmdList = CCmdParser::pars(cmdString, parsError, parsErrDesc);
    bool tstResult = cmd->execute(cmdList, tstErrCode, tstError);

    QVERIFY2(result == tstResult, "Result is failure");
    QVERIFY2(errorCode == tstErrCode, "Err code is failure");
    QVERIFY2(respons == cmd->respons(), "Respons is failure");
}

void TestsCorelib::testCommandDsp_data()
{
    initCommandsTestColumns();
    commandsTestSet1();
    commandsTestSet2();
}

void TestsCorelib::testCommandDsp()
{
    QFETCH(QString, cmdString);
    QFETCH(int, errorCode);
    QFETCH(QString, respons);
    QFETCH(bool, result);

    int tstErrCode = ERR_NOT_ERROR;
    QString tstError = ERR_NOT_ERROR_STR;

    QString tstRespons;
    bool tstResult = m_cmdDispatcher->execute(QString(), cmdString, tstRespons, tstErrCode, tstError);

    QVERIFY2(result == tstResult, "Result is failure");
    QVERIFY2(errorCode == tstErrCode, "Err code is failure");
    QVERIFY2(respons == tstRespons, "Respons is failure");
}

QTEST_APPLESS_MAIN(TestsCorelib)

#include "tst_testscorelib.moc"