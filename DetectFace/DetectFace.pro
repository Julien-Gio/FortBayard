QT       -= gui

TARGET = test_detectmotion
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        facehandler.cpp \
        main.cpp

HEADERS += \
    facehandler.h



INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core420 \
    -lopencv_highgui420 \
    -lopencv_imgproc420 \
    -lopencv_imgcodecs420 \
    -lopencv_videoio420 \
    -lopencv_features2d420 \
    -lopencv_calib3d420 \
    -lopencv_objdetect420
