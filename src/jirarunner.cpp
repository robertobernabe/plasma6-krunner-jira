/*
    SPDX-FileCopyrightText: 2021 Vitalii Koreniev <nemish94@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "jirarunner.h"

// KF
#include <KConfigGroup>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KRunner/QueryMatch>
#include <QDesktopServices>
#include <QProcess>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

JiraRunner::JiraRunner(QObject *parent, const KPluginMetaData &metaData)
    : KRunner::AbstractRunner(parent, metaData)
{
    setMinLetterCount(3);
    reloadConfiguration();
}

JiraRunner::~JiraRunner() = default;

void JiraRunner::init()
{
    addSyntax(QStringLiteral("jira :q:"), i18n("Find Jira issue by key or search term"));
    addSyntax(QStringLiteral(":q:"), i18n("Open Jira issue by key (e.g. PROJ-123)"));
}

void JiraRunner::reloadConfiguration()
{
    const KConfigGroup grp = config();
    setBaseUrl(grp.readEntry(QStringLiteral("jiraUrl"), QString()));
}

void JiraRunner::setBaseUrl(const QString &url)
{
    m_jiraUrl = url.trimmed();
    if (!m_jiraUrl.isEmpty() && !m_jiraUrl.endsWith(QLatin1Char('/'))) {
        m_jiraUrl.append(QLatin1Char('/'));
    }
}

QString JiraRunner::baseUrl() const
{
    return m_jiraUrl;
}

QUrl JiraRunner::buildUrl(const QString &issueKey) const
{
    if (m_jiraUrl.isEmpty()) {
        return QUrl();
    }
    return QUrl(m_jiraUrl + issueKey);
}

QUrl JiraRunner::buildSearchUrl(const QString &searchTerm) const
{
    const QString term = searchTerm.trimmed();
    if (m_jiraUrl.isEmpty() || term.isEmpty()) {
        return QUrl();
    }
    QString rootUrl = m_jiraUrl;
    if (rootUrl.endsWith(QStringLiteral("browse/"), Qt::CaseInsensitive)) {
        rootUrl.chop(7);
    } else if (rootUrl.endsWith(QStringLiteral("browse"), Qt::CaseInsensitive)) {
        rootUrl.chop(6);
    }
    if (!rootUrl.endsWith(QLatin1Char('/'))) {
        rootUrl.append(QLatin1Char('/'));
    }
    QUrl url(rootUrl + QStringLiteral("secure/QuickSearch.jspa"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("searchString"), term);
    url.setQuery(query);
    return url;
}

void JiraRunner::match(KRunner::RunnerContext &context)
{
    const QString query = context.query().trimmed();
    if (query.isEmpty()) {
        return;
    }

    const QRegularExpression ticketRegex(QStringLiteral("\\b[A-Za-z]+-\\d+\\b"));

    // Check for trigger word "jira " (case-insensitive)
    static const QRegularExpression prefixRegex(QStringLiteral("^jira\\s+(.+)$"), QRegularExpression::CaseInsensitiveOption);
    const QRegularExpressionMatch prefixMatch = prefixRegex.match(query);

    if (prefixMatch.hasMatch()) {
        const QString rest = prefixMatch.captured(1).trimmed();
        if (rest.isEmpty()) {
            return;
        }

        // Check if rest is a standalone ticket key
        const QRegularExpressionMatch ticketMatch = ticketRegex.match(rest);
        if (ticketMatch.hasMatch() && ticketMatch.capturedLength() == rest.length()) {
            const QString ticketKey = ticketMatch.captured().toUpper();
            KRunner::QueryMatch match(this);
            match.setText(ticketKey);
            match.setData(buildUrl(ticketKey));
            if (m_jiraUrl.isEmpty()) {
                match.setSubtext(i18n("Jira URL is not configured. Click to configure."));
            }
            match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::High);
            context.addMatch(match);
        } else {
            // "jira <search term>"
            KRunner::QueryMatch match(this);
            match.setText(i18n("Search Jira for '%1'", rest));
            match.setData(buildSearchUrl(rest));
            if (m_jiraUrl.isEmpty()) {
                match.setSubtext(i18n("Jira URL is not configured. Click to configure."));
            }
            match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Moderate);
            context.addMatch(match);
        }
        return;
    }

    // Direct ticket key without prefix (e.g. "Check PROJ-123")
    const QRegularExpressionMatch directMatch = ticketRegex.match(query);
    if (directMatch.hasMatch()) {
        const QString ticketKey = directMatch.captured().toUpper();
        KRunner::QueryMatch match(this);
        match.setText(ticketKey);
        match.setData(buildUrl(ticketKey));
        if (m_jiraUrl.isEmpty()) {
            match.setSubtext(i18n("Jira URL is not configured. Click to configure."));
        }
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Low);
        context.addMatch(match);
    }
}

void JiraRunner::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match)
{
    Q_UNUSED(context)
    if (m_jiraUrl.isEmpty()) {
        QProcess::startDetached(QStringLiteral("kcmshell6"), {QStringLiteral("kcm_krunner_jirarunner")});
        return;
    }
    const QUrl targetUrl = match.data().toUrl();
    if (targetUrl.isValid()) {
        QDesktopServices::openUrl(targetUrl);
    } else {
        QDesktopServices::openUrl(buildUrl(match.text()));
    }
}

K_PLUGIN_CLASS_WITH_JSON(JiraRunner, "plasma-runner-jirarunner.json")

#include "jirarunner.moc"
