ifneq (${DEBUG},1)
        CFLAGS   += -ftrivial-auto-var-init=zero -fPIC -fstack-protector-strong -fstack-clash-protection
        CXXFLAGS += -ftrivial-auto-var-init=zero -fPIC -fstack-protector-strong -fstack-clash-protection
        LDFLAGS  += -pie -Wl,-z,relro,-z,now
        CPPFLAGS += -D_FORTIFY_SOURCE=3
endif
