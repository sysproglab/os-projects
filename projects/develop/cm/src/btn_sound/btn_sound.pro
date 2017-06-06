QT += core dbus 
QT -= gui
CONFIG += c++11 qtquickcompiler warn_off


HEADERS += SetProperty/SetProperty.h \


SOURCES += btn_sound.cpp \
		   GPIOController/GPIOController.cpp \
    	   SetProperty/SetProperty.cpp \



target.path = $$[QT_INSTALL_EXAMPLES]
sources.files = $$SOURCES $$HEADERS btn_sound.pro
sources.path = $$[QT_INSTALL_EXAMPLES]
INSTALLS += target sources
