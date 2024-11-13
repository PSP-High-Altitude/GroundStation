import QtQuick

Item {
    Text {
        id: clock_text
        text: "00:00:00 AM"
        font.pointSize: 20
        color: "white"
    }

    Timer {
        interval: 100; running: true; repeat: true;
        onTriggered: updateTime()
    }

    function updateTime() {
        var date = new Date;
        var hours = date.getHours() % 12
        var ampm = date.getHours() / 12
        var minutes = date.getMinutes()
        var seconds = date.getSeconds();

        const zeroPad = (num, places) => String(num).padStart(places, '0')
        if(hours == 0) hours = 12;

        clock_text.text = zeroPad(hours, 2) + ":" + zeroPad(minutes, 2) + ":" + zeroPad(seconds, 2) + " " + (ampm ? "PM" : "AM")
    }
}
