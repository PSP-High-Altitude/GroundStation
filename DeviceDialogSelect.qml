import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "AppStyle"

Rectangle {
    property string dev_name: ""
    color: AppStyle.light
    height: 40

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        Text {
            id: device_name
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: dev_name
            font.pointSize: 16
            verticalAlignment: Text.AlignVCenter
            color: "white"
        }

        CustomSwitch {
            Layout.alignment: Qt.AlignRight
            implicitHeight: 30
            id: device_on
            onToggled: {
                if(state === "WAIT") {
                    wait_timer.running = true
                }
            }
        }

        // DEMO of wait animation
        // TODO: replace with actual device connection
        Timer {
            id: wait_timer
            interval: 1000
            repeat: false
            running: false
            onTriggered: {
                device_on.state = "ON"
            }
        }
    }
}
