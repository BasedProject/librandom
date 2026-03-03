#!/usr/bin/make -f
.SUFFIXES:

TARGET := $(shell basename $$PWD).out

## make.mk
# commented assignments are overrides and are otherwise reasonable defaults.

CFLAGS   := -std=c23 -O2 -pthread
CXXFLAGS := -std=c++20 -O2 -pthread
CPPFLAGS := -D_GNU_SOURCE -Wno-format
LDFLAGS  := -lm

SOURCE.dir := source
OBJECT.dir := object

MAKE.dir := tool/make
MAKE.filter := 03-peru.mk

## 00-compiler.mk

# Externally overridable with CC=.. CXX=..
PREFER_GCC := 0

## 01-debug.mk

DEBUG          ?= 0
SANITIZE       ?= 0
VECTORIZED_ALL ?= 0
VECTORIZED     ?= 0

## 11-lib.mk

NOT_APART_OF_LIBRARY := source/main.c
LIBTARGET := ${TARGET}

## 11-bison.mk

LFLAGS += --debug --trace
YFLAGS += --debug

## 11-gperf.mk

#GPERF := gperf
#GPERF.orig :=

## 11-pch.mk

HEADER.pch.filter := source/librandom_x.h

###

# Keep source flat. All filenames have optional, assumed prefixes.

-include ${MAKE.dir}/make.mk
