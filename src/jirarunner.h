/*
    SPDX-FileCopyrightText: 2021 Vitalii Koreniev <nemish94@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef JIRARUNNER_H
#define JIRARUNNER_H

#include <KRunner/AbstractRunner>
#include <QUrl>

class JiraRunner : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    explicit JiraRunner(QObject *parent, const KPluginMetaData &metaData);
    ~JiraRunner() override;

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;

    void reloadConfiguration() override;

    QUrl buildUrl(const QString &issueKey) const;
    void setBaseUrl(const QString &url);
    QString baseUrl() const;

private:
    QString m_jiraUrl;
};

#endif
