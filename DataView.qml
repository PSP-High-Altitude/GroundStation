import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts

RowLayout {
    id: data_view
    Layout.topMargin: 10
    Layout.leftMargin: 10
    Layout.bottomMargin: 10
    Layout.rightMargin: 10
    Layout.fillHeight: true
    Layout.fillWidth: true
    property int time: 0
    property int minX: 0
    property int maxX: 0
    property int minY: 0
    property int maxY: 0
    property bool autoScale: true
    function addDataPoint(type, x, y) {
        data_chart.series(type).append(x, y)
    }

    // Chart of different flight data
    ChartView {
        id: data_chart
        objectName: "data_chart"
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.horizontalStretchFactor: 1
        Layout.verticalStretchFactor: 1
        //legend.visible: true
        antialiasing: true

        //ValuesAxis {
        //    id: axisAlt
        //    Component.onCompleted: {
        //        console.log(this)
        //    }
        //}

        //ValuesAxis {
        //    id: axisSpd
        //    Component.onCompleted: {
        //        console.log(this)
        //    }
        //}

        // Various data series
        DataSeries {
            id: altitude_series
            name: "Altitude"
            //axisY: axisAlt
        }
        DataSeries {
            id: speed_series
            name: "Speed"
            //axisYRight: axisSpd
        }

        // Provide mouse area for chart interaction
        MouseArea {
            id: chart_mouse
            property int dStartX: 0
            property int dStartY: 0
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            onPressed: {
                dStartX = chart_mouse.mouseX
                dStartY = chart_mouse.mouseY
            }

            function moveAxes() {
                if(containsPress) {
                    autoScale = false
                    let dx = chart_mouse.mouseX - dStartX
                    let dy = chart_mouse.mouseY - dStartY
                    data_chart.scrollLeft(dx)
                    data_chart.scrollUp(dy)
                }
            }

            onMouseXChanged: {
                moveAxes()
                dStartX = chart_mouse.mouseX
                dStartY = chart_mouse.mouseY
            }

            onMouseYChanged: {
                moveAxes()
                dStartX = chart_mouse.mouseX
                dStartY = chart_mouse.mouseY
            }

            onClicked: function(mouse) {
                if (mouse.button === Qt.RightButton)
                    contextMenu.popup()
            }

            onWheel: function(wheel) {
                autoScale = false
                if(wheel.angleDelta.y < 0) {
                    data_chart.zoomOut()
                } else {
                    data_chart.zoomIn()
                }
            }

            Menu {
                id: contextMenu
                MenuItem {
                    text: "Autoscale Axes"
                    onTriggered: {
                        data_view.autoScale = true
                        for(var i = 0; i < data_chart.count; i++) {
                            data_chart.series(i).rescaleAxes()
                        }
                    }
                }
            }
        }
    }

    DataTable {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.horizontalStretchFactor: 1
        Layout.verticalStretchFactor: 1
    }

    Timer {
        interval: 500
        repeat: true
        running: true
        onTriggered: {
            var y = Math.floor(Math.random() * 100)
            data_view.addDataPoint("Altitude", time, y)
            y = Math.floor(Math.random() * 1000)
            data_view.addDataPoint("Speed", time, y)
            time++
        }
    }
}
