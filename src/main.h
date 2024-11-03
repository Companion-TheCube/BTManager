#pragma once
#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <filesystem>
#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <mutex>
#include "argparse.hpp"
#include "nlohmann/json.hpp"
#ifndef HTTPLIB_INCLUDED
#define HTTPLIB_INCLUDED
#include <httplib.h>
#endif
#ifdef __linux__
#include <sys/wait.h>
#include <unistd.h>
#endif
#ifndef WIN32_INCLUDED
#define WIN32_INCLUDED
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#endif

// #include <QtBluetooth/QBluetoothLocalDevice>
// #include <QtBluetooth/QLowEnergyAdvertisingData>
// #include <QtBluetooth/QLowEnergyCharacteristic>
// #include <QtBluetooth/QLowEnergyDescriptor>
// #include <QtBluetooth/QLowEnergyService>
// #include <QtBluetooth/QLowEnergyController>
// #include <QtBluetooth/QBluetoothUuid>
// #include <QtBluetooth/QBluetoothAddress>
// #include <QtBluetooth/QBluetoothDeviceInfo>
// #include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
// #include <QtBluetooth/QBluetoothLocalDevice>
// #include <QtBluetooth/QBluetoothServer>
// #include <QtBluetooth/QBluetoothSocket>

#ifndef PROJECT_VERSION_STRING
#define PROJECT_VERSION_STRING "0.0.0"
#endif
#ifndef PROJECT_NAME
#define PROJECT_NAME "BT_Http_Manager"
#endif

#endif// MAIN_H
