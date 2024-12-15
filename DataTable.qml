import QtQuick
import QtQuick.Layouts
import Qt.labs.qmlmodels
import "AppStyle"

ColumnLayout {
    Rectangle {
        Layout.preferredHeight: 50
        color: "transparent"
    }

    TableView {
        id: table_view
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.horizontalStretchFactor: 1
        Layout.verticalStretchFactor: 1
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
                return name_col_width - 1
            case 1:
                return val_col_width - 1
            case 2:
                return units_col_width - 1
            default:
                return 0
            }
        }


        model: TableModel {
            id: table_model
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
            border.color: "white"
            color: AppStyle.window

            Text {
                color: "white"
                text: display
                anchors.centerIn: parent
            }
        }
    }

    function setRow(type, value) {
        for(var i = 0; i < table_model.rowCount; i++) {
            if(table_model.getRow(i).name === type) {
                table_model.setRow(0, {
                    name: type,
                    value: value,
                    units: table_model.getRow(i).units,
                })
            }
        }
    }
}
