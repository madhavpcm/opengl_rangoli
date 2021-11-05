include(openglwindow.pri)
include(./include/qtimgui/qtimgui.pri)
CONFIG += console

SOURCES += \
    BSplineWindow.cpp \
    curveobject.cpp \
    main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/gui/openglwindow
INSTALLS += target

INCLUDEPATH += $$PWD/include

RESOURCES += \
    opengl.qrc

HEADERS += \
    BSplineWindow.h \
    curveobject.h
