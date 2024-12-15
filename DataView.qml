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
    function addDataPoint(type, x, y) {
        // Block NANs
        if(isNaN(x) || isNaN(y)) {
            return
        }

        if(data_chart.getSeries(type)) {
            data_chart.getSeries(type).addPoint(x, y)
        }
        data_table.setRow(type, y)
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

        leftMargin: 75
        rightMargin: 200
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
            }
        ]

        // Various data series
        series: [
            DataSeries {
                id: altitude_series
                name: "Altitude"
                axisx: timeAxis
                axisy: axisAlt
                color: "white"
            },
            DataSeries {
                id: acc_x_series
                name: "Acceleration (x)"
                axisx: timeAxis
                axisy: axisAcc
                color: "red"
            },
            DataSeries {
                id: acc_y_series
                name: "Acceleration (y)"
                axisx: timeAxis
                axisy: axisAcc
                color: "orangered"
            },
            DataSeries {
                id: acc_z_series
                name: "Acceleration (z)"
                axisx: timeAxis
                axisy: axisAcc
                color: "chocolate"
            }
        ]
    }

    DataTable {
        id: data_table
        Layout.preferredWidth: 500
        Layout.fillHeight: true
        Layout.verticalStretchFactor: 1
    }

    FileIO {
        id: test_file
        source: "C:/Users/griff/OneDrive/Documents/Purdue Space Program/skyshot test/dat_00_trimmed_zero_start.csv"
    }

    Component.onCompleted: {
        test_file.open()
    }

    Timer {
        interval: 50
        repeat: true
        running: true
        onTriggered: {
            const line = test_file.readLine()
            if(line === "") {
                console.log("out of data")
                running = false
            }

            const data = line.split(',')
            const timestamp = Number(data[0])
            const pressure = Number(data[2])
            const acc_x = Number(data[3]) * 9.8
            const acc_y = Number(data[4]) * 9.8
            const acc_z = Number(data[5]) * 9.8
            const alt = (1 - (pressure/1013.25) ** 0.190284) * 145366.45

            data_view.addDataPoint("Altitude", timestamp/1e6, alt)
            data_view.addDataPoint("Acceleration (x)", timestamp/1e6, acc_x)
            data_view.addDataPoint("Acceleration (y)", timestamp/1e6, acc_y)
            data_view.addDataPoint("Acceleration (z)", timestamp/1e6, acc_z)

            for(let i = 0; i < 39; i++) {
                test_file.readLine()
            }
            time++
        }
    }
}
