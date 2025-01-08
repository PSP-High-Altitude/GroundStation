import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "AppStyle"
import Device 1.0
import PSPCOMReader 1.0
import PSPCOMDecoder 1.0
import SerialList 1.0

Rectangle {
    id: root
    property Device device
    color: AppStyle.light
    height: 40

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        Text {
            id: device_name
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: device.deviceName
            font.pointSize: 16
            verticalAlignment: Text.AlignVCenter
            color: "white"
        }

        SerialList {
            id: serial_list
        }

        ComboBox {
            Layout.alignment: Qt.AlignRight
            displayText: device.portName
            id: combo
            model: ListModel {
                id: model
                Component.onCompleted: {
                    // Refresh the combobox when clicked
                    combo.pressedChanged.connect(() => {
                        if(combo.pressed) {
                            // First clear it
                            model.clear()
                            // Then list every available port
                            for(let port of serial_list.ports) {
                                model.append({ text: port })
                            }
                        }
                    })
                }
            }

            // If the combobox is changed, update the device's port
            onCurrentTextChanged: {
                if(currentText !== "") {
                    device.portName = currentText
                }
            }
        }

        CustomSwitch {
            Layout.alignment: Qt.AlignRight
            implicitHeight: 30
            id: device_on
            onToggled: {
                switch(state) {
                case "WAIT":
                    device.connect()
                    break
                case "ON":
                    break
                case "OFF":
                    device.disconnect()
                    break
                }
            }
        }
    }

    // Connect device signals
    function setDevice() {
        device.portClosed.connect(() => {
            device_on.state = "OFF"
        })

        device.portOpened.connect(() => {
            device_on.state = "ON"
        })
    }

    // Set device on creation
    Component.onCompleted: {
        setDevice();
    }

    // Set device if property is changed
    onDeviceChanged: {
        setDevice();
    }
}
