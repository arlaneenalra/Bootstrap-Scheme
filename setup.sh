#!/bin/sh

TARGET=build

echo 'Setting up an out of source build tree:'
if [ -d $TARGET ] ; then 
	echo "	Cleaning out old build: $TARGET"
	rm -r $TARGET
fi

echo "	Creating Directory: $TARGET"
mkdir $TARGET

ln -s ../lib $TARGET/lib
ln -s ../test $TARGET/test

cd $TARGET

echo "	Running cmake"
cmake ..

echo 'Done.'
