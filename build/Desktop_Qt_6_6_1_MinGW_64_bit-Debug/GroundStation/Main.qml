import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 1280
    height: 720
    visible: true
    title: qsTr("PSP GroundStation")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }
}
