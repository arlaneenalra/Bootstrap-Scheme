

noinst_LIBRARIES=libparser.a
libparser_a_SOURCES=lexer.l scheme.y core.c util.c gc_funcs.c \
	            scheme_funcs.c eval.c output.c

BUILT_SOURCES=lexer.c scheme.c

AM_YFLAGS=-d --verbose

MOSTLYCLEANFILES=scheme.h scheme.output

include ../../Makefile.extra

#EXTRA_DIST=core.h
