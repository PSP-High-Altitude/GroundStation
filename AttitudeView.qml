import QtQuick
import QtQuick.Layouts
import PSPCOMDecoder 1.0
import AppSettings 1.0

ColumnLayout {
    Layout.topMargin: 10
    Layout.leftMargin: 10
    Layout.bottomMargin: 10
    Layout.rightMargin: 10
    Layout.fillHeight: true
    Layout.fillWidth: true
    required property AppSettings settings

    RowLayout {
        Layout.topMargin: 10
        Layout.leftMargin: 10
        Layout.bottomMargin: 10
        Layout.rightMargin: 10
        Layout.fillHeight: true
        Layout.fillWidth: true

        PSPMap {
            id: map
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.horizontalStretchFactor: 1
            Layout.verticalStretchFactor: 1
        }

        TapeAltimeter {
            id: tape_alt
            altitude: 190
            ground_alt: 190
            Layout.fillHeight: true
            Layout.verticalStretchFactor: 1
        }
    }

    RowLayout {
        Layout.topMargin: 10
        Layout.leftMargin: 10
        Layout.bottomMargin: 10
        Layout.rightMargin: 10
        Layout.fillHeight: true
        Layout.fillWidth: true

        GridLayout {
            id: pyro_layout
            columns: 3

            Repeater {
                id: pyro_repeater
                property list<string> pyro_name_arr: ["Main", "Drogue", "A1", "A2", "A3", "A4"]
                model: 6
                delegate: PyroIndicator {
                    pyro_name: "Pyro " + pyro_repeater.pyro_name_arr[index]
                }
            }
        }
    }

    PSPCOMDecoder {
        id: decoder
        onGpsPosReceived: (gps_pos, fix_ok) => {
            if(fix_ok) {
                map.setMarker(gps_pos.lat, gps_pos.lon)
            }
        }
        onPresReceived: (pres) => {
            if(pres.pres < 10) {
                return
            }
            let height_msl = 44307.69396 * (1-((pres.pres/1013.25) ** 0.190284))
            tape_alt.altitude = height_msl
        }
        onPyroStatusReceived: (pyro) => {
            if(pyro.num_pyro_stat === 1) {
                pyro_repeater.itemAt(0).state = pyro.pyro_stat_0.p0_cont ? "SHORT" : "OPEN"
                pyro_repeater.itemAt(1).state = pyro.pyro_stat_0.p1_cont ? "SHORT" : "OPEN"
                pyro_repeater.itemAt(2).state = pyro.pyro_stat_0.p2_cont ? "SHORT" : "OPEN"
                pyro_repeater.itemAt(3).state = pyro.pyro_stat_0.p3_cont ? "SHORT" : "OPEN"
            } else if(pyro.num_pyro_stat > 1) {
                pyro_repeater.itemAt(0).state = pyro.pyro_stat_0.p0_cont ? "SHORT" : "OPEN"
                pyro_repeater.itemAt(1).state = pyro.pyro_stat_0.p1_cont ? "SHORT" : "OPEN"
                pyro_repeater.itemAt(2).state = pyro.pyro_stat_0.p2_cont ? "SHORT" : "OPEN"
                pyro_repeater.itemAt(3).state = pyro.pyro_stat_0.p3_cont ? "SHORT" : "OPEN"
                pyro_repeater.itemAt(4).state = pyro.pyro_stat_1.p0_cont ? "SHORT" : "OPEN"
                pyro_repeater.itemAt(5).state = pyro.pyro_stat_1.p1_cont ? "SHORT" : "OPEN"
            }
        }
    }

    Component.onCompleted: {
        settings.activeDeviceMessageReceived.connect((msg) => {
            decoder.processMessage(msg)
        })
    }
}
