SOURCES = main.cpp \
    musicplayer.cpp
LIBS = -lmpdclient

FORMS += \
    musicplayer.ui

HEADERS += \
    musicplayer.h
QT += widgets

TARGET = MusicPlayer
