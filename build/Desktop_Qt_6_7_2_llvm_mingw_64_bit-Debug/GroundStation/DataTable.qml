import QtQuick
import QtQuick.Layouts
import Qt.labs.qmlmodels

TableView {
    id: table_view
    columnSpacing: 1
    rowSpacing: 1
    clip: true
    boundsMovement: Flickable.StopAtBounds
    columnWidthProvider: function (column) {
        return table_view.width / table_view.columns
    }

    model: TableModel {
        TableModelColumn {
            display: "name"
        }
        TableModelColumn {
            display: "color"
        }

        rows: [
            {
                "name": "cat",
                "color": "black"
            },
            {
                "name": "dog",
                "color": "brown"
            },
            {
                "name": "bird",
                "color": "white"
            }
        ]
    }

    delegate: Rectangle {
        implicitHeight: 20
        implicitWidth: 20
        border.width: 1

        Text {
            text: display
            anchors.centerIn: parent
        }
    }
}
