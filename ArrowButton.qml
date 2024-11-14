import QtQuick
import QtQuick.Shapes


Item {
    id: arrow_button_container
    signal clicked()
    property string direction
    Shape {
        anchors.centerIn: parent
        ShapePath {
            strokeWidth: 8
            strokeColor: "white"
            fillColor: "transparent"

            joinStyle: ShapePath.RoundJoin
            capStyle: ShapePath.RoundCap

            startY: (direction === "left") ? arrow_button_container.height/2 : arrow_button_container.height

            // Define the path segments
            PathLine { x: (direction === "left") ? arrow_button_container.width : 0; y: arrow_button_container.height }
            PathLine { x: (direction === "left") ? 0 : arrow_button_container.width; y: arrow_button_container.height/2 }
            PathLine { x: (direction === "left") ? arrow_button_container.width : 0; y: 0 }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            arrow_button_container.clicked()
        }
    }
}
