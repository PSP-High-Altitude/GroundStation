import QtQuick
import QtCharts

Item {
    property string name: "Series"
    property bool startChecked: false
    property DataAxis axisx
    property DataAxis axisy
    property list<point> data
    property list<string> table_translation: [] // If defined, maps ints to strings (e.g. flight phase names)
    property string units: ""
    property int precision: 2
    property color color: "black"
    signal pointAdded()
    property real xmin: NaN
    property real xmax: NaN
    property real ymin: NaN
    property real ymax: NaN
    property Item chart

    function addPoint(x, y) {
        // add point
        data.push(Qt.point(x, y))

        // find new min and max
        if(isNaN(xmin) || x < xmin) xmin = x
        if(isNaN(xmax) || x > xmax) xmax = x
        if(isNaN(ymin) || y < ymin) ymin = y
        if(isNaN(ymax) || y > ymax) ymax = y

        // rescale axes
        if(chart) {
            if(chart.autoScale) {
                if(axisx) {
                    chart.rescaleAxis(axisx)
                }
                if(axisy) {
                    chart.rescaleAxis(axisy)
                }
            }
        }

        pointAdded()
    }

    function rescaleAxes() {
        if(axisx == null || axisy == null) {
            return
        }

        // rescale axes
        chart.rescaleAxis(axisx)
        chart.rescaleAxis(axisy)
        chart.repaint()
    }
}
