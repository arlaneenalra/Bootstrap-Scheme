#!/bin/sh
echo "Cleanning . . ."
make clean
echo

echo "Source Statistics:"
wc `find . -regextype posix-basic -iregex '.*\.\(scm\|h\|c\|y\|l\)' | grep -v 'test\/test_eyeball\|CMake'`

echo
echo "Commit Count: " `git log | grep '^commit' | wc -l`
echo
