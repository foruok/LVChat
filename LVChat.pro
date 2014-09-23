TEMPLATE = app

QT += qml quick network multimedia

SOURCES += main.cpp \
    audiorecorder.cpp \
    messengerConnection.cpp \
    scanner.cpp \
    accessPointModel.cpp \
    pointSizeToPixelSize.cpp \
    ../stockMonitor/qDebug2Logcat.cpp \
    messengerManager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    android/AndroidManifest.xml

HEADERS += \
    audiorecorder.h \
    messengerConnection.h \
    protocol.h \
    scanner.h \
    voiceMessage.h \
    accessPointModel.h \
    pointSizeToPixelSize.h \
    ../stockMonitor/qDebug2Logcat.h \
    messengerManager.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
