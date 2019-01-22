#Makefile for arduino projects
SRC_DIR = ${PWD}
BUILD_DIR = ${PWD}/build
MAIN_FILE = ping1d-simple.ino

PACKAGE ?= arduino
ARCH ?= avr
BOARD ?= uno

ARG += --board ${PACKAGE}:${ARCH}:${BOARD} --verbose-build --pref build.path=${BUILD_DIR}/$<

# Define default serial port
SERIAL ?= /dev/ttyUSB0

all: ping1d-advanced-verify ping1d-simple-verify pingmessage-verify

deps:
	arduino --install-boards ${PACKAGE}:${ARCH}

%: %/%.ino

%-verify: %
	arduino --verify ${ARG} ${SRC_DIR}/$</$<.ino

%-upload : %
	arduino --upload ${ARG} ${SRC_DIR}/$</$<.ino --port ${SERIAL}

clean:
	rm -rf ${BUILD_DIR}

# Make usage:
# make <example_name>-verify
# make <example_name>-upload

# ex:
# make ping1d-simple-upload SERIAL=/dev/ttyUSBx
