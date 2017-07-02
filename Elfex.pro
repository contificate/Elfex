#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T23:57:40
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# linux dependencies
# TODO: create separate build targets (obscurecolin)
LIBS += -lcapstone -lpthread

TARGET = Elfex
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        elfexwindow.cc \
    main.cc \
    readers/filereader.cc \
    readers/memreader.cc \
    readers/elfreader.cc \
    disasm/disassembler.cc \
    disasm/armdisassembler.cc

HEADERS += \
        elfexwindow.h \
    readers/binaryreader.h \
    readers/filereader.h \
    readers/memreader.h \
    readers/elfreader.h \
    formats/elf.h \
    disasm/disassembler.h \
    disasm/arch/arm.h \
    disasm/exceptions/disasmexception.h \
    disasm/armdisassembler.h

FORMS += \
        elfexwindow.ui
