QT += quick
HEADERS = file_manager.hpp
SOURCES += main.cpp file_manager.cpp
distribute.path = /usr/share/quickfm
distribute.files = *.qml *.svgz
target.path = /usr/bin
INSTALLS += target distribute
CONFIG += c++11 debug qml_debug
