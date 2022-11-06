cmake_minimum_required(VERSION 3.16)

project(learning_graph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS_DEBUG "-fsanitize=undefined,address -g3 -O0")

find_package(Qt6 REQUIRED
    Core
    Widgets
    Sql
    Svg
    LinguistTools
    Network
)

qt_standard_project_setup()

add_executable(learning_graph
    main.cpp
    MainWindow.cpp
    GlobalSignalHandler.cpp
    ComboboxIdDialog.cpp
    SmartListWidget.cpp
    ScaleSpinBox.cpp
    appendExtention.cpp

    db/init.cpp
    db/theme.cpp
    db/package.cpp
    db/graph.cpp
    db/graphNode.cpp
    db/themeEdge.cpp
    db/list.cpp

    ThemeContextMenu.cpp
    ThemeInfoDialog.cpp
    ThemeTab.cpp

    PackageComboBox.cpp
    PackageTab.cpp
    PackageInfoDialog.cpp

    GraphEditWidget.cpp
    GraphTab.cpp
    GraphNodeItem.cpp
    GraphScene.cpp
    GraphEdge.cpp
    GraphView.cpp
    GraphInfoDialog.cpp

    LearningListWidget.cpp

    filesystem/theme.cpp
    filesystem/package.cpp
    filesystem/graph.cpp
    filesystem/list.cpp

    GenerationOptionsDialog.cpp
    PackageGenerator.cpp
    DependencyDirectionDialog.cpp
)

set(LIBS
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Svg
    Qt6::Network
)

target_link_libraries(learning_graph ${LIBS})

qt_add_resources(learning_graph "res"
    PREFIX "/"
    BASE "img/"
    FILES
        "img/arrow.svg"
        "img/pointer.svg"
        "img/plus.svg"
        "img/cross.svg"
)

qt_add_translations(learning_graph
    TS_FILES localeRu.ts
    QM_FILES_OUTPUT_VARIABLE qm_files
)

# Tests

add_executable(tests
    db/theme.cpp
    db/package.cpp
    db/graph.cpp

    tests/tests.cpp
)

target_link_libraries(tests ${LIBS})

add_custom_command(
    TARGET tests
    POST_BUILD
    COMMAND ./tests
    WORKING_DIRECTORY .
)
