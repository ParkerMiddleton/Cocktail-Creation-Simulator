QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = barblunder.ico

SOURCES += \
    applicationmodel.cpp \
    gamelayer.cpp \
    main.cpp \
    mainmenupage.cpp \
    mainwindow.cpp \
    barmodel.cpp \
    recipe.cpp \
    settingsmenupage.cpp

HEADERS += \
    applicationmodel.h \
    barmodel.h \
    gamelayer.h \
    mainmenupage.h \
    mainwindow.h \
    qt_pch.h \
    recipe.h \
    settingsmenupage.h

FORMS += \
    gamelayer.ui \
    mainmenupage.ui \
    mainwindow.ui \
    settingsmenupage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    gamegraphics.qrc \
    menugraphics.qrc \
    music.qrc \
    sounds.qrc \
    misc.qrc

PRECOMPILED_HEADER += \
    qt_pch.h

DISTFILES += \
    uml.qmodel
