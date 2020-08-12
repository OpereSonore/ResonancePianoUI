QT += core qml quick multimedia serialport

CONFIG += c++11

SOURCES += main.cpp \
    mediaplayer.cpp \
    mediaplayerdata.cpp \
    fatalerror.cpp \
    logger.cpp \
    usbmountpoint.cpp \
    plnet.cpp \
    equalizer.cpp \
    modulename.cpp \
    musicstand.cpp \
    updatehandler.cpp \
    crc.cpp \
    gpio.cpp \
    powersupply.cpp

HEADERS += \
    mediaplayer.h \
    mediaplayerdata.h \
    global.h \
    fatalerror.h \
    logger.h \
    usbmountpoint.h \
    common/H/pl_net_structs.h \
    plnet.h \
    equalizer.h \
    modulename.h \
    common/H/pl_net_structs.h \
    musicstand.h \
    updatehandler.h \
    crc.h \
    debugbreak.h \
    gpio.h \
    powersupply.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

VERSION = 01.02.00
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# RP01 -> Resonance Piano v01
# RF01 -> Resonane Fonico v01
MODEL_NAME = RP01
DEFINES += APP_MODEL_NAME=\\\"$$MODEL_NAME\\\"

target.path = /home/pi/pianoLight
INSTALLS += target

