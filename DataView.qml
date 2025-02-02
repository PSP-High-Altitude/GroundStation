import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtCharts
import FileIO 1.0
import AppSettings 1.0
import Device 1.0
import PSPCOMDecoder 1.0

RowLayout {
    id: data_view
    required property AppSettings settings
    Layout.topMargin: 10
    Layout.leftMargin: 10
    Layout.bottomMargin: 10
    Layout.rightMargin: 10
    Layout.fillHeight: true
    Layout.fillWidth: true
    property real time_0_ms: 0

    function getSeries(name) {
        for(var i = 0; i < series.length; i++) {
            if(series[i].name === name) {
                return series[i]
            }
        }
        return null
    }

    function addDataPoint(type, x, y) {
        // Block NANs
        if(isNaN(x) || isNaN(y)) {
            return
        }

        if(data_view.getSeries(type)) {
            data_view.getSeries(type).addPoint(x, y)
        }
        data_table.setRow(type, y)
    }

    property list<DataSeries> series: [
        DataSeries {
            id: altitude_gps_series
            name: "Altitude ASL (GPS)"
            axisx: timeAxis
            axisy: axisAlt
            color: "white"
            precision: 2
            units: "m"
        },
        DataSeries {
            id: altitude_baro_series
            name: "Altitude ASL (Baro)"
            axisx: timeAxis
            axisy: axisAlt
            color: "lightgray"
            precision: 2
            units: "m"
        },
        DataSeries {
            id: acc_x_series
            name: "Acceleration (x)"
            axisx: timeAxis
            axisy: axisAcc
            color: "red"
            precision: 2
            units: "g"
        },
        DataSeries {
            id: acc_y_series
            name: "Acceleration (y)"
            axisx: timeAxis
            axisy: axisAcc
            color: "orangered"
            precision: 2
            units: "g"
        },
        DataSeries {
            id: acc_z_series
            name: "Acceleration (z)"
            axisx: timeAxis
            axisy: axisAcc
            color: "chocolate"
            precision: 2
            units: "g"
        },
        DataSeries {
            id: rssi_series
            name: "RSSI"
            axisx: timeAxis
            axisy: axisRssi
            color: "violet"
            precision: 2
            units: "dBm"
        }
    ]

    // Chart of different flight data
    DataGraph {
        id: data_chart
        objectName: "data_chart"
        title: "Live Data"
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.horizontalStretchFactor: 1
        Layout.verticalStretchFactor: 1

        leftMargin: 75
        rightMargin: 220
        topMargin: 50
        bottomMargin: 20

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
                color: "lime"
            },
            DataAxis {
                id: axisAcc

                label: "Acceleration (m/s^2)"
                min: -20
                max: 20
                color: "orangered"
            },
            DataAxis {
                id: axisRssi

                label: "RSSI (dBm)"
                min: -150
                max: 0
                color: "violet"
            }
        ]

        // Various data series
        series: data_view.series
    }

    DataTable {
        id: data_table
        Layout.preferredWidth: 500
        Layout.fillHeight: true
        Layout.verticalStretchFactor: 1
        series: data_view.series
    }

    PSPCOMDecoder {
        id: decoder
        onRssiReceived: (rssi) => {
            addDataPoint("RSSI", (Date.now() - time_0_ms) / 1000.0, rssi);
        }
        onGpsPosReceived: (gps_pos) => {
            addDataPoint("Altitude ASL (GPS)", (Date.now() - time_0_ms) / 1000.0, gps_pos.height_msl);
        }
        onPresReceived: (pres) => {
            if(pres < 10) {
                return
            }
            let height_msl = 44307.69396 * (1-((pres.pres/1013.25) ** 0.190284))
            addDataPoint("Altitude ASL (Baro)", (Date.now() - time_0_ms) / 1000.0, height_msl);
        }
    }

    Component.onCompleted: {
        time_0_ms = Date.now()
        settings.activeDeviceMessageReceived.connect((msg) => {
            decoder.processMessage(msg)
        })
    }
}
