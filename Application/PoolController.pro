#------------------------------------------------
#
# Project created by QtCreator 2012-02-06T20:06:36
#
#-------------------------------------------------

QT       += core gui sql widgets serialport

TARGET = temperaturaPiscina
TEMPLATE = app

MOC_DIR     = build
RCC_DIR     = build
UI_DIR      = build
OBJECTS_DIR = build


SOURCES +=\
    src/gui/MainWindow.cpp \
    src/hardware/AbstractTemperaturesensor.cpp \
    src/hardware/Arduino.cpp \
    src/hardware/CpuTemperature.cpp \
    src/hardware/DialogAddCalibrationPoint.cpp \
    src/hardware/TemperatureSensor.cpp \
    src/HelperClasses/Logger.cpp \
    src/main.cpp

HEADERS  += \
    src/hardware/CpuTemperature.h \
    src/hardware/DialogAddCalibrationPoint.h \
    src/hardware/TemperatureSensor.h \
    src/hardware/Arduino.h \
    src/hardware/AbstractTemperaturesensor.h \
    src/gui/MainWindow.h \
    src/HelperClasses/Logger.h

FORMS    += \
    src/gui/MainWindow.ui \
    src/hardware/DialogAddCalibrationPoint.ui

DISTFILES += \
    scripts/caratteristica.py \
    ../Arduino/programma_arduino.pde

