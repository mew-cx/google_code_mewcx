# $Id$
# $URL$

EXE = readdem
RAW = ${EXE}.raw
PNG = ${EXE}.png

INC_PATH = ../../include
SRC_PATH = ${INC_PATH}/mewcx/src

SRC_FILES = \
	${EXE}.cpp \
	${SRC_PATH}/Coord.cpp \
	${SRC_PATH}/UsgsGridfloat.cpp \
	${SRC_PATH}/Version.cpp

CPPFLAGS = -I${INC_PATH}


#all : ${EXE} ${PNG}
all : ${EXE}

${EXE} : ${SRC_FILES}

${RAW} : ${EXE}
	${EXE}

${PNG} : ${RAW}
	convert -size 4051x4051 -depth 8 gray:$< $@

clean :
	-rm -f ${EXE} ${PNG} *.o *~

clobber : clean
	-rm -f ${RAW}

# vim: set sw=4 ts=8 ic ai:
