# GroundStation
PSP High Altitude Ground Station software
(Work in progress)

Created in QT Creator using QT 6.8.1

## Supported Platforms

Windows

## Build Instructions

Download and install QT Creator Community Edition. In the installer, install QT 6.8.1 and all of its addons. Clone this repository.

Start QT Creator and open the directory you cloned to as a project. Setup the QT Creator build settings to build with CMake in Debug mode to collaborate or Release mode for regular use. Note that after compiling in Release mode, the program still requires QT dll files to run outside of QT Creator.

## Supported Devices

Custom devices can be created, however, all devices must support communication using the PSPCOM protocol. Information about the PSPCOM protocol can be found on the PSPHA Confluence here: https://purdue-space-program.atlassian.net/wiki/spaces/HA/pages/410189948/PSPCOM.
GroundStation is developed with the focus of PSP High Altitude's PAL series flight computers and, as such, functionality with other devices is not guaranteed.
