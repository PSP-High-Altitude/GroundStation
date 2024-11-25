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

    ChartView {
        id: data_chart
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.horizontalStretchFactor: 1
        Layout.verticalStretchFactor: 1
        legend.visible: true

        LineSeries {
            id: altitude_series
            name: "Altitude"
            onPointAdded: function(index) {
                let x = this.at(index).x
                let y = this.at(index).y
                // create lower and upper rounded values
                let hiX = Math.ceil(x/10)*10 + 10
                let lowX = Math.floor(x/10)*10 - 10
                let hiY = Math.ceil(y/10)*10 + 10
                let lowY = Math.floor(y/10)*10 - 10
                // find new min and max
                if(x < minX) minX = lowX
                if(x > maxX) maxX = hiX
                if(y < minY) minY = lowY
                if(y > maxY) maxY = hiY

                if(!autoScale) {
                    // Don't actually rescale if this is false
                    return
                }

                // rescale axes
                this.axisX.min = minX
                this.axisX.max = maxX
                this.axisY.min = minY
                this.axisY.max = maxY
            }
        }

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
                    onTriggered: { data_view.autoScale = true }
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
            time++
        }
    }
}
