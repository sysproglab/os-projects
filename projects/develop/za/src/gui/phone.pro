QT += gui qml quick core dbus widgets
CONFIG += c++11 qtquickcompiler

HEADERS += mainwindow.h

SOURCES += main.cpp mainwindow.cpp

OTHER_FILES = main.qml dialing.qml call.qml button.qml

RESOURCES += res.qrc

target.path = $$[QT_INSTALL_EXAMPLES]
sources.files = $$SOURCES $$HEADERS $$RESOURCES phone.pro
sources.path = $$[QT_INSTALL_EXAMPLES]
INSTALLS += target sources
