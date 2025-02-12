import QtQuick

Item {
    id: root
    implicitHeight: 60
    implicitWidth: 120
    state: "OPEN"
    property color color_on: AppStyle.switch_on
    property color color_off: AppStyle.switch_off
    property string pyro_name: "Pyro ?"

    states: [
        State {
            name: "OPEN"
            PropertyChanges { target: indicator_base; color: color_off }
            PropertyChanges { target: indicator_state_label; text: "OPEN" }
        },
        State {
            name: "SHORT"
            PropertyChanges { target: indicator_base; color: color_on }
            PropertyChanges { target: indicator_state_label; text: "SHORT" }
        }
    ]

    transitions: [
        Transition {
            from: "OPEN"
            to: "SHORT"
            ColorAnimation { target: indicator_base; duration: 100}
        },
        Transition {
            from: "SHORT"
            to: "OPEN"
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
            text: "OPEN"
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            y: parent.y + parent.height - 30
            font.pointSize: 12
            font.bold: true
        }
    }
}
