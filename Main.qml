import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion
import QtQuick.Shapes
import "AppStyle"

ApplicationWindow {
    id: app_window
    width: 1280
    height: 720
    visible: true
    title: "PSP GroundStation"
    color: AppStyle.window
    property int stack_index: 0
    readonly property var stack_components: [attitude_view, data_view]

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Exit"
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: "Devices"
            MenuItem {
                text: "Devices..."
                onTriggered: device_dialog.show();
            }
        }
    }

    // Popups/Dialogs
    DeviceDialog {
        id: device_dialog
    }

    ColumnLayout {
        anchors.fill: parent

        // Header section
        RowLayout {
            Layout.topMargin: 10
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            Layout.rightMargin: 10
            Clock {
                id: clock
                height: 30
            }
        }

        // Data section
        RowLayout {
            id: data_section
            Layout.fillHeight: true
            Layout.fillWidth: true

            ArrowButton {
                width: 30
                height: 100
                Layout.margins: 10
                direction: "left"
                onClicked: {
                    main_swipe_view.decrementCurrentIndex()
                }
            }

            SwipeView {
                id: main_swipe_view
                Layout.fillHeight: true
                Layout.fillWidth: true
                currentIndex: 0
                interactive: false

                AttitudeView {
                    id: attitude_view
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: (main_swipe_view.currentItem === this)
                }

                DataView {
                    id: data_view
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    visible: (main_swipe_view.currentItem === this)
                }
            }

            ArrowButton {
                width: 30
                height: 100
                Layout.margins: 10
                direction: "right"
                onClicked: {
                    main_swipe_view.incrementCurrentIndex()
                }
            }
        }
    }
}
