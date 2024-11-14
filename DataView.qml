import QtQuick
import QtQuick.Layouts
import QtCharts

RowLayout {
    Layout.topMargin: 10
    Layout.leftMargin: 10
    Layout.bottomMargin: 10
    Layout.rightMargin: 10
    Layout.fillHeight: true
    Layout.fillWidth: true

    ChartView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.horizontalStretchFactor: 1
        Layout.verticalStretchFactor: 1

        LineSeries {
            name: "Line"
            XYPoint { x: 0; y: 0 }
            XYPoint { x: 1.1; y: 2.1 }
            XYPoint { x: 1.9; y: 3.3 }
            XYPoint { x: 2.1; y: 2.1 }
            XYPoint { x: 2.9; y: 4.9 }
            XYPoint { x: 3.4; y: 3.0 }
            XYPoint { x: 4.1; y: 3.3 }
        }
    }

    DataTable {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.horizontalStretchFactor: 1
        Layout.verticalStretchFactor: 1
    }
}
