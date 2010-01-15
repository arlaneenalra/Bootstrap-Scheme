include ../Makefile.extra

DEPENDENCIES=libtester/libtester.a
LDADD=@LIBOBJS@ libtester/libtester.a

TESTS=test_tester test_fail

XFAIL_TESTS=test_fail

check_PROGRAMS=test_tester test_fail

#test_tester_SOURCES=test_tester.c


SUBDIRS=libtester



# do we want to use valgrind?
if USE_VALGRIND
TESTS_ENVIRONMENT=valgrind -q --leak-check=full --error-exitcode=2
endif
