import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ApplicationWindow {
    id: app_window
    width: 1280
    height: 720
    visible: true
    title: "PSP GroundStation"

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Exit"
                onTriggered: Qt.quit();
            }
        }
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
            Layout.topMargin: 10
            Layout.leftMargin: 10
            Layout.bottomMargin: 10
            Layout.rightMargin: 10

            PSPMap {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.horizontalStretchFactor: 1
                Layout.verticalStretchFactor: 1
            }

            DataTable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.horizontalStretchFactor: 1
                Layout.verticalStretchFactor: 1

            }

            TapeAltimeter {
                id: tape_alt
                altitude: 190
                ground_alt: 190
                Layout.fillHeight: true
                Layout.verticalStretchFactor: 1
            }
        }
    }
}
