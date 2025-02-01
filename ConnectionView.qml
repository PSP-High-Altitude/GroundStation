import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Device 1.0
import AppSettings 1.0

RowLayout {
    id: root
    required property AppSettings settings
    ComboBox {
        id: dropdown
        font.pointSize: 20
        onCurrentIndexChanged: {
            if(currentIndex) {
                settings.activeDevice = model.get(currentIndex).device
            } else {
                settings.activeDevice = null
            }
        }

        model: ListModel {
            id: model
            ListElement {
                text: "No device selected"
                device: null
            }
            Component.onCompleted: {
                for (let dev of settings.deviceList) {
                    if(dev.isConnected) {
                        // If the device is connected, display it
                        append({text: dev.deviceName, device: dev})
                    }
                    dev.isConnectedChanged.connect(() => updateModel(dev))
                }
                settings.deviceAdded.connect((device) => addDevice(device))
                settings.deviceRemoved.connect(() => removeDevice())
            }

            // Remove a device
            function removeDevice() {
                for (var i = 1; i < count; i++) {
                    // Search to see if this is the removed device
                    for(let dev of settings.deviceList) {
                        if(dev === get(i).device) {
                            if(i === dropdown.currentIndex) {
                                dropdown.currentIndex = 0
                            }
                            remove(i, 1)

                            return
                        }
                    }
                }
            }

            // Add a device
            function addDevice(dev) {
                if(dev.isConnected) {
                    // If the device is connected, display it
                    append({ text: dev.deviceName, device: dev })
                }
                dev.isConnectedChanged.connect(updateModel(dev))
            }

            // Update a specific device in the model
            function updateModel(obj) {
                var found = false
                for (var i = 1; i < count; i++) {
                    if (get(i).device === obj) {
                        if(!obj.isConnected) {
                            // If the device is disconnected, stop displaying it
                            dropdown.currentIndex = 0
                            remove(i, 1);
                            i--;
                        }
                        found = true
                        break;
                    }
                }
                if(!found && obj.isConnected) {
                    // If the device is connected, but not shown, diplay it
                    append({ text: obj.deviceName, device: obj })
                }
            }
        }

        contentItem: Text {
            leftPadding: 4
            rightPadding: dropdown.indicator.width + dropdown.spacing
            text: dropdown.displayText
            font: dropdown.font
            color: "white"
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        TextMetrics {
            id: display_text_metrics
            font: dropdown.font
            text: dropdown.displayText
        }

        background: Rectangle {
            implicitWidth: display_text_metrics.width + 20
            implicitHeight: display_text_metrics.height + 10
            color: dropdown.pressed ? "#2c2c2c" : "#3c3c3c"
            border.color: "#4c4c4c"
            radius: 2
        }

        popup: Popup {
            y: dropdown.height - 1
            width: dropdown.width
            implicitHeight: contentItem.implicitHeight
            padding: 1

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: dropdown.popup.visible ? dropdown.delegateModel : null
                currentIndex: dropdown.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                color: "#3c3c3c"
                border.color: "#4c4c4c"
                radius: 2
            }
        }
    }
}
