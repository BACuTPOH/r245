QT = gui core
CONFIG += qt warn_on release console
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/settings_window.ui ui/main_window.ui ui/monitor_window.ui
HEADERS = src/settings_window.h src/main_window.h src/utils.h src/monitor_window.h
SOURCES = src/main.cpp \
 src/settings_window.cpp \
 src/main_window.cpp \
 src/utils.cpp \
 src/monitor_window.cpp
TEMPLATE = app
