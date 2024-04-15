QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arrow.cpp \
    board.cpp \
    gamewindow.cpp \
    homewindow.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    windialog.cpp \
    event.cpp

HEADERS += \
    arrow.h \
    board.h \
    gamewindow.h \
    homewindow.h \
    mainwindow.h \
    player.h \
    windialog.h \
    event.h

FORMS += \
    gamewindow.ui \
    homewindow.ui \
    mainwindow.ui \
    windialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
