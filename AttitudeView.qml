import QtQuick
import QtQuick.Layouts

RowLayout {
    Layout.topMargin: 10
    Layout.leftMargin: 10
    Layout.bottomMargin: 10
    Layout.rightMargin: 10
    Layout.fillHeight: true
    Layout.fillWidth: true

    PSPMap {
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
