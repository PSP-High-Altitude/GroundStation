cmake_minimum_required(VERSION 3.5)

project(GroundStation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS SerialPort)
find_package(Qt6 REQUIRED COMPONENTS Qml)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickWidgets)
find_package(Qt6 REQUIRED COMPONENTS Network)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h
    display/map.cpp display/map.h
    clock.cpp clock.h
    display/connection_status.cpp display/connection_status.h
    mainwindow.ui
    resources.qrc
    display/tape_altimeter.cpp display/tape_altimeter.h
    groundstation.rc
    console.h console.cpp
    keys.h
    native_event_filter.h native_event_filter.cpp
    pspcom/pspcom.h pspcom/pspcom.cpp
    pspcom/pspcommsg.h
    serial/serial_device.h serial/serial_device.cpp
    serial/serial_port.h serial/serial_port.cpp
    serial/udp_port.h serial/udp_port.cpp
    device.h device.cpp
    menus/createdevice.h menus/createdevice.cpp menus/createdevice.ui
    menus/selectmenu.h menus/selectmenu.cpp menus/selectmenu.ui
    menus/editdevice.h menus/editdevice.cpp menus/editdevice.ui
    menus/createpspcom.h menus/createpspcom.cpp menus/createpspcom.ui
    pspcom/pspcomworker.h pspcom/pspcomworker.cpp
    pspcom/crc.h pspcom/crc.cpp
    utils/search.h
    utils/equatable.h
    display/data_table_row.h display/data_table_row.cpp
    display/datatableconfig.h display/datatableconfig.cpp display/datatableconfig.ui
    display/data_table.h display/data_table.cpp
    utils/comboboxdelegate.h utils/comboboxdelegate.cpp
    display/pyro_status.h display/pyro_status.cpp
    menus/pyro_menu.h menus/pyro_menu.cpp menus/pyro_menu.ui
    utils/misc.h utils/misc.cpp
    menus/resetdevice.ui menus/resetdevice.h menus/resetdevice.cpp
)

# set(CMAKE_EXE_LINKER_FLAGS "/MANIFEST:NO")
qt_add_executable(GroundStation
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(GroundStation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(GroundStation PRIVATE Qt6::SerialPort)
target_link_libraries(GroundStation PRIVATE Qt6::Qml)
target_link_libraries(GroundStation PRIVATE Qt6::Quick)
target_link_libraries(GroundStation PRIVATE Qt6::QuickWidgets)
target_link_libraries(GroundStation PRIVATE setupapi)
target_link_libraries(GroundStation PRIVATE Qt6::Network)


set_target_properties(GroundStation PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# SET_TARGET_PROPERTIES(GroundStation PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:WINDOWS")
# target_link_options(GroundStation PRIVATE "/MANIFESTUAC:level='requireAdministrator'" PRIVATE "/MANIFESTUAC:uiAccess='false'")

install(TARGETS GroundStation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GroundStation)
endif()
