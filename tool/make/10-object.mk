SOURCE.orig.cxx := $(wildcard ${SOURCE.dir}/*.cpp ${SOURCE.dir}/*.c++ ${SOURCE.dir}/*.C)
SOURCE.orig.c := $(wildcard ${SOURCE.dir}/*.c)
SOURCE.orig := $(SOURCE.orig.c) $(SOURCE.orig.cxx)
SOURCE.orig := $(SOURCE.orig:${SOURCE.dir}/%=%)
SOURCE.orig := ${SOURCE.orig:.c=.o}
SOURCE.orig := ${SOURCE.orig:.cpp=.o}
SOURCE.orig := ${SOURCE.orig:.c++=.o}
SOURCE.orig := ${SOURCE.orig:.C=.o}
OBJECT.orig := $(addprefix ${OBJECT.dir}/, ${SOURCE.orig})

CPPFLAGS += -I${OBJECT.dir}

vpath %.c ${SOURCE.dir}
vpath %.cpp ${SOURCE.dir}
vpath %.c++ ${SOURCE.dir}
vpath %.C ${SOURCE.dir}

# this file / 99-all.mk is designed for a single target executable object, not library generation.
# Therefore, we'll call linkage exactly once and I can avoid touching Bourne Shell.
${TARGET}: ${OBJECT.orig}
	@echo "LD	$@"
ifeq (${SOURCE.orig.cxx},)
	@${LINK.c} -o "$@" $+
else
	@${LINK.cpp} -o "$@" $+
endif

${OBJECT.dir}/%.o: %.c
	@echo "CC	$<"
	@${COMPILE.c} -o $@ $<

# C++

${OBJECT.dir}/%.o: %.cpp
	@echo "C++	$<"
	@${COMPILE.cpp} -o $@ $<

${OBJECT.dir}/%.o: %.c++
	@echo "C++	$<"
	@${COMPILE.cpp} -o $@ $<

${OBJECT.dir}/%.o: %.C
	@echo "C++	$<"
	@${COMPILE.cpp} -o $@ $<
