include ../Makefile.extra

bin_PROGRAMS=bootstrap


bootstrap_SOURCES=bootstrap.c
bootstrap_DEPENDENCIES=libparser/libparser.a
bootstrap_LDADD=@LIBOBJS@ libparser/libparser.a

SUBDIRS=libparser

