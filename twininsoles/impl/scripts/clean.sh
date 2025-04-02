#! /bin/bash

clean=false

while getopts :tc flag
do
    case $flag in
        c) clean=true ;;
        *) echo 'Error in command line parsing' >&2
           exit 1
    esac
done

if $clean; then
	idf.py fullclean
fi
