QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += resources_big

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    evaluationcontext.cpp \
    expression.cpp \
    main.cpp \
    mainwindow.cpp \
    program.cpp \
    pseudoio.cpp \
    runtime.cpp \
    statement.cpp \
    tokenizer.cpp \
    utils.cpp

HEADERS += \
    evaluationcontext.h \
    exceptions.h \
    expression.h \
    mainwindow.h \
    program.h \
    pseudoio.h \
    runtime.h \
    statement.h \
    tokenizer.h \
    utils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
