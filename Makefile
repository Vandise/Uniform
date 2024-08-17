UNAME       := $(shell uname)
CC          := gcc
INC_DIR     := inc/

ADDFLAGS    := -g -O2 -std=c11
SHAREDFLAG  := -shared

ifeq ($(OS), Windows_NT)
  LIBEXT    := dll
  SYSLIBDIR := /c/Windows/system32
  SYSINCDIR := /usr/include
  ADDFLAGS  := $(ADDFLAGS) -L/c/Windows/system32
else
  ifeq ($(UNAME), Linux)
    SYSLIBDIR := /usr/local/lib
    SYSINCDIR := /usr/local/include
    ADDFLAGS  := $(ADDFLAGS) -fPIC
  endif
  
  ifeq ($(UNAME), Darwin)
    SYSLIBDIR := /usr/local/lib
    SYSINCDIR := /usr/local/include
    TESTWRAPDMP  := 1
    SHAREDFLAG   := -dynamiclib
    LIBEXT    := dylib
  endif
endif

-include modules/scanner/Makefile.in

testing:
	echo $ACTION
