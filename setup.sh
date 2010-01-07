#!/bin/sh
git log > ChangeLog
aclocal
autoconf
automake --add-missing
