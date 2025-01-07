cmake_minimum_required(VERSION 3.16)

project(GroundStation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Quick)
find_package(Qt6 6.8 REQUIRED COMPONENTS Charts)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS SerialPort)

qt_standard_project_setup(REQUIRES 6.8)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/groundstation.rc")

qt_add_executable(appGroundStation
    main.cpp
    ${app_icon_resource_windows}
    resources.qrc
)

set_source_files_properties(AppStyle/AppStyle.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(appGroundStation
    URI GroundStation
    VERSION 1.0
    QML_FILES
        Main.qml
        TapeAltimeter.qml
        Clock.qml
        PSPMap.qml
        DataTable.qml
        DeviceDialog.qml
        DeviceDialogSelect.qml
        AppStyle/AppStyle.qml
        CustomSwitch.qml
        AttitudeView.qml
        DataView.qml
        ArrowButton.qml
        DataSeries.qml
        DataGraph.qml
        DataAxis.qml
        ConnectionView.qml
    RESOURCES
        resources.qrc
        resources/alt_marker.png
        resources/appico.ico
        resources/map_pointer.png
        resources/marker.png
    SOURCES
        groundstation.rc
        fileio.h fileio.cpp
        device.h device.cpp
        pspcom_reader.h pspcom_reader.cpp
        pspcommsg.h
        crc.h crc.cpp
        pspcom_decoder.h pspcom_decoder.cpp
        settings.h settings.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appGroundStation PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGroundStation
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appGroundStation
    PRIVATE Qt6::Quick
    PRIVATE Qt6::QuickControls2
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Charts
    PRIVATE Qt6::SerialPort
)

include(GNUInstallDirs)
install(TARGETS appGroundStation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
