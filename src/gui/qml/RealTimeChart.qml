import QtQuick

Item {
    id: chart

    property var points: []
    property color accent: "#3b82f6"
    property color gridColor: "#2e303c"

    onPointsChanged: canvas.requestPaint()

    Canvas {
        id: canvas
        anchors.fill: parent
        renderStrategy: Canvas.Cooperative

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            var padL = 40, padR = 14, padT = 12, padB = 22
            var plotW = width - padL - padR
            var plotH = height - padT - padB
            if (plotW <= 0 || plotH <= 0)
                return

            var minV = 0, maxV = 100

            ctx.strokeStyle = chart.gridColor
            ctx.lineWidth = 1
            ctx.font = "10px sans-serif"
            ctx.fillStyle = "#6b7280"
            ctx.textBaseline = "middle"
            for (var i = 0; i <= 4; ++i) {
                var y = padT + plotH * i / 4
                ctx.beginPath()
                ctx.moveTo(padL, y)
                ctx.lineTo(padL + plotW, y)
                ctx.stroke()
                var val = Math.round(maxV - (maxV - minV) * i / 4)
                ctx.fillText(String(val), 10, y)
            }

            var n = chart.points.length
            if (n < 2)
                return

            var stepX = plotW / (n - 1)
            function px(idx) { return padL + idx * stepX }
            function py(v) { return padT + plotH * (1 - (v - minV) / (maxV - minV)) }

            var grad = ctx.createLinearGradient(0, padT, 0, padT + plotH)
            grad.addColorStop(0, Qt.rgba(chart.accent.r, chart.accent.g, chart.accent.b, 0.35))
            grad.addColorStop(1, Qt.rgba(chart.accent.r, chart.accent.g, chart.accent.b, 0.0))

            ctx.beginPath()
            ctx.moveTo(px(0), py(chart.points[0]))
            for (var j = 1; j < n; ++j)
                ctx.lineTo(px(j), py(chart.points[j]))
            ctx.lineTo(px(n - 1), padT + plotH)
            ctx.lineTo(px(0), padT + plotH)
            ctx.closePath()
            ctx.fillStyle = grad
            ctx.fill()

            ctx.beginPath()
            ctx.moveTo(px(0), py(chart.points[0]))
            for (var k = 1; k < n; ++k)
                ctx.lineTo(px(k), py(chart.points[k]))
            ctx.strokeStyle = chart.accent
            ctx.lineWidth = 2
            ctx.lineJoin = "round"
            ctx.stroke()

            var lastX = px(n - 1)
            var lastY = py(chart.points[n - 1])
            ctx.beginPath()
            ctx.arc(lastX, lastY, 8, 0, Math.PI * 2)
            ctx.fillStyle = Qt.rgba(chart.accent.r, chart.accent.g, chart.accent.b, 0.25)
            ctx.fill()
            ctx.beginPath()
            ctx.arc(lastX, lastY, 4, 0, Math.PI * 2)
            ctx.fillStyle = chart.accent
            ctx.fill()
        }
    }

    onWidthChanged: canvas.requestPaint()
    onHeightChanged: canvas.requestPaint()
    onAccentChanged: canvas.requestPaint()
    Component.onCompleted: canvas.requestPaint()
}
