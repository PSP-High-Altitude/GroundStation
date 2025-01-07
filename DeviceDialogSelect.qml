import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "AppStyle"
import Device 1.0
import PSPCOMReader 1.0
import PSPCOMDecoder 1.0

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

        device.newDataArrived.connect(() => {
            pspcom.write(device.read(device.bytesAvailable))
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

    PSPCOMReader {
        id: pspcom
        onMessageReceived: (msg) => {
            decoder.processMessage(msg)
        }
    }

    PSPCOMDecoder {
        id: decoder
        onRssiReceived: (rssi) => {
            console.log(rssi)
        }
    }
}
