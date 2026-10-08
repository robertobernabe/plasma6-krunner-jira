/*
    SPDX-FileCopyrightText: 2026 Roberto Bernabe
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "jirarunnerconfig.h"

#include <KConfigGroup>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KSharedConfig>

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

JiraRunnerConfig::JiraRunnerConfig(QObject *parent, const KPluginMetaData &metaData)
    : KCModule(qobject_cast<QWidget *>(parent), metaData)
{
    auto layout = new QFormLayout(widget());
    m_urlLineEdit = new QLineEdit(widget());
    m_urlLineEdit->setPlaceholderText(QStringLiteral("https://your-company.atlassian.net/browse/"));
    m_urlLineEdit->setClearButtonEnabled(true);

    auto explanation = new QLabel(i18n("Base URL for Jira tickets (e.g. https://your-company.atlassian.net/browse/)"), widget());
    explanation->setWordWrap(true);

    layout->addRow(i18n("Jira URL:"), m_urlLineEdit);
    layout->addRow(QString(), explanation);

    connect(m_urlLineEdit, &QLineEdit::textChanged, this, &JiraRunnerConfig::markAsChanged);
}

void JiraRunnerConfig::load()
{
    KConfigGroup grp = KSharedConfig::openConfig(QStringLiteral("krunnerrc"))->group(QStringLiteral("Runners")).group(QStringLiteral("jirarunner"));
    m_urlLineEdit->setText(grp.readEntry(QStringLiteral("jiraUrl"), QString()));
    setNeedsSave(false);
}

void JiraRunnerConfig::save()
{
    KConfigGroup grp = KSharedConfig::openConfig(QStringLiteral("krunnerrc"))->group(QStringLiteral("Runners")).group(QStringLiteral("jirarunner"));
    grp.writeEntry(QStringLiteral("jiraUrl"), m_urlLineEdit->text().trimmed());
    grp.sync();
    setNeedsSave(false);
}

void JiraRunnerConfig::defaults()
{
    m_urlLineEdit->clear();
    markAsChanged();
}

K_PLUGIN_CLASS_WITH_JSON(JiraRunnerConfig, "kcm_krunner_jirarunner.json")

#include "jirarunnerconfig.moc"
