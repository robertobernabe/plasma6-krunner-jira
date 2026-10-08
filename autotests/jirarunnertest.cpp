#include <QTest>
#include <QUrl>
#include <KConfigGroup>
#include <KPluginMetaData>
#include <KRunner/RunnerContext>
#include <KSharedConfig>
#include "jirarunner.h"

class JiraRunnerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testDefaultUrlIsEmpty();
    void testUrlConstructionCustom();
    void testUrlConstructionWithoutSlash();
    void testMatchQueryWhenConfigured();
    void testMatchQueryWhenUnconfigured();
    void testNoMatchQuery();
    void testConfigReloading();
    void testPrefixedTicketQuery();
    void testPrefixedSearchQuery();
    void testPrefixedCaseInsensitive();
    void testUnconfiguredPrefixedSearch();
    void testSearchUrlConstruction();
};

void JiraRunnerTest::testDefaultUrlIsEmpty()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    QVERIFY(runner.baseUrl().isEmpty());
    QUrl url = runner.buildUrl(QStringLiteral("PROJ-123"));
    QVERIFY(url.isEmpty());
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

void JiraRunnerTest::testMatchQueryWhenConfigured()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    runner.setBaseUrl(QStringLiteral("https://mycompany.atlassian.net/browse/"));
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("Check ticket ABC-987 please"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 1);
    QCOMPARE(context.matches().constFirst().text(), QStringLiteral("ABC-987"));
    QVERIFY(context.matches().constFirst().subtext().isEmpty());
}

void JiraRunnerTest::testMatchQueryWhenUnconfigured()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("Check ticket ABC-987 please"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 1);
    QCOMPARE(context.matches().constFirst().text(), QStringLiteral("ABC-987"));
    QVERIFY(!context.matches().constFirst().subtext().isEmpty());
}

void JiraRunnerTest::testNoMatchQuery()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    runner.setBaseUrl(QStringLiteral("https://mycompany.atlassian.net/browse/"));
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("No ticket here"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 0);
}

void JiraRunnerTest::testConfigReloading()
{
    KConfigGroup grp = KSharedConfig::openConfig(QStringLiteral("krunnerrc"))->group(QStringLiteral("Runners")).group(QStringLiteral("jirarunner"));
    const QString previousValue = grp.readEntry(QStringLiteral("jiraUrl"), QString());
    grp.writeEntry(QStringLiteral("jiraUrl"), QStringLiteral("https://custom-jira.org/browse/"));
    grp.sync();

    // Use metadata with id "jirarunner"
    const QString jsonPath = QStringLiteral(TEST_SRC_DIR "/../src/plasma-runner-jirarunner.json");
    const KPluginMetaData md = KPluginMetaData::fromJsonFile(jsonPath);
    QVERIFY(md.isValid());
    QCOMPARE(md.pluginId(), QStringLiteral("jirarunner"));
    JiraRunner runner(nullptr, md);
    runner.reloadConfiguration();
    QCOMPARE(runner.baseUrl(), QStringLiteral("https://custom-jira.org/browse/"));
    QCOMPARE(runner.buildUrl(QStringLiteral("TEST-1")).toString(), QStringLiteral("https://custom-jira.org/browse/TEST-1"));

    // Cleanup
    if (previousValue.isEmpty()) {
        grp.deleteEntry(QStringLiteral("jiraUrl"));
    } else {
        grp.writeEntry(QStringLiteral("jiraUrl"), previousValue);
    }
    grp.sync();
}

void JiraRunnerTest::testPrefixedTicketQuery()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    runner.setBaseUrl(QStringLiteral("https://mycompany.atlassian.net/browse/"));
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("jira ABC-123"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 1);
    QCOMPARE(context.matches().constFirst().text(), QStringLiteral("ABC-123"));
    QVERIFY(context.matches().constFirst().subtext().isEmpty());
}

void JiraRunnerTest::testPrefixedSearchQuery()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    runner.setBaseUrl(QStringLiteral("https://mycompany.atlassian.net/browse/"));
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("jira login authentication error"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 1);
    QVERIFY(context.matches().constFirst().text().contains(QStringLiteral("login authentication error")));
    QVERIFY(context.matches().constFirst().subtext().isEmpty());
}

void JiraRunnerTest::testPrefixedCaseInsensitive()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    runner.setBaseUrl(QStringLiteral("https://mycompany.atlassian.net/browse/"));
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("JIRA PROJ-42"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 1);
    QCOMPARE(context.matches().constFirst().text(), QStringLiteral("PROJ-42"));
}

void JiraRunnerTest::testUnconfiguredPrefixedSearch()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    KRunner::RunnerContext context;
    context.setQuery(QStringLiteral("jira crash on startup"));
    runner.match(context);

    QCOMPARE(context.matches().count(), 1);
    QVERIFY(context.matches().constFirst().text().contains(QStringLiteral("crash on startup")));
    QVERIFY(!context.matches().constFirst().subtext().isEmpty());
}

void JiraRunnerTest::testSearchUrlConstruction()
{
    KPluginMetaData md;
    JiraRunner runner(nullptr, md);
    QVERIFY(runner.buildSearchUrl(QStringLiteral("some query")).isEmpty());

    runner.setBaseUrl(QStringLiteral("https://mycompany.atlassian.net/browse/"));
    QUrl url = runner.buildSearchUrl(QStringLiteral("login error"));
    // Expect base URL stripped of /browse/ or pointing to QuickSearch
    // e.g. https://mycompany.atlassian.net/secure/QuickSearch.jspa?searchString=login+error
    QVERIFY(url.toString().contains(QStringLiteral("QuickSearch.jspa")));
    QVERIFY(url.toString().contains(QStringLiteral("searchString=login")));
}

QTEST_MAIN(JiraRunnerTest)
#include "jirarunnertest.moc"
