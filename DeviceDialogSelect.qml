import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "AppStyle"

Rectangle {
    property string dev_name: ""
    color: AppStyle.light
    height: 40

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        Text {
            id: device_name
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: dev_name
            font.pointSize: 16
            verticalAlignment: Text.AlignVCenter
            color: "white"
        }

        CustomSwitch {
            Layout.alignment: Qt.AlignRight
            implicitHeight: 30
            id: device_on
        }
    }
}
