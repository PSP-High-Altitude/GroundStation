import QtQuick

Item {
    id: root
    implicitHeight: 60
    implicitWidth: 120
    state: off_state
    property string on_state: "ON"
    property string off_state: "OFF"
    property color color_on: AppStyle.switch_on
    property color color_off: AppStyle.switch_off
    property string pyro_name: "Pyro ?"

    states: [
        State {
            name: off_state
            PropertyChanges { target: indicator_base; color: color_off }
            PropertyChanges { target: indicator_state_label; text: off_state }
        },
        State {
            name: on_state
            PropertyChanges { target: indicator_base; color: color_on }
            PropertyChanges { target: indicator_state_label; text: on_state }
        }
    ]

    transitions: [
        Transition {
            from: off_state
            to: color_on
            ColorAnimation { target: indicator_base; duration: 100}
        },
        Transition {
            from: color_on
            to: off_state
            ColorAnimation { target: indicator_base; duration: 100}
        }
    ]

    Rectangle {
        id: indicator_base
        border.color: "white"
        border.width: 2
        anchors.fill: parent
        radius: 5
        color: color_off

        Text {
            width: parent.width
            text: pyro_name
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            y: parent.y + 5
            font.pointSize: 12
            font.bold: true
        }

        Text {
            id: indicator_state_label
            width: parent.width
            text: off_state
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            y: parent.y + parent.height - 30
            font.pointSize: 12
            font.bold: true
        }
    }
}
