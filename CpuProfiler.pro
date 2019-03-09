QT          -= gui

TARGET       = CpuProfiler
TEMPLATE     = app

CONFIG      += c++11
CONFIG      += console
CONFIG      -= app_bundle

CONFIG      += link_pkgconfig

PKGCONFIG   += UUIDs
PKGCONFIG   += libstardate
PKGCONFIG   += libparallel

DEFINES     += QT_DEPRECATED_WARNINGS

INCLUDEPATH += $${PWD}/include

HEADERS     += $${PWD}/include/CpuProfiler.hpp

SOURCES     += $${PWD}/srcs/CpuProfiler.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OTHER_FILES += $${PWD}/CMakeLists.txt
OTHER_FILES += $${PWD}/VERSION.txt

OTHER_FILES += $${PWD}/resources/CpuProfiler.rc
