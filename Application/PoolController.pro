#------------------------------------------------
#
# Project created by QtCreator 2012-02-06T20:06:36
#
#-------------------------------------------------

QT       += core gui sql

TARGET = temperaturaPiscina
TEMPLATE = app

MOC_DIR     = build
RCC_DIR     = build
UI_DIR      = build
OBJECTS_DIR = build


CONFIG += extserialport

SOURCES +=\
    src/gui/ComDialog.cpp \
    src/gui/MainWindow.cpp \
    src/hardware/AbstractTemperaturesensor.cpp \
    src/hardware/Arduino.cpp \
    src/hardware/CpuTemperature.cpp \
    src/hardware/DialogAddCalibrationpoint.cpp \
    src/hardware/TemperatureSensor.cpp \
    src/HelperClasses/Logger.cpp \
    src/main.cpp

HEADERS  += \
    src/gui/ComDialog.h \
    src/hardware/CpuTemperature.h \
    src/hardware/DialogAddCalibrationpoint.h \
    src/hardware/TemperatureSensor.h \
    src/hardware/Arduino.h \
    src/hardware/AbstractTemperaturesensor.h \
    src/gui/MainWindow.h \
    src/HelperClasses/Logger.h

FORMS    += \
    src/hardware/DialogAddCalibrationpoint.ui \
    src/gui/MainWindow.ui \
    src/gui/ComDialog.ui

