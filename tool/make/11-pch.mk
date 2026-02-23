HEADER.pch.filter ?=
HEADER.orig.cxx := $(wildcard ${SOURCE.dir}/*.H ${OBJECT.dir}/*.H ${SOURCE.dir}/*.hpp ${OBJECT.dir}/*.hpp ${SOURCE.dir}/*.h++ ${OBJECT.dir}/*.h++)
HEADER.orig.c   := $(wildcard ${SOURCE.dir}/*.h ${OBJECT.dir}/*.h)
HEADER.orig := $(filter-out ${HEADER.pch.filter},${HEADER.orig.c} ${HEADER.orig.cxx})
HEADER.orig := $(HEADER.orig:${SOURCE.dir}/%=%)
HEADER.orig := $(HEADER.orig:${OBJECT.dir}/%=%)

HEADER.pch := $(addprefix ${OBJECT.dir}/, ${HEADER.orig})

ifeq (${CC},gcc)
        HEADER.pch := $(addsuffix .gch,${HEADER.pch})
else
        HEADER.pch := $(addsuffix .pch,${HEADER.pch})
endif

ALSO       += ${HEADER.pch}

vpath %.h ${SOURCE.dir} ${OBJECT.dir}

${OBJECT.dir}/%.h.pch: %.h
	@echo "PCH	$<"
	@${COMPILE.c} ${PCHFLAGS} -x c-header -o $@ $<

${OBJECT.dir}/%.h.gch: %.h
	@echo "GCH	$<"
	@${COMPILE.c} ${PCHFLAGS} -x c-header -o $@ $<

# C++

${OBJECT.dir}/%.H.pch: %.H
	@echo "PCH	$<"
	@${COMPILE.cpp} ${PCHFLAGS} -x c-header -o $@ $<

${OBJECT.dir}/%.H.gch: %.H
	@echo "GCH	$<"
	@${COMPILE.cpp} ${PCHFLAGS} -x c-header -o $@ $<

${OBJECT.dir}/%.hpp.pch: %.hpp
	@echo "PCH	$<"
	@${COMPILE.cpp} ${PCHFLAGS} -x c-header -o $@ $<

${OBJECT.dir}/%.hpp.gch: %.hpp
	@echo "GCH	$<"
	@${COMPILE.cpp} ${PCHFLAGS} -x c-header -o $@ $<

${OBJECT.dir}/%.hpp.pch: %.h++
	@echo "PCH	$<"
	@${COMPILE.cpp} ${PCHFLAGS} -x c-header -o $@ $<

${OBJECT.dir}/%.hpp.gch: %.h++
	@echo "GCH	$<"
	@${COMPILE.cpp} ${PCHFLAGS} -x c-header -o $@ $<
