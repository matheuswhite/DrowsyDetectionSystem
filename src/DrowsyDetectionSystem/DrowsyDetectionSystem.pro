#-------------------------------------------------
#
# Project created by QtCreator 2017-11-08T10:40:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DrowsyDetectionSystem
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

# OpenCV paths for headers and libs
INCLUDEPATH += /usr/local/include/opencv

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgcodecs \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_videoio \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_flann
# --- ENDOF --- OpenCV paths for headers and libs

SOURCES += \
        main.cpp \
    facedetector.cpp \
    mouthdetector.cpp \
    yawningdetector.cpp \
    videoplayer.cpp

HEADERS += \
    facedetector.h \
    common.h \
    mouthdetector.h \
    yawningdetector.h \
    videoplayer.h
