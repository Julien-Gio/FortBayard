# configuration Qt
QT       += core gui opengl widgets
TEMPLATE  = app

# ajout des libs au linker
win32 {
    win32-msvc* {
        LIBS     += opengl32.lib glu32.lib
    } else {
        LIBS     += -lopengl32 -lglu32
    }
}
else {
        LIBS     += -lGL -lGLU
}


# nom de l'exe genere
TARGET 	  = FortBayard

# fichiers sources/headers/ressources
SOURCES += main.cpp myglwidget.cpp \
    maze.cpp \
    player.cpp
HEADERS += myglwidget.h \
    cell.h \
    maze.h \
    player.h
