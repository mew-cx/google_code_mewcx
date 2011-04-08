# $Id$

EXE = readdem
RAW = ${EXE}.raw
PNG = ${EXE}.png

#all : ${EXE} ${PNG}

${EXE} : ${EXE}.cpp ll2utm.cpp

${RAW} : ${EXE}
	${EXE}

${PNG} : ${RAW}
	convert -size 4051x4051 -depth 8 gray:$< $@

clean :
	-rm -f ${EXE} ${PNG} *.o *~

clobber : clean
	-rm -f ${RAW}

# EOF
