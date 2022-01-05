QT       += core gui
QT       += network ## 网络模块

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


LIBS += -lVersion

RC_ICONS = BiliBang.ico

VERSION = 1.1.2.3
QMAKE_TARGET_PRODUCT = "BiliBang"
QMAKE_TARGET_DESCRIPTION = "BiliBili新番时间线"
QMAKE_TARGET_COPYRIGHT = "Copyright GiteeLou. All rights reserved."
RC_LANG = 0x0004

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    floatwindow.cpp \
    httprequest.cpp \
    labelpanel.cpp \
    main.cpp \
    mainwindow.cpp \
    windesktop.cpp

HEADERS += \
    bang.h \
    floatwindow.h \
    httprequest.h \
    labelpanel.h \
    mainwindow.h \
    windesktop.h

#TRANSLATIONS += \
#    BiliBang_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
