import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "AppStyle"
import Device 1.0

Window {
    id: device_dialog_window
    width: 480
    height: 320
    modality: Qt.ApplicationModal
    title: "Devices..."
    property list<Device> deviceList: []

    Page {
        anchors.fill: parent
        ListView {
            id: device_dialog_list
            anchors.fill: parent
            model: deviceList.length
            spacing: 5
            anchors.margins: 5
            delegate: DeviceDialogSelect {
                width: device_dialog_list.width
                device: deviceList[index]
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
