#!/bin/sh

# Install native compilation. build and debugging tools
apt-get install -y build-essential cmake make gdb

# Install cross-complilation tools
apt-get install -y crossbuild-essential-armhf crossbuild-essential-arm64

# Install git for Cmake FetchConten
apt-get install -y git

# Install dependencies for SimpleBLE
apt-get install -y libdbus-1-dev libdbus-1-dev:${TARGET_ARCH} libfmt-dev:${TARGET_ARCH}

# Install dependency for BlinkStick usb
apt-get install -y libusb-1.0-0-dev:${TARGET_ARCH}
