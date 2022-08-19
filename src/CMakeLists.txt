cmake_minimum_required(VERSION 3.16)

project(learning_graph LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

find_package(Qt6 REQUIRED
    Core
    Widgets
    Sql
    Svg
)

qt_standard_project_setup()

add_executable(learning_graph
    main.cpp
    dbLogics.cpp
    GlobalSignalHandler.cpp
    MainWindow.cpp

    ThemeContextMenu.cpp
    ThemeInfoWindow.cpp
    ThemeListWidget.cpp
    ThemeTab.cpp

    PackageComboBox.cpp
    PackageListWidget.cpp
    PackageTab.cpp

    GraphEditTab.cpp
    GraphListWidget.cpp
    GraphTab.cpp
    GraphNodeWidget.cpp

    LearningListTab.cpp
)

target_link_libraries(learning_graph
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Svg
)

qt_add_resources(learning_graph "res"
    PREFIX "/"
    BASE "res/"
    FILES
        "res/arrow.svg"
        "res/pointer.svg"
        "res/plus.svg"
)
