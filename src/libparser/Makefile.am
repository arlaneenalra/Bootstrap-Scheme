
noinst_LIBRARIES=libparser.a
libparser_a_SOURCES=lexer.l scheme.y

BUILT_SOURCES=lexer.c scheme.c

AM_YFLAGS=-d --verbose
AM_LFLAGS="--header-file=`pwd`/lexer.h" # this is a stupid requirement

MOSTLYCLEANFILES=lexer.h scheme.h scheme.output

include ../../Makefile.extra

#EXTRA_DIST=core.h