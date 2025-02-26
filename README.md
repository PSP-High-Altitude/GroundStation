# GroundStation
PSP High Altitude Ground Station software

Created in QT Creator using QT 6.6.1

## Supported Platforms

Windows

## Build Instructions

Download and install QT Creator Community Edition. In the installer, install QT 6.6.1 and all of its addons. Clone this repository and add the following file:

keys.h:
```
#ifndef KEYS_H
#define KEYS_H

#define GOOGLE_API_KEY "your key in quotes"

#endif
```
The google key must have the following APIs enabled: Maps Elevation API

Start QT Creator and open the directory you cloned to as a project. Setup the QT Creator build settings to build with CMake in Debug mode to collaborate or Release mode for regular use. Note that after compiling in Release mode, the program still requires QT dll files to run outside of QT Creator.

## Supported Devices

Custom devices can be created, however, all devices must support communication using the PSPCOM protocol. Information about the PSPCOM protocol can be found on the PSPHA Confluence here: https://purdue-space-program.atlassian.net/wiki/spaces/HA/pages/410189948/PSPCOM.
