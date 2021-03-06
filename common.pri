TEMP = $$[QT_INSTALL_LIBS] libQtGui.prl
PRL  = $$[QT_INSTALL_LIBS] QtGui.framework/QtGui.prl
include($$join(TEMP, "/"))
include($$join(PRL, "/"))

contains(QMAKE_PRL_CONFIG, shared) {
    DEFINES += QT_SHARED
} else {
    DEFINES += QT_STATIC
    QTPLUGIN += qjpeg qgif qtiff qmng
}

MOC_DIR = ../../build
OBJECTS_DIR = ../../build
UI_DIR = ../../build

win32 {
    CONFIG += console
}

QT += webkit network xmlpatterns
