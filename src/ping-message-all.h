// C++ implementation of the Blue Robotics 'Ping' binary message protocol

//~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!
// THIS IS AN AUTOGENERATED FILE
// DO NOT EDIT
//~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!

#pragma once

#include "ping-message-common.h"
#include "ping-message-ping1d.h"
#include "ping-message-ping360.h"

// Declare an empty Q_NAMESPACE and Q_ENUM_NS if it's not a Qt project, otherwise include QObject
#ifndef QT_CORE_LIB
#define Q_NAMESPACE
#define Q_ENUM_NS(x)
#else
#include <QObject>
#endif

namespace PingEnumNamespace {
Q_NAMESPACE
enum class PingDeviceType {
    UNKNOWN = 0,
    PING1D = 1,
    PING360 = 2,
    PINGHF = 3,
};
Q_ENUM_NS(PingDeviceType)

enum class PingMessageId {
    BLUEBPS_SET_CELL_VOLTAGE_MINIMUM = 9000,
    BLUEBPS_SET_CELL_VOLTAGE_TIMEOUT = 9001,
    BLUEBPS_SET_CURRENT_MAX = 9002,
    BLUEBPS_SET_CURRENT_TIMEOUT = 9003,
    BLUEBPS_SET_TEMPERATURE_MAX = 9004,
    BLUEBPS_SET_TEMPERATURE_TIMEOUT = 9005,
    BLUEBPS_SET_STREAM_RATE = 9006,
    BLUEBPS_SET_LPF_SAMPLE_FREQUENCY = 9007,
    BLUEBPS_SET_LPF_SETTING = 9008,
    BLUEBPS_CELL_VOLTAGE_MIN = 9100,
    BLUEBPS_CELL_TIMEOUT = 9101,
    BLUEBPS_CURRENT_MAX = 9102,
    BLUEBPS_CURRENT_TIMEOUT = 9103,
    BLUEBPS_TEMPERATURE_MAX = 9104,
    BLUEBPS_TEMPERATURE_TIMEOUT = 9105,
    BLUEBPS_STATE = 9106,
    BLUEBPS_EVENTS = 9107,
    BLUEBPS_REBOOT = 9200,
    BLUEBPS_ERASE_FLASH = 9201,
    BLUEBPS_RESET_DEFAULTS = 9202,
    COMMON_ACK = 1,
    COMMON_NACK = 2,
    COMMON_ASCII_TEXT = 3,
    COMMON_GENERAL_REQUEST = 6,
    COMMON_DEVICE_INFORMATION = 4,
    COMMON_PROTOCOL_VERSION = 5,
    PING1D_SET_DEVICE_ID = 1000,
    PING1D_SET_RANGE = 1001,
    PING1D_SET_SPEED_OF_SOUND = 1002,
    PING1D_SET_MODE_AUTO = 1003,
    PING1D_SET_PING_INTERVAL = 1004,
    PING1D_SET_GAIN_SETTING = 1005,
    PING1D_SET_PING_ENABLE = 1006,
    PING1D_FIRMWARE_VERSION = 1200,
    PING1D_DEVICE_ID = 1201,
    PING1D_VOLTAGE_5 = 1202,
    PING1D_SPEED_OF_SOUND = 1203,
    PING1D_RANGE = 1204,
    PING1D_MODE_AUTO = 1205,
    PING1D_PING_INTERVAL = 1206,
    PING1D_GAIN_SETTING = 1207,
    PING1D_TRANSMIT_DURATION = 1208,
    PING1D_GENERAL_INFO = 1210,
    PING1D_DISTANCE_SIMPLE = 1211,
    PING1D_DISTANCE = 1212,
    PING1D_PROCESSOR_TEMPERATURE = 1213,
    PING1D_PCB_TEMPERATURE = 1214,
    PING1D_PING_ENABLE = 1215,
    PING1D_PROFILE = 1300,
    PING1D_GOTO_BOOTLOADER = 1100,
    PING1D_CONTINUOUS_START = 1400,
    PING1D_CONTINUOUS_STOP = 1401,
    PING360_DEVICE_ID = 2000,
    PING360_DEVICE_DATA = 2300,
    PING360_AUTO_DEVICE_DATA = 2301,
    PING360_RESET = 2600,
    PING360_TRANSDUCER = 2601,
    PING360_AUTO_TRANSMIT = 2602,
    PING360_MOTOR_OFF = 2903,
};
Q_ENUM_NS(PingMessageId)
}

using namespace PingEnumNamespace;