import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts
import FileIO 1.0

RowLayout {
    id: data_view
    Layout.topMargin: 10
    Layout.leftMargin: 10
    Layout.bottomMargin: 10
    Layout.rightMargin: 10
    Layout.fillHeight: true
    Layout.fillWidth: true
    property int time: 0
    property bool autoScale: true
    function addDataPoint(type, x, y) {
        if(data_chart.getSeries(type)) {
            data_chart.getSeries(type).addPoint(x, y)
        }
    }

    // Chart of different flight data
    DataGraph {
        id: data_chart
        objectName: "data_chart"
        title: "Live Data"
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.horizontalStretchFactor: 1
        Layout.verticalStretchFactor: 1
        //legend.visible: true
        //antialiasing: true

        leftMargin: 75
        rightMargin: 200
        topMargin: 50

        xaxis: DataAxis {
            id: timeAxis
            min: 0
            max: 10
            label: "Time (s)"
        }

        yaxis: [
            DataAxis {
                id: axisAlt

                label: "Altitude (m)"
            },
            DataAxis {
                id: axisSpd

                label: "Speed (m/s)"
                min: -10
                max: 300
                //spacing: 20
                color: "red"
            },
            DataAxis {
                id: axisAcc

                label: "Acceleration (m/s^2)"
                min: -20
                max: 20
                //spacing: 4
                color: "orange"
            }
        ]

        // Various data series
        series: [
            DataSeries {
                id: altitude_series
                name: "Altitude"
                axisx: timeAxis
                axisy: axisAlt
                color: "black"
            },
            DataSeries {
                id: speed_series
                name: "Speed"
                axisx: timeAxis
                axisy: axisSpd
                color: "red"
            }
        ]

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
                    if(dx) {
                        dStartX = chart_mouse.mouseX
                    }
                    if(dy) {
                        dStartY = chart_mouse.mouseY
                    }
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
                        data_chart.rescaleAxis(data_chart.xaxis)
                        for(var i = 0; i < data_chart.yaxis.length; i++) {
                            data_chart.rescaleAxis(data_chart.yaxis[i])
                        }
                    }
                }
            }
        }
    }

    //DataTable {
    //    Layout.fillWidth: true
    //    Layout.fillHeight: true
    //    Layout.horizontalStretchFactor: 1
    //    Layout.verticalStretchFactor: 1
    //}

    FileIO {
        id: test_file
        source: "D:/github_repos/PSP-HA-Firmware/data/dm3/dm3_hwil_dat.csv"
    }

    Component.onCompleted: {
        test_file.open()
    }

    Timer {
        interval: 25//500
        repeat: true
        running: true
        onTriggered: {
            const line = test_file.readLine()
            const data = line.split(',')
            const timestamp = Number(data[0])
            const pressure = Number(data[2])
            const alt = (1 - (pressure/1013.25) ** 0.190284) * 145366.45

            data_view.addDataPoint("Altitude", timestamp/1e6, alt)
            time++
        }
    }
}
