/*
    SPDX-FileCopyrightText: 2026 Roberto Bernabe
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef JIRARUNNERCONFIG_H
#define JIRARUNNERCONFIG_H

#include <KCModule>

class QLineEdit;

class JiraRunnerConfig : public KCModule
{
    Q_OBJECT

public:
    explicit JiraRunnerConfig(QObject *parent, const KPluginMetaData &metaData);
    ~JiraRunnerConfig() override = default;

    void load() override;
    void save() override;
    void defaults() override;

private:
    QLineEdit *m_urlLineEdit = nullptr;
};

#endif
