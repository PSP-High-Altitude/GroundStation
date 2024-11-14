import QtQuick
import QtQuick.Controls
import "AppStyle"

Switch {
    implicitHeight: 30
    state: "OFF"

    states: [
        State {
            name: "OFF"
            PropertyChanges { target: custom_switch_back; color: AppStyle.switch_off}
            PropertyChanges { target: custom_switch_slider; x: 4}
        },
        State {
            name: "ON"
            PropertyChanges { target: custom_switch_back; color: AppStyle.switch_on}
            PropertyChanges { target: custom_switch_slider; x: (parent.width - width - 4)}
        }
    ]

    transitions: [
        Transition {
            from: "OFF"
            to: "ON"
            ColorAnimation { target: custom_switch_back; duration: 100}
            NumberAnimation { target: custom_switch_slider; properties: "x"; duration: 200; easing.type: Easing.InOutSine}
        },
        Transition {
            from: "ON"
            to: "OFF"
            ColorAnimation { target: custom_switch_back; duration: 100}
            NumberAnimation { target: custom_switch_slider; properties: "x"; duration: 200; easing.type: Easing.InOutSine}
        }
    ]

    onPressed: {
        // Actually checked
        if(!checked) {
            state = "ON"
        } else {
            state = "OFF"
        }
    }

    indicator: Rectangle {
        id: custom_switch_back
        height: parent.height
        width: height*2
        radius: width/2
        border.color: "white"
        border.width: 2

        Rectangle {
            id: custom_switch_slider
            width: parent.height - parent.border.width*2 - 4
            height: parent.height - parent.border.width*2 - 4
            radius: width/2
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
