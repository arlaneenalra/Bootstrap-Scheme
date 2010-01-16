include ../Makefile.extra

DEPENDENCIES=libtester/libtester.a
LDADD=@LIBOBJS@ libtester/libtester.a ../src/libparser/libparser.a

TESTS=test_tester test_fail create_interp scheme_define

XFAIL_TESTS=test_fail

check_PROGRAMS=${TESTS} ${XFAIL_TESTS}

SUBDIRS=libtester


# do we want to use valgrind?
if USE_VALGRIND
TESTS_ENVIRONMENT=valgrind -q --leak-check=full --error-exitcode=2
endif
