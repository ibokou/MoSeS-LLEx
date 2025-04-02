#!/bin/bash

NIMBLE_ARCHIVE=nimble.tar.gz
NIMBLE_DIR=mynewt-nimble
BUILD_DIR="build/"
NIMBLE_VERSION=nimble_1_6_0_tag
clean=false
makeclean=false
CROSS_COMPILE=
NIMBLE_CFLAGS=
NIMBLE_LDFLAGS=

while getopts :amtc flag
do
    case $flag in
		a) CROSS_COMPILE=arm-linux-gnueabihf- ;;
		m) NIMBLE_CFLAGS=-m32
		   NIMBLE_LDFLAGS=-m32 ;;
        t) clean=true ;;
		c) makeclean=true ;;
        *) echo 'Error in command line parsing' >&2
           exit 1
    esac
done

if $clean; then
	rm -r $NIMBLE_DIR
fi
 
if [ ! -d "$NIMBLE_DIR" ];  then
	wget https://github.com/apache/mynewt-nimble/archive/refs/tags/$NIMBLE_VERSION.tar.gz -O $NIMBLE_ARCHIVE
	tar xvfz $NIMBLE_ARCHIVE --transform s/$NIMBLE_DIR-$NIMBLE_VERSION/$NIMBLE_DIR/
	rm $NIMBLE_ARCHIVE
fi

if [ ! -d "$BUILD_DIR" ];  then
	mkdir -p $BUILD_DIR
fi

if $makeclean; then
	make clean
fi

make CROSS_COMPILE=$CROSS_COMPILE NIMBLE_CFLAGS=$NIMBLE_CFLAGS NIMBLE_LDFLAGS=$NIMBLE_LDFLAGS
