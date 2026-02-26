#!/usr/bin/make -f
.SUFFIXES:

TARGET := random.out
LIBTARGET := librandom
NOT_APART_OF_LIBRARY := source/main.c

CFLAGS   := -std=c23 -O2 -pthread
CPPFLAGS := -D_GNU_SOURCE -Wno-format
LDFLAGS  := -lm

SOURCE.dir := source
OBJECT.dir := object

HEADER.pch.filter := source/librandom_x.h

-include tool/make/*.mk
