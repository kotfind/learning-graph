TARGET = learning_graph

QT += widgets sql

CONFIG += c++17
QMAKE_CXX="ccache distcc g++"
QMAKE_CC="ccache distcc gcc"

# Main
SOURCES += $$_PRO_FILE_PWD_/src/main.cpp

# Graphics
HEADERS += $$_PRO_FILE_PWD_/src/graphics/*.h
SOURCES += $$_PRO_FILE_PWD_/src/graphics/*.cpp

# Logic
HEADERS += $$_PRO_FILE_PWD_/src/logics/*.h
SOURCES += $$_PRO_FILE_PWD_/src/logics/*.cpp
