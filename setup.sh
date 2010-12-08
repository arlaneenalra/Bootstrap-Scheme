#!/bin/sh

TARGET=build

echo 'Setting up an out of source build tree:'
if [ ! -d $TARGET ] ; then 
	echo "	Creating Directory: $TARGET"
	mkdir $TARGET
fi

cd $TARGET

echo "	Running cmake"
cmake ..

echo 'Done.'
