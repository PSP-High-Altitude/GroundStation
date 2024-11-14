import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "AppStyle"

Window {
    id: device_dialog_window
    width: 480
    height: 320
    modality: Qt.ApplicationModal
    title: "Devices..."

    Page {
        anchors.fill: parent
        ListView {
            id: device_dialog_list
            anchors.fill: parent
            model: 3
            spacing: 5
            anchors.margins: 5
            delegate: DeviceDialogSelect {
                width: device_dialog_list.width
                dev_name: "Device " + index
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
