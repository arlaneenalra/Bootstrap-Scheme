#!/bin/sh

echo "Source Statistics:"
wc `find . -regextype posix-basic -iregex '.*\.\(scm\|h\|c\|y\|l\)' | grep -v 'test\/test_eyeball\|CMake\|build\|vendor'`

echo
echo "Commit Count: " `git log | grep '^commit' | wc -l`
echo
