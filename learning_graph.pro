lessThan(QT_MAJOR_VERSION, 6): error("requires Qt6")

TARGET = learning_graph

QT += widgets sql

CONFIG += c++17
QMAKE_CXX="ccache distcc g++"
QMAKE_CC="ccache distcc gcc"

# Graphics
HEADERS += $$_PRO_FILE_PWD_/src/*.h
SOURCES += $$_PRO_FILE_PWD_/src/*.cpp
