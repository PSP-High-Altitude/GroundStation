import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    ComboBox {
        id: dropdown
        font.pointSize: 20

        model: ListModel {
            ListElement { text: "No Device Connected" }
        }

        contentItem: Text {
            leftPadding: 4
            rightPadding: dropdown.indicator.width + dropdown.spacing
            text: dropdown.displayText
            font: dropdown.font
            color: "white"
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        TextMetrics {
            id: display_text_metrics
            font: dropdown.font
            text: dropdown.displayText
        }

        background: Rectangle {
            implicitWidth: display_text_metrics.width + 20
            implicitHeight: display_text_metrics.height + 10
            color: dropdown.pressed ? "#2c2c2c" : "#3c3c3c"
            border.color: "#4c4c4c"
            radius: 2
        }

        popup: Popup {
            y: dropdown.height - 1
            width: dropdown.width
            implicitHeight: contentItem.implicitHeight
            padding: 1

            contentItem: ListView {
                clip: true
                implicitHeight: contentHeight
                model: dropdown.popup.visible ? dropdown.delegateModel : null
                currentIndex: dropdown.highlightedIndex

                ScrollIndicator.vertical: ScrollIndicator { }
            }

            background: Rectangle {
                color: "#3c3c3c"
                border.color: "#4c4c4c"
                radius: 2
            }
        }
    }
}
