import QtQuick

Item {
    id: gauge

    property string title: "CPU"
    property real value: 0
    property color accent: "#3b82f6"
    property color trackColor: "#2a2c37"

    property real animValue: value
    Behavior on animValue {
        NumberAnimation { duration: 800; easing.type: Easing.OutCubic }
    }
    onValueChanged: animValue = value

    Canvas {
        id: canvas
        anchors.fill: parent
        anchors.bottomMargin: 24

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)
            var cx = width / 2
            var cy = height / 2
            var lineW = Math.max(6, Math.min(width, height) * 0.13)
            var r = Math.min(width, height) / 2 - lineW / 2 - 3
            if (r <= 0)
                return
            var start = Math.PI * 0.75
            var end = Math.PI * 2.25

            ctx.lineWidth = lineW
            ctx.lineCap = "round"

            ctx.strokeStyle = gauge.trackColor
            ctx.beginPath()
            ctx.arc(cx, cy, r, start, end, false)
            ctx.stroke()

            var v = Math.max(0, Math.min(1, gauge.animValue / 100))
            ctx.strokeStyle = gauge.accent
            ctx.beginPath()
            ctx.arc(cx, cy, r, start, start + (end - start) * v, false)
            ctx.stroke()
        }
    }

    Connections {
        target: gauge
        function onAnimValueChanged() { canvas.requestPaint() }
    }

    Text {
        anchors.centerIn: parent
        text: Math.round(gauge.animValue) + "%"
        color: "#ffffff"
        font.pixelSize: 20
        font.bold: true
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        text: gauge.title
        color: "#8b90a0"
        font.pixelSize: 12
    }

    onWidthChanged: canvas.requestPaint()
    onHeightChanged: canvas.requestPaint()
    onAccentChanged: canvas.requestPaint()
    Component.onCompleted: canvas.requestPaint()
}
