#!/bin/sh

# Add apt sources for supported architectures
find /multiarch/apt-sources/ -name "*.list" -exec cp {} /etc/apt/sources.list.d/ \;

# Update apt database for new apt sources
apt-get update

# Set supported architectures in dpkg
dpkg --add-architecture arm64
dpkg --add-architecture armhf
