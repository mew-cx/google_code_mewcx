# $Id$

EXE = readdem
RAW = ${EXE}.raw
PNG = ${EXE}.png

INC_FILES = \
	../../include/mewcx/Version.h \
	../../include/mewcx/ll2utm.h \
	../../include/mewcx/usgs_seamless.h

SRC_FILES =  \
	../../include/mewcx/src/GridFloat.cpp \
	../../include/mewcx/src/Version.cpp \
	../../include/mewcx/src/ll2utm.cpp \
	../../include/mewcx/src/usgs_seamless.cpp

CPPFLAGS = -I ../../include


#all : ${EXE} ${PNG}

${EXE} : ${EXE}.cpp ${SRC_FILES}

${RAW} : ${EXE}
	${EXE}

${PNG} : ${RAW}
	convert -size 4051x4051 -depth 8 gray:$< $@

clean :
	-rm -f ${EXE} ${PNG} *.o *~

clobber : clean
	-rm -f ${RAW}

# vim: set sw=4 ts=8 ic ai:
