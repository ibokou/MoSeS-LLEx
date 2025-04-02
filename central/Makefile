#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#  *  http://www.apache.org/licenses/LICENSE-2.0
#  * Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

# Toolchain commands
CROSS_COMPILE ?=
CC      := ccache $(CROSS_COMPILE)gcc
CXX     := ccache $(CROSS_COMPILE)g++ -std=c++20
LD      := $(CROSS_COMPILE)gcc
AR      := $(CROSS_COMPILE)ar
AS      := $(CROSS_COMPILE)as
NM      := $(CROSS_COMPILE)nm
OBJDUMP := $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy
SIZE    := $(CROSS_COMPILE)size

# Output
OUTDIR := build
BLINKSTICK_ROOT := ../blinkstickcpp-lib-0.2.1
SIMPLE_LIB_ROOT := ../../SimpleBLE

SRC := $(NIMBLE_SRC)

# Source files for Blinkstick library
SRC += \
	$(wildcard $(BLINKSTICK_ROOT)/src/*.cc) \
	$(wildcard $(BLINKSTICK_ROOT)/src/libusb/*.c) \
	$(NULL)

# Source files for SimpleDbus library
SRC += \
	$(wildcard $(BLINKSTICK_ROOT)/src/*.cc) \
	$(wildcard $(BLINKSTICK_ROOT)/src/libusb/*.c) \
	$(NULL)

# Source files for SimpleDbus library
SRC += \
	$(wildcard $(SIMPLE_LIB_ROOT)/simpledbus/src/advanced/*.cpp) \
	$(wildcard $(SIMPLE_LIB_ROOT)/simpledbus/src/base/*.cpp) \
	$(wildcard $(SIMPLE_LIB_ROOT)/simpledbus/src/interfaces/*.cpp) \
	$(NULL)

# Source files for SimpleBluez library
SRC += \
	$(wildcard $(SIMPLE_LIB_ROOT)/simplebluez/src/*.cpp) \
	$(wildcard $(SIMPLE_LIB_ROOT)/simplebluez/src/interfaces/*.cpp) \
	$(NULL)

# Source files for central app
SRC += \
	../StateMachine/Fault.cc \
	../StateMachine/StateMachine_.cc \
	src/statemachine/central.cc \
	src/led/blinkstick.cc \
	src/collect/collector.cc \
	src/plan/gait_estimation.cc \
	src/act/experiment.cc \
	src/io/file.cc \
	src/io/log.cc \
	src/os/os.cc \
	src/ble/bluez/advertise.cc \
	src/ble/bluez/proxy.cc \
	src/ble/peer.cc \
	src/ble/stack.cc \
	src/main.cc \
	$(NULL)

# Add NPL and all NimBLE directories to include paths
INC = \
    ./include \
	$(NIMBLE_ROOT)/porting/npl/linux/include \
	$(NIMBLE_ROOT)/nimble/transport/socket/include \
	$(NIMBLE_INCLUDE) \
	$(TINYCRYPT_INCLUDE) \
	$(NULL)

# Include for Blinkstick library
INC += \
	$(BLINKSTICK_ROOT)/include \
	$(NULL)

INC += \
	$(BLINKSTICK_ROOT)/include \
	$(NULL)	

# Includes for sdbus-ccp
INC += \
	/usr/arm-linux-gnueabihf/include \
	/usr/include/dbus-1.0 \
	/usr/arm-linux-gnueabihf/include/c++/11/ \
	/usr/lib/arm-linux-gnueabihf/dbus-1.0/include \
	$(NULL)

# Includes for simplebluez
INC += \
	$(SIMPLE_LIB_ROOT)/simpledbus/include \
	$(SIMPLE_LIB_ROOT)/simplebluez/include \
	/usr/include/dbus-1.0 \
	/usr/lib/arm-linux-gnueabihf/dbus-1.0/include \
	$(NULL)

# Include for central app
INC += \
	src/ble/include \
	../StateMachine/include

INCLUDES := $(addprefix -I, $(INC))

SRC_C  = $(filter %.c,  $(SRC))
SRC_CC = $(filter %.cc, $(SRC))
SRC_CPP = $(filter %.cpp, $(SRC))

OBJ := $(SRC_C:.c=.o)
OBJ += $(SRC_CC:.cc=.o)
OBJ += $(SRC_CPP:.cpp=.o)

TINYCRYPT_OBJ := $(TINYCRYPT_SRC:.c=.o)

CFLAGS =                    \
    $(INCLUDES)             \
    -g                      \
    -D_GNU_SOURCE           \
	-DEXTERNAL_EVENT_NO_HEAP_DATA \
    $(NULL)

LIBS := $(NIMBLE_LDFLAGS) -lfmt -ldbus-1 -lsdbus-c++ -lusb-1.0 -lrt -lpthread -lstdc++

.PHONY: all clean
.DEFAULT: all

all: $(OUTDIR)/nimble-linux

clean:
	find src -name '*.o' -delete
	find mynewt-nimble -name '*.o' -delete
	find ../StateMachine -name '*.o' -delete
	find ../blinkstickcpp-lib-0.2.1 -name '*.o' -delete
	find ../../SimpleBLE -name '*.o' -delete
	rm nimble-linux -f

$(TINYCRYPT_OBJ): CFLAGS+=$(TINYCRYPT_CFLAGS)

%.o: %.c
	$(CC) -c $(INCLUDES) $(CFLAGS) -o $@ $<

%.o: %.cc
	$(CXX) -c $(INCLUDES) $(CFLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) -c $(INCLUDES) $(CFLAGS) -o $@ $<

$(OUTDIR)/nimble-linux: $(OBJ) $(TINYCRYPT_OBJ)
	$(LD) -o $@ $^ $(LIBS)
	$(SIZE) $@