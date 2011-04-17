# $Id$

EXE = readdem
RAW = ${EXE}.raw
PNG = ${EXE}.png

INC_FILES = \
	../../include/mewcx/Version.h \
	../../include/mewcx/Coord.h \
	../../include/mewcx/UsgsGridfloat.h

SRC_FILES =  \
	../../include/mewcx/src/Version.cpp \
	../../include/mewcx/src/Coord.cpp \
	../../include/mewcx/src/UsgsGridfloat.cpp

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
