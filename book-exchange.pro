QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminauthors.cpp \
    adminmenu.cpp \
    authcontroller.cpp \
    connectioncontroller.cpp \
    dbtable.cpp \
    forms.cpp \
    literaturebooks.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    adminauthors.h \
    adminmenu.h \
    authcontroller.h \
    connectioncontroller.h \
    dbtable.h \
    forms.h \
    literaturebooks.h \
    login.h \
    mainwindow.h

FORMS += \
    adminauthors.ui \
    adminmenu.ui \
    literaturebooks.ui \
    login.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
