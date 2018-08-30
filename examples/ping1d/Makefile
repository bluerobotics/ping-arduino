#Makefile for arduino projects
SRC_DIR = ${PWD}
BUILD_DIR = ${PWD}/build
MAIN_FILE = ping1d.ino

PACKAGE ?= arduino
ARCH ?= avr
BOARD ?= nano

ARG = --board ${PACKAGE}:${ARCH}:${BOARD} --verbose-build --pref build.path=${BUILD_DIR}

# Define default serial port
SERIAL ?= /dev/ttyUSB0

all:
	arduino --verify ${ARG} ${SRC_DIR}/${MAIN_FILE}

deps:
	arduino --install-boards ${PACKAGE}:${ARCH}
	arduino --install-library "ping-arduino"

flash:
	arduino --upload ${ARG} ${SRC_DIR}/${MAIN_FILE} --port ${SERIAL}

clean:
	rm -rf ${BUILD_DIR}

run: all flash

# Make examples:
# make flash SERIAL=/dev/ttyUSBx