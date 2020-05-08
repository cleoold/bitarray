CC = gcc
PREFIX = /usr/local
INC = /usr/include

LUA_VERSION = 5.3

OUTPUT_DIR = out

LUA_INC = -I$(INC)/lua$(LUA_VERSION)
LUA_LIB = -llua$(LUA_VERSION)

ifneq ($(filter cygwin% msys% mingw%, $(HOST_OS)),)
	CORE = $(OUTPUT_DIR)/bitarray.dll
	LIBFLAG = -shared
	LIBS = $(LUA_LIB)
else
ifeq ($(HOST_OS),darwin)
	CORE = $(OUTPUT_DIR)/bitarray.so
	LIBFLAG = -bundle -undefined dynamic_lookup
	CCSHARED = -fno-common
else
	CORE = $(OUTPUT_DIR)/bitarray.so
	LIBFLAG = -shared
	CCSHARED = -fPIC
endif
endif

ifndef CFLAGS
	CFLAGS = -Wall -Wextra -Wno-sign-compare -O2 -g
endif
CFLAGS += $(CCSHARED) $(LUA_CFLAGS)
LDFLAGS += $(LIBFLAG)

SRC = ext/bitarray.c ext/bitarray_impl.h
OBJ = $(OUTPUT_DIR)/bitarray.o

.PHONY : all

all : $(OUTPUT_DIR) $(CORE)

$(OUTPUT_DIR) :
	mkdir -p $@

$(CORE) : $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)

$(OBJ) : $(SRC)
	$(CC) $(CFLAGS) -c -o $@ $< $(LUA_INC)

clean :
	rm -r $(OUTPUT_DIR)

