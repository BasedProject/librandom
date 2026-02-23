DEBUG      ?= 0
SANITIZE   ?= 0
VECTORIZED ?= 0

ifeq (${DEBUG},1)
        LFLAGS   += --debug --trace
        YFLAGS   += --debug
        # there's gcc15 -flto-incremental
        # for clang see https://clang.llvm.org/docs/ThinLTO.html
        ifeq (${CC},clang)
                CFLAGS   += -flto=thin -glldb
                CXXFLAGS += -flto=thin -glldb
        else
                ifeq (${CC},gcc)
                        CFLAGS   += -pg -ggdb
                        CXXFLAGS += -pg -ggdb
                else
                        CFLAGS   += -g
                        CXXFLAGS += -g
                endif
        endif
        CFLAGS   += -fno-inline -Wall -Wextra -Wpedantic -Wshadow -Wundef -fno-omit-frame-pointer
        CXXFLAGS += -fno-inline -Wall -Wextra -Wpedantic -Wshadow -Wundef -fno-omit-frame-pointer
else
        CFLAGS   += -flto=auto -ftree-vectorize -march=x86-64 -mtune=generic
        CXXFLAGS += -flto=auto -ftree-vectorize -march=x86-64 -mtune=generic
        CPPFLAGS += -DNDEBUG
endif

ifeq (${SANITIZE},1)
        ifeq (${CC},clang)
                CFLAGS   += -fvisibility=hidden -fsanitize=address,leak,undefined,bounds,cfi
                CXXFLAGS += -fvisibility=hidden -fsanitize=address,leak,undefined,bounds,cfi
        else
                CFLAGS   += -fsanitize=address,leak,undefined
                CXXFLAGS += -fsanitize=address,leak,undefined
        endif
endif

ifeq (${VECTORIZED_ALL},1)
        ifeq (${CC},clang)
          CFLAGS   += -Rpass=loop-vectorize -Rpass-missed=loop-vectorize
          CXXFLAGS += -Rpass=loop-vectorize -Rpass-missed=loop-vectorize
        else
          CFLAGS   += -fopt-info-vec-all
          CXXFLAGS += -fopt-info-vec-all
        endif
else
        ifeq (${VECTORIZED},1)
                ifeq (${CC},clang)
                        CFLAGS   += -Rpass=loop-vectorize
                        CXXFLAGS += -Rpass=loop-vectorize
                else
                        CFLAGS   += -fopt-info-vec-optimized
                        CXXFLAGS += -fopt-info-vec-optimized
                endif
        endif
endif
