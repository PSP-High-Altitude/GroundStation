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
    radius: 5

    HoverHandler {
        id: hover_handler
    }

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

        Rectangle {
            MouseArea {
                anchors.fill: parent
                id: del_area
                hoverEnabled: true
                z: 1
            }
            radius: 3
            Layout.margins: 5
            Layout.minimumHeight: 25
            Layout.maximumHeight: 25
            Layout.minimumWidth: 25
            Layout.maximumWidth: 25
            color: del_area.containsMouse ? AppStyle.light_highlight : "transparent"
            Repeater {
                model: 2
                delegate: Rectangle {
                    visible: hover_handler.hovered
                    anchors.centerIn: parent
                    width: 20
                    height: 2
                    rotation: 45 + 90*index
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
