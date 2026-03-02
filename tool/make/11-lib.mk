NOT_APART_OF_LIBRARY := $(NOT_APART_OF_LIBRARY:${SOURCE.dir}/%=%)
NOT_APART_OF_LIBRARY := $(NOT_APART_OF_LIBRARY:${OBJECT.dir}/%=%)
NOT_APART_OF_LIBRARY := $(addprefix ${OBJECT.dir}/,${NOT_APART_OF_LIBRARY})
NOT_APART_OF_LIBRARY := $(NOT_APART_OF_LIBRARY:.c=.o)
NOT_APART_OF_LIBRARY := $(NOT_APART_OF_LIBRARY:.cpp=.o)
OBJECT.lib := $(filter-out ${NOT_APART_OF_LIBRARY},${OBJECT.orig})

LIBTARGET ?= lib$(basename $(TARGET))

${OBJECT.dir}/${LIBTARGET}.a: ${OBJECT.lib}
	@echo "AR	$@"
	@ar rcs -o "$@" $+

${OBJECT.dir}/${LIBTARGET}.so: ${OBJECT.lib}
	@echo "SO	$@"
ifeq (${SOURCE.orig.cxx},)
	@${LINK.c} -shared -o "$@" $+
else
	@${LINK.cpp} -shared -o "$@" $+
endif

ALSO += ${OBJECT.dir}/${LIBTARGET}.a ${OBJECT.dir}/${LIBTARGET}.so
