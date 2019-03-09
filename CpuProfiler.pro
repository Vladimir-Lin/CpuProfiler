QT          -= gui

CONFIG      += c++11 console
CONFIG      -= app_bundle

DEFINES     += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $${PWD}/include

HEADERS     += $${PWD}/include/CpuProfiler.hpp

SOURCES     += $${PWD}/srcs/CpuProfiler.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OTHER_FILES += $${PWD}/CMakeLists.txt
