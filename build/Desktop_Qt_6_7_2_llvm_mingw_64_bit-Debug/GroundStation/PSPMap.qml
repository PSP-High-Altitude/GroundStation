import QtQuick
import QtLocation 6.5
import QtPositioning 6.5
import QtQuick.Controls

Item {
    Plugin {
        id: mapPlugin
        name: "osm"

        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "http://tiles.griffinrosseng.com/osm_repository"
        }
        PluginParameter {
            name: "osm.mapping.highdpi_tiles"
            value: true
        }
    }

    function addMarker(latitude, longitude)
    {
        var Component = Qt.createComponent("resources/marker.qml")
        var item = Component.createObject(window, {
            id: marker,
            coordinate: QtPositioning.coordinate(latitude, longitude)
        })
        map.addMapItem(item)
    }

    Map {
        id: map
        objectName: "map"
        anchors.fill: parent
        plugin: mapPlugin
        activeMapType: supportedMapTypes[5]
        center {
            latitude: 40.42371
            longitude: -86.921194
        }
        zoomLevel: 14

        MapQuickItem {
            id: marker
            zoomLevel: map.zoomLevel

            sourceItem: Image {
                id: marker_img
                source: "resources/marker.png"
                sourceSize.width: 32
                sourceSize.height: 32
            }

            coordinate: QtPositioning.coordinate(40.42371, -86.921194)
            anchorPoint.x: marker_img.width / 2
            anchorPoint.y: marker_img.height
        }

        function setMarker(lat, lon) {
            marker.coordinate.latitude = lat
            marker.coordinate.longitude = lon
        }

        function recenter() {
            center.latitude = marker.coordinate.latitude
            center.longitude = marker.coordinate.longitude
        }

        WheelHandler {
                    id: wheel
                    rotationScale: 1/120
                    property: "zoomLevel"
        }
        DragHandler {
                    id: drag
                    target: null
                    onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
        }
    }

    Button {
        flat: true
        width: 40
        height: 40
        x: map.width - width - 15
        y: 15
        text: ""
        onClicked: map.recenter()
        icon {
            source: "resources/map_pointer.png"
            color: "transparent"
            width: 25
            height: 25
        }
    }
}
