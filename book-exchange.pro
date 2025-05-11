QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminauthors.cpp \
    admingenres.cpp \
    adminliterature.cpp \
    adminmenu.cpp \
    authcontroller.cpp \
    connectioncontroller.cpp \
    dbtable.cpp \
    forms.cpp \
    literaturebooks.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    usermenu.cpp \
    usermybooks.cpp \
    useroffers.cpp

HEADERS += \
    adminauthors.h \
    admingenres.h \
    adminliterature.h \
    adminmenu.h \
    authcontroller.h \
    connectioncontroller.h \
    dbtable.h \
    forms.h \
    literaturebooks.h \
    login.h \
    mainwindow.h \
    usermenu.h \
    usermybooks.h \
    useroffers.h

FORMS += \
    adminauthors.ui \
    admingenres.ui \
    adminliterature.ui \
    adminmenu.ui \
    literaturebooks.ui \
    login.ui \
    mainwindow.ui \
    usermenu.ui \
    usermybooks.ui \
    useroffers.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
