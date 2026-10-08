add_definitions(-DTRANSLATION_DOMAIN=\"plasma_runner_org.kde.jirarunner\")

kcoreaddons_add_plugin(jirarunner SOURCES
    jirarunner.cpp
    INSTALL_NAMESPACE "kf6/krunner"
)

target_link_libraries(jirarunner
    KF6::Runner
    KF6::I18n
    Qt6::Gui
    KF6::ConfigCore
)

kcoreaddons_add_plugin(kcm_krunner_jirarunner SOURCES
    jirarunnerconfig.cpp
    INSTALL_NAMESPACE "kf6/krunner/kcms"
)

target_link_libraries(kcm_krunner_jirarunner
    KF6::KCMUtils
    KF6::I18n
    KF6::ConfigCore
    Qt6::Widgets
)
