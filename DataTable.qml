import QtQuick
import QtQuick.Layouts
import Qt.labs.qmlmodels
import "AppStyle"

ColumnLayout {
    property list<DataSeries> series

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
            rows: []
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

    function getNewValue(series) {
        let precision = series.precision
        var value_str = ""

        if(series.data.length > 0 && precision > 0) {
            // Get the latest value, with decimals
            value_str = series.data[series.data.length-1].y.toFixed(precision)
        } else if (series.data.length > 0) {
            // Get the latest value, no decimals
            value_str = Math.round(series.data[series.data.length-1].y).toString()
        }
        if(series.data.length > 0 && series.table_translation.length > 0) {
            // Get the string this value is mapped to
            if(series.data[series.data.length-1].y >= 0 && series.data[series.data.length-1].y < series.table_translation.length) {
                // Within range, so get the translation
                value_str = series.table_translation[Math.round(series.data[series.data.length-1].y)]
            } else {
                // Outside of range, display as unknown
                value_str += " (Unknown)"
            }
        }
        return value_str
    }

    function updateRow(name) {
        for(var i = 0; i < table_model.rowCount; i++) {
            if(table_model.getRow(i).name === name) {
                let value_str = getNewValue(series[i])

                // Modify row
                var row = table_model.getRow(i)
                row.value = value_str
                table_model.setRow(i, row)
            }
        }
    }

    Component.onCompleted: {
        for (let the_series of series) {
            let value_str = getNewValue(the_series)

            table_model.appendRow({ "name" : the_series.name, "value" : value_str, "units" : the_series.units })
        }
    }
}
