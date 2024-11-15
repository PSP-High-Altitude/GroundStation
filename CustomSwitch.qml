import QtQuick
import QtQuick.Controls
import QtQuick.Particles
import "AppStyle"

Switch {
    id: root
    implicitHeight: 30
    state: "OFF"
    property color color_on: AppStyle.switch_on
    property color color_off: AppStyle.switch_off
    property color color_wait: AppStyle.switch_wait

    states: [
        State {
            name: "OFF"
            PropertyChanges { target: custom_switch_back; color: color_off}
            PropertyChanges { target: custom_switch_slider; x: 4}
        },
        State {
            name: "ON"
            PropertyChanges { target: custom_switch_back; color: color_on}
            PropertyChanges { target: custom_switch_slider; x: (parent.width - width - 4)}
        },
        State {
            name: "WAIT"
            PropertyChanges { target: custom_switch_back; color: color_wait}
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
            from: "OFF"
            to: "WAIT"
            ColorAnimation { target: custom_switch_back; duration: 100}
            NumberAnimation { target: custom_switch_slider; properties: "x"; duration: 200; easing.type: Easing.InOutSine}
        },
        Transition {
            from: "ON"
            to: "OFF"
            ColorAnimation { target: custom_switch_back; duration: 100}
            NumberAnimation { target: custom_switch_slider; properties: "x"; duration: 200; easing.type: Easing.InOutSine}
        },
        Transition {
            from: "WAIT"
            to: "OFF"
            ColorAnimation { target: custom_switch_back; duration: 100}
            NumberAnimation { target: custom_switch_slider; properties: "x"; duration: 200; easing.type: Easing.InOutSine}
        }
    ]

    onPressed: {
        // Actually checked
        if(!checked) {
            state = "WAIT"
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

    Repeater {
        id: loading_indicator
        anchors.fill: parent
        model: 4
        property real rotation: 0.0
        delegate: Rectangle {
            width: index+1
            height: index+1
            radius: 180
            color: "white"
            x: custom_switch_back.height/2 - width/2 + 7*Math.cos((2-index + loading_indicator.rotation)*Math.PI*0.3)
            y: custom_switch_back.height/2 - height/2 + 7*Math.sin((2-index + loading_indicator.rotation)*Math.PI*0.3)
            visible: root.state === "WAIT"
        }
    }

    Timer {
        interval: 20
        repeat: true
        running: true
        onTriggered: {
            loading_indicator.rotation -= 0.1
        }
    }
}
