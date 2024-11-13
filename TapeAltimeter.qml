import QtQuick 2.0
import QtQml
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

Rectangle {
    id: altimeter_window
    visible: true
    width: 200

    property int vertical_division: 100
    property int tick_spacing: 75
    property int tick_interval: 100
    property int altitude: 0
    property int ground_alt: 0
    property int max_alt: 500000
    property int min_alt: -5000
    property int current_displayed_altitude: 0
    property int current_max_idx: 0
    property int timer_period: 10

    color: "#2c2c2c"

    Rectangle {
        id: alt_border
        border.color: "white"
        border.width: 2
        anchors.fill: parent
        color: "transparent"
        z: 3
    }

    Rectangle {
        id: sky
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 2
        y: 2
        width: altimeter_window.width - 4
        height: altimeter_window.height - 4
        color: "#79BFD2"
        opacity: 1
        z: 2
    }

    Rectangle {
        id: ground
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 2
        y: 2
        width: altimeter_window.width - 4
        height: altimeter_window.height - 4
        color: "#47310b"
        opacity: 1
        z: 1
    }

    ListView {
        id: tick_repeater
        interactive: false
        model: (max_alt-min_alt)
        z: 3

        verticalLayoutDirection: ListView.BottomToTop
        anchors.fill: parent
        clip: true
        delegate: Item {
            z: 3
            height: tick_spacing
            width: altimeter_window.width - 4
            Rectangle {
                id: tick_rect_left
                y: 0
                anchors.left: parent.left
                anchors.right: tick_text.left
                anchors.rightMargin: 10
                height: 2
                color: "white"
            }

            Rectangle {
                id: tick_rect_right
                y: 0
                anchors.left: tick_text.right
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: -2
                height: 2
                color: "white"
            }

            Text {
                id: tick_text
                anchors.verticalCenter: tick_rect_left.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 38
                horizontalAlignment: Text.AlignRight
                text: "" + (index * tick_interval + min_alt)
                color: "white"
                font.pointSize: 14
            }
        }
        Component.onCompleted: {
            tick_repeater.contentY = min_alt/tick_interval*tick_spacing
        }
    }

    Image {
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.verticalCenter: parent.verticalCenter
        source: "resources/alt_marker.png"
        z: 3

        Text {
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            text: current_displayed_altitude + " m"
            color: "white"
            font.pointSize: 14
            z: 3
        }
    }

    Timer {
        interval: timer_period
        running: true
        repeat: true
        onTriggered: {
            var dist = (altitude - ((((-1*tick_repeater.contentY - tick_repeater.height/2)/tick_spacing)*tick_interval + (min_alt-tick_interval))))

            // cutoff so we don't just keep spamming movement
            if(Math.abs(dist) < 0.5) {
                return
            } else {
                // sigmoid function
                var speed = 100 / (1 + Math.E**(-0.001*dist*(tick_spacing/tick_interval))) - 50
            }

            // set altitude indicator
            current_displayed_altitude = (((-1*tick_repeater.contentY - tick_repeater.height/2)/tick_spacing)*tick_interval + (min_alt-tick_interval))

            // set sky height
            sky.height = Math.min(altimeter_window.height-4, (altimeter_window.height)/2 + ((current_displayed_altitude - ground_alt)*(tick_spacing/tick_interval)))

            // set sky color
            var skyObject = Qt.createQmlObject('
                import QtQuick 2.0
                Gradient {
                    GradientStop { id: "gStop1"; position: 1.0; color: "blue" }
                    GradientStop { id: "gStop2"; position: 0.0; color: "blue" }
                }
                ',
               sky,
               "dynamicSnippet1");
            skyObject.stops[0].color = Qt.hsva((230-(Math.E**(3.6-0.0001*current_displayed_altitude)))/360, 60.0/100, (82/100)*(Math.E**(-0.1-0.0001*current_displayed_altitude))+0.15, 1.0)
            skyObject.stops[1].color = Qt.hsva((230-(Math.E**(3.6-0.0001*current_displayed_altitude)))/360, 60.0/100, (82/100)*(Math.E**(-0.5-0.0001*current_displayed_altitude))+0.025, 1.0)
            sky.gradient = skyObject;

            // set ground color
            var gndObject = Qt.createQmlObject('
                import QtQuick 2.0
                Gradient {
                    GradientStop { id: "gStop1"; position: 1.0; color: "brown" }
                    GradientStop { id: "gStop2"; position: 0.0; color: "brown" }
                }
                ',
               ground,
               "dynamicSnippet1");
            gndObject.stops[0].color = Qt.hsva((42.0/360), 50.0/100, (16/100)*(Math.E**(-0.5-0.0001*current_displayed_altitude))+0.025, 1.0)
            gndObject.stops[1].color = Qt.hsva((42.0/360), 50.0/100, (16/100)*(Math.E**(-0.1-0.0001*current_displayed_altitude))+0.15, 1.0)
            ground.gradient = gndObject;

            tick_repeater.contentY -= speed
        }
    }
}
