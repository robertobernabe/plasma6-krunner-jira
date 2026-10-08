#include <QTest>
#include <QUrl>
#include <KPluginMetaData>
#include <KRunner/RunnerContext>
#include "jirarunner.h"

class JiraRunnerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testUrlConstructionDefault();
    void testUrlConstructionCustom();
    void testUrlConstructionWithoutSlash();
    void testMatchQuery();
    void testNoMatchQuery();
};

void JiraRunnerTest::testUrlConstructionDefault()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    // Default URL is https://dermpro.atlassian.net/browse/
    QUrl url = runner.buildUrl(QStringLiteral("PROJ-123"));
    QCOMPARE(url.toString(), QStringLiteral("https://dermpro.atlassian.net/browse/PROJ-123"));
}

void JiraRunnerTest::testUrlConstructionCustom()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    runner.setBaseUrl(QStringLiteral("https://mycompany.atlassian.net/browse/"));
    QUrl url = runner.buildUrl(QStringLiteral("FOO-42"));
    QCOMPARE(url.toString(), QStringLiteral("https://mycompany.atlassian.net/browse/FOO-42"));
}

void JiraRunnerTest::testUrlConstructionWithoutSlash()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    runner.setBaseUrl(QStringLiteral("https://mycompany.atlassian.net/browse"));
    QUrl url = runner.buildUrl(QStringLiteral("FOO-42"));
    QCOMPARE(url.toString(), QStringLiteral("https://mycompany.atlassian.net/browse/FOO-42"));
}

void JiraRunnerTest::testMatchQuery()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("Check ticket ABC-987 please"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 1);
    QCOMPARE(context.matches().constFirst().text(), QStringLiteral("ABC-987"));
}

void JiraRunnerTest::testNoMatchQuery()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("No ticket here"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 0);
}

QTEST_MAIN(JiraRunnerTest)
#include "jirarunnertest.moc"
