#!/bin/sh

TARGET=build

echo 'Setting up an out of source build tree:'
if [ ! -d $TARGET ] ; then 
	echo "	Creating Directory: $TARGET"
	mkdir $TARGET
	ln -s ../lib $TARGET/lib
	ln -s ../test $TARGET/test
fi

cd $TARGET

echo "	Running cmake"
cmake ..

echo 'Done.'
