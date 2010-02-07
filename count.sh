#!/bin/sh
wc `find . -regextype posix-basic -iregex '.*\.\(scm\|h\|c\|y\|l\)' | grep -v test`
