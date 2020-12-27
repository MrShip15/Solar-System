QT       += core gui xml
QT       += testlib core gui

LIBS += -lopengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CameraRotator.cpp \
    Planet.cpp \
    asteroid.cpp \
    main.cpp \
    Widget.cpp \
    satellite.cpp \
    test_planet.cpp

HEADERS += \
    CameraRotator.h \
    Planet.h \
    Widget.h \
    asteroid.h \
    satellite.h \
    test_planet.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Models.qrc \
    Shaders.qrc \
    Textures.qrc
