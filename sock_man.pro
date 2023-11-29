QT       += core gui sql
QT += core-private
QT       += sql
#QT += xlsx
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = exe.ico
# RC_FILE += logo.rc


CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addsock.cpp \
    adduser.cpp \
    boss_select.cpp \
    help.cpp \
    main.cpp \
    mainwindow.cpp \
    modsock.cpp \
    moduser.cpp \
    selectsock.cpp \
    signin.cpp \
    sock_sql.cpp \
    sockman_work.cpp \
    user_sql.cpp \
    userman_work.cpp

HEADERS += \
    addsock.h \
    adduser.h \
    boss_select.h \
    help.h \
    mainwindow.h \
    modsock.h \
    moduser.h \
    selectsock.h \
    signin.h \
    sock_sql.h \
    sockman_work.h \
    user_sql.h \
    userman_work.h

FORMS += \
    addsock.ui \
    adduser.ui \
    boss_select.ui \
    help.ui \
    mainwindow.ui \
    modsock.ui \
    moduser.ui \
    selectsock.ui \
    signin.ui \
    sockman_work.ui \
    userman_work.ui



include(qtxlsx/src/xlsx/qtxlsx.pri)


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
RESOURCES += \
        qt.conf

DISTFILES +=



# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target
