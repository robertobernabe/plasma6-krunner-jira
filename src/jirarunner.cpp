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

JiraRunner::JiraRunner(QObject *parent, const KPluginMetaData &metaData)
    : KRunner::AbstractRunner(parent, metaData)
{
    setMinLetterCount(4);
    reloadConfiguration();
}

JiraRunner::~JiraRunner() = default;

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

void JiraRunner::match(KRunner::RunnerContext &context)
{
    const QString term = context.query();
    const QRegularExpression regex(QStringLiteral("\\w+-\\d+"));
    const QRegularExpressionMatch termMatch = regex.match(term);
    if (termMatch.hasMatch()) {
        KRunner::QueryMatch match(this);
        match.setText(termMatch.captured());
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
    QDesktopServices::openUrl(buildUrl(match.text()));
}

K_PLUGIN_CLASS_WITH_JSON(JiraRunner, "plasma-runner-jirarunner.json")

#include "jirarunner.moc"
