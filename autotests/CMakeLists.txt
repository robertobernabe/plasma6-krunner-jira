add_executable(jirarunnertest
    jirarunnertest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/jirarunner.cpp
)

target_include_directories(jirarunnertest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

target_compile_definitions(jirarunnertest PRIVATE
    TEST_SRC_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
    TRANSLATION_DOMAIN="plasma_runner_org.kde.jirarunner"
)

target_link_libraries(jirarunnertest
    Qt6::Test
    Qt6::Gui
    KF6::Runner
    KF6::I18n
    KF6::ConfigCore
)

add_test(NAME jirarunnertest COMMAND jirarunnertest)
