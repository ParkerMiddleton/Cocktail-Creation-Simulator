QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gamepage.cpp \
    main.cpp \
    mainmenupage.cpp \
    mainwindow.cpp \
    barmodel.cpp \
    recipe.cpp

HEADERS += \
    barmodel.h \
    gamepage.h \
    mainmenupage.h \
    mainwindow.h \
    qt_pch.h \
    recipe.h

FORMS += \
    gamepage.ui \
    mainmenupage.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc \
    sounds.qrc

PRECOMPILED_HEADER += \
    qt_pch.h

DISTFILES += \
    drinks
