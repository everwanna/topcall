TEMPLATE = lib
CONFIG -= qt
#CONFIG += staticlib
CONFIG += debug_and_release

PRECOMPILED_HEADER = stdafx.h

INCLUDEPATH += ./

DEFINES += _USRDLL
DEFINES += _NOT_INCLUDE_BOOST_BASIC_HEADERS
DEFINES += _NOT_INCLUDE_TOOL_COMMON_HEADERS

win32{
include(../../include/common.pri)
QMAKE_CXXFLAGS += /EHsc

INCLUDEPATH += ../../../depends/dw3rd/openssl-1.0.0d/include
QMAKE_LIBDIR += ../../../depends/dw3rd/openssl-1.0.0d/lib/release

LIBS += ws2_32.lib
LIBS += wininet.lib
LIBS += dwutility.lib
LIBS += dwbase.lib
LIBS += winmm.lib
}

unix {
DESTDIR = ../../../bin/

LIBS += -lssl
LIBS += -lcrypto
}

HEADERS += 	\
	netlog.h	\
	blockbuf.h 	\
	conn.h 		\
	connmgr.h 	\
	extension.h \
	ilinklayer.h\
	ioengine.h 	\
	linklayerenc.h \
	linklayerproxy.h \
	mutex.h 	\
	mempool.h 	\
	netmod.h

SOURCES += 		\
	dllmain.cpp \
	conn.cpp 	\
	connmgr.cpp \
	ilinklayer.cpp \
	ioengine.cpp \
	linklayerenc.cpp \
	linklayerproxy.cpp \
	mutex.cpp 	\
	mempool.cpp \
	netmod.cpp 
