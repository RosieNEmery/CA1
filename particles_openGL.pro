TARGET=particles_openGL

OBJECTS_DIR=obj

QT+=gui opengl core

isEqual(QT_MAJOR_VERSION, 5) {
        cache()
        DEFINES +=QT5BUILD
}

MOC_DIR=moc

SOURCES+= $$PWD/src/NGLScene.cpp \
          $$PWD/src/main.cpp \
          $$PWD/src/particle.cpp \
          $$PWD/src/particleSystem.cpp \
          $$PWD/src/attractor.cpp

HEADERS+= $$PWD/include/NGLScene.h \
          $$PWD/include/particle.h \
          $$PWD/include/particleSystem.h \
          $$PWD/include/attractor.h

INCLUDEPATH +=./include

OTHER_FILES+= shaders/*.glsl \

CONFIG += console

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}
