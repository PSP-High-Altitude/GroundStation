import QtQuick
import QtQuick.Layouts
import Qt.labs.qmlmodels

Item {
    TableView {
        id: table_view
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true
        boundsMovement: Flickable.StopAtBounds
        columnWidthProvider: function (column) {
            var units_col_width = Math.min(50, table_view.width)
            var name_col_width = Math.max((table_view.width - units_col_width)/3, 0)
            var val_col_width = Math.max(table_view.width - units_col_width - name_col_width, 0)
            switch(column) {
            case 0:
                return name_col_width
            case 1:
                return val_col_width
            case 2:
                return units_col_width
            default:
                return 0
            }
        }


        model: TableModel {
            TableModelColumn {
                display: "name"
            }
            TableModelColumn {
                display: "value"
            }
            TableModelColumn {
                display: "units"
            }

            rows: [
                {
                    "name": "Altitude",
                    "value": "0",
                    "units": "m"
                },
                {
                    "name": "Latitude",
                    "value": "1",
                    "units": "°"
                },
                {
                    "name": "Longitude",
                    "value": "2",
                    "units": "°"
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
}
