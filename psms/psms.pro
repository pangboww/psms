#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T22:58:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = psms
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addproductdialog.cpp \
    addproviderdialog.cpp \
    providertablemodel.cpp \
    saletablemodel.cpp \
    purchasetablemodel.cpp \
    producttablemodel.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    initdb.h \
    addproductdialog.h \
    addproviderdialog.h \
    providertablemodel.h \
    saletablemodel.h \
    purchasetablemodel.h \
    producttablemodel.h \
    qcustomplot.h

FORMS    += mainwindow.ui \
    addproductdialog.ui \
    addproviderdialog.ui
