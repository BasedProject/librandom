#!/usr/bin/make -f
.SUFFIXES:

TARGET := random.out
LIBTARGET := librandom
NOT_APART_OF_LIBRARY := source/main.c

CFLAGS   := -std=c23 -O2 -pthread
CPPFLAGS := -D_GNU_SOURCE
LDFLAGS  := -lm

SOURCE.dir := source
OBJECT.dir := object

HEADER.pch.filter :=

-include tool/make/*.mk
