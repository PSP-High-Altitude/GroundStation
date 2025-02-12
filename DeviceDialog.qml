import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "AppStyle"
import AppSettings 1.0
import Device 1.0

Window {
    id: device_dialog_window
    width: 480
    height: 320
    modality: Qt.ApplicationModal
    title: "Devices..."
    property AppSettings settings

    Page {
        anchors.fill: parent
        ColumnLayout {
            anchors.fill: parent
            ListView {
                id: device_dialog_list
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
                Layout.fillHeight: true
                Layout.verticalStretchFactor: 1
                model: settings.deviceList.length
                spacing: 5
                Layout.margins: 10
                delegate: DeviceDialogSelect {
                    width: device_dialog_list.width
                    device: settings.deviceList[index]
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.horizontalStretchFactor: 1
                Layout.margins: 10
                Layout.minimumHeight: 40
                Layout.maximumHeight: 40
                radius: 5
                color: AppStyle.light
                Rectangle {
                    x: (parent.width/2) - (width/2)
                    y: (parent.height/2) - (height/2)
                    width: 5
                    height: 20
                    radius: 1
                    color: "lightgray"
                }
                Rectangle {
                    x: (parent.width/2) - (width/2)
                    y: (parent.height/2) - (height/2)
                    width: 20
                    height: 5
                    radius: 1
                    color: "lightgray"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: settings.addNewDevice()
                }
            }
        }

        footer: DialogButtonBox {
            standardButtons: DialogButtonBox.Ok
            onAccepted: {
                device_dialog_window.hide()
            }
        }
    }
}
