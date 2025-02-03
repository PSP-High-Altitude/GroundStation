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
        let series = [...data_view.graph_series, ...data_view.table_only_series]
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
        data_table.updateRow(type)
    }

    property list<DataSeries> graph_series: [
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
            startChecked: true
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
        },
        DataSeries {
            id: flight_phase_series
            name: "Flight Phase"
            axisx: timeAxis
            axisy: phaseAxis
            color: "palegreen"
            precision: 0
            units: ""
            table_translation: ["Init", "Ready", "Boost", "Fast", "Coast", "Drogue", "Main", "Landed"]
        }
    ]

    property list<DataSeries> table_only_series: [
        DataSeries {
            id: lat_series
            name: "Latitude"
            precision: 8
            units: "°"
        },
        DataSeries {
            id: lon_series
            name: "Longitude"
            precision: 8
            units: "°"
        },
        DataSeries {
            id: gps_fix_series
            name: "GPS Fix"
            precision: 0
            table_translation: ["Invalid", "Valid"]
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
        rightMargin: 275
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
                color: "limegreen"
            },
            DataAxis {
                id: axisAcc

                label: "Acceleration (g)"
                min: -20
                max: 20
                color: "orange"
            },
            DataAxis {
                id: axisRssi

                label: "RSSI (dBm)"
                min: -150
                max: 0
                color: "violet"
            },
            DataAxis {
                id: phaseAxis

                label: "Flight Phase"
                min: 0
                max: 6
                color: "palegreen"
            }
        ]

        // Various data series
        series: data_view.graph_series
    }

    DataTable {
        id: data_table
        Layout.preferredWidth: 500
        Layout.fillHeight: true
        Layout.verticalStretchFactor: 1
        series: [...data_view.graph_series, ...data_view.table_only_series]
    }

    PSPCOMDecoder {
        id: decoder
        onRssiReceived: (rssi) => {
            addDataPoint("RSSI", (Date.now() - time_0_ms) / 1000.0, rssi)
        }
        onGpsPosReceived: (gps_pos, fix_ok) => {
            if(fix_ok) {
                addDataPoint("Altitude ASL (GPS)", (Date.now() - time_0_ms) / 1000.0, gps_pos.height_msl)
                addDataPoint("Latitude", (Date.now() - time_0_ms) / 1000.0, gps_pos.lat)
                addDataPoint("Longitude", (Date.now() - time_0_ms) / 1000.0, gps_pos.lon)
            }
        }
        onPresReceived: (pres) => {
            if(pres.pres < 10) {
                return
            }
            let height_msl = 44307.69396 * (1-((pres.pres/1013.25) ** 0.190284))
            addDataPoint("Altitude ASL (Baro)", (Date.now() - time_0_ms) / 1000.0, height_msl)
        }
        onSysStatusReceived: (status) => {
            addDataPoint("Flight Phase", (Date.now() - time_0_ms) / 1000.0, status.flight_phase)
            addDataPoint("GPS Fix", (Date.now() - time_0_ms) / 1000.0, status.gps_fix_ok)
            //Q_PROPERTY(uint8_t gps_fix_ok  MEMBER m_gps_fix_ok)
            //Q_PROPERTY(uint8_t storage_ok MEMBER m_storage_ok)
            //Q_PROPERTY(uint8_t ins_ok  MEMBER m_ins_ok)
            //Q_PROPERTY(uint8_t flight_phase MEMBER m_flight_phase)
            //Q_PROPERTY(uint8_t error_flag  MEMBER m_error_flag)
        }
    }

    Component.onCompleted: {
        time_0_ms = Date.now()
        settings.activeDeviceMessageReceived.connect((msg) => {
            decoder.processMessage(msg)
        })
    }
}
