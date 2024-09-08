import QtQuick 2.0
import QtQml
import QtQuick.Controls

Window {
    id: altimeter_window
    visible: true
    flags: Qt.FramelessWindowHint
    color: "#2c2c2c"

    readonly property int vertical_division: 100
    property int current_max_alt: altimeter_height
    property int current_min_alt: 0
    property int current_displayed_altitude: 0
    property int current_max_idx: 0

    Rectangle {
        border.color: "white"
        border.width: 2
        anchors.fill: parent
        width: altimeter_window.width
        height: altimeter_window.height
        color: "transparent"
        z: 3
    }

    Rectangle {
        id: sky
        anchors.left: parent.left
        anchors.leftMargin: 2
        y: 2
        width: 118
        height: altimeter_window.height - 4
        color: "#79BFD2"
        opacity: 1
        z: 2
    }

    Rectangle {
        id: ground
        anchors.left: parent.left
        anchors.leftMargin: 2
        y: 2
        width: 118
        height: altimeter_window.height - 4
        color: "#47310b"
        opacity: 1
        z: 1
    }

    Repeater {
        id: tick_repeater
        model: tick_list
        delegate: Rectangle {
            id: tick_rect
            x: modelData.rect.x
            y: modelData.rect.y
            width: modelData.rect.width
            height: modelData.rect.height
            color: "white"
            z: 3

            Text {
                id: tick_text
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                width: altimeter_window.width
                x: 50
                text: modelData.text
                color: "white"
                font.pointSize: 12
                z: 3
            }
        }
    }

    Image {
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.verticalCenter: parent.verticalCenter
        source: "../images/Resources/alt_marker.png"
        z: 3

        Text {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: current_displayed_altitude + " m"
            color: "white"
            font.pointSize: 14
            z: 3
        }
    }

    Component.onCompleted: {
        for(var i = 0; i < tick_repeater.count; i++) {
            tick_repeater.itemAt(i).children[0].text = (altimeter_height - vertical_division*i)
        }
        current_min_alt = altimeter_height - vertical_division*(tick_repeater.count - 1)
    }

    Timer {
        interval: timer_period
        running: true
        repeat: true
        onTriggered: {
            var altitude_copy = altitude
            for(var i = 0; i < tick_repeater.count; i++) {
                if(Number(tick_repeater.itemAt(i).children[0].text) === current_max_alt)
                {
                    current_max_idx = i
                    current_displayed_altitude = current_max_alt + ((tick_repeater.itemAt(i).y - (altimeter_window.height/2)) / tick_spacing * vertical_division)
                }
            }

            for(i = 0; i < tick_repeater.count; i++) {
                var change_amount = ((current_displayed_altitude - altitude_copy) * tick_spacing * timer_period) / (300 * vertical_division)
                if(change_amount > tick_spacing)
                {
                    tick_repeater.itemAt(i).y -= tick_spacing
                }
                else if(change_amount < -tick_spacing)
                {
                    tick_repeater.itemAt(i).y += tick_spacing
                }
                else
                {
                    tick_repeater.itemAt(i).y -= change_amount
                }

                if(tick_repeater.itemAt(i).y > altimeter_height)
                {
                    tick_repeater.itemAt(i).y -= altimeter_height
                    tick_repeater.itemAt(i).children[0].text = current_max_alt + vertical_division
                    current_max_alt += vertical_division
                    current_min_alt += vertical_division
                }
                if(tick_repeater.itemAt(i).y < 0)
                {
                    tick_repeater.itemAt(i).y += altimeter_height
                    tick_repeater.itemAt(i).children[0].text = current_min_alt - vertical_division
                    current_min_alt -= vertical_division
                    current_max_alt -= vertical_division
                }
            }

            if(ground_alt > current_min_alt && ground_alt < current_max_alt)
            {
                var ground_pos = tick_repeater.itemAt(current_max_idx).y + (current_max_alt - ground_alt) * (tick_spacing / vertical_division);
                sky.y = 2
                sky.height = ground_pos - 2
            }

            if(ground_alt < current_min_alt)
            {
                sky.y = 2
                sky.height = altimeter_height - 4
            }

            if(ground_alt > current_max_alt)
            {
                sky.y = altimeter_height
            }
        }
    }
}
