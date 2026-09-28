import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#1b1c24"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 16

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 150
            spacing: 16

            ServerStatusCard { Layout.fillWidth: true }
            DeviceTile { Layout.preferredWidth: 250 }
            AlarmCard { Layout.preferredWidth: 340 }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 220
            spacing: 16

            ResourcesCard { Layout.preferredWidth: 440 }
            TrafficCard { Layout.fillWidth: true }
        }

        RealTimeCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 180
        }
    }

    // ================= 基础组件 =================
    component GlassCard: Rectangle {
        property color cardColor: "#20212b"
        property color cardBorder: "#2a2c37"
        radius: 10
        color: cardColor
        border.width: 1
        border.color: cardBorder
    }

    component PulseDot: Item {
        property color dotColor: "#22c55e"
        Rectangle {
            anchors.centerIn: parent
            width: 12
            height: 12
            radius: 6
            color: dotColor
        }
        Rectangle {
            anchors.centerIn: parent
            width: 12
            height: 12
            radius: 6
            color: "transparent"
            border.width: 2
            border.color: dotColor
            NumberAnimation on scale {
                from: 1; to: 3; duration: 1500
                loops: Animation.Infinite; running: true
            }
            NumberAnimation on opacity {
                from: 0.6; to: 0; duration: 1500
                loops: Animation.Infinite; running: true
            }
        }
    }

    // ================= 服务器状态卡片 =================
    component ServerStatusCard: GlassCard {
        RowLayout {
            anchors.fill: parent
            anchors.margins: 18
            spacing: 16

            Item {
                Layout.preferredWidth: 66
                Layout.preferredHeight: 66
                Rectangle {
                    anchors.centerIn: parent
                    width: 48
                    height: 48
                    radius: 24
                    color: dashboard.serverOnline ? "#14351f" : "#3a1c1c"
                }
                PulseDot {
                    anchors.centerIn: parent
                    dotColor: dashboard.serverOnline ? "#22c55e" : "#ef4444"
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                RowLayout {
                    spacing: 8
                    Text {
                        text: "服务器状态"
                        color: "#f1f2f6"
                        font.pixelSize: 15
                        font.bold: true
                    }
                    Rectangle {
                        radius: 4
                        color: dashboard.serverOnline ? "#14351f" : "#3a1c1c"
                        implicitWidth: chip.implicitWidth + 16
                        implicitHeight: 20
                        Text {
                            id: chip
                            anchors.centerIn: parent
                            text: dashboard.serverOnline ? "运行中" : "已停止"
                            color: dashboard.serverOnline ? "#22c55e" : "#ef4444"
                            font.pixelSize: 11
                        }
                    }
                }

                Text {
                    text: dashboard.serverName + " · " + dashboard.version
                    color: "#8b90a0"
                    font.pixelSize: 12
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    spacing: 32
                    ColumnLayout {
                        spacing: 1
                        Text { text: "运行时长"; color: "#8b90a0"; font.pixelSize: 11 }
                        Text {
                            text: dashboard.uptime
                            color: "#e6e8ee"; font.pixelSize: 16; font.bold: true
                            font.family: "Consolas"
                        }
                    }
                    ColumnLayout {
                        spacing: 1
                        Text { text: "在线设备"; color: "#8b90a0"; font.pixelSize: 11 }
                        Text {
                            text: dashboard.onlineDevices + " / " + dashboard.totalDevices
                            color: "#e6e8ee"; font.pixelSize: 16; font.bold: true
                        }
                    }
                }
            }
        }
    }

    // ================= 在线设备数量 =================
    component DeviceTile: GlassCard {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 18
            spacing: 8

            Text { text: "在线设备数量"; color: "#8b90a0"; font.pixelSize: 12 }

            RowLayout {
                spacing: 6
                Text {
                    text: dashboard.onlineDevices
                    color: "#ffffff"
                    font.pixelSize: 36
                    font.bold: true
                }
                Text {
                    text: "/ " + dashboard.totalDevices
                    color: "#6b7280"
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignBottom
                    Layout.bottomMargin: 8
                }
                Item { Layout.fillWidth: true }
            }

            Item { Layout.fillHeight: true }

            Flow {
                Layout.fillWidth: true
                spacing: 6
                Repeater {
                    model: dashboard.deviceStates
                    Rectangle {
                        width: 14
                        height: 14
                        radius: 3
                        color: modelData ? "#22c55e" : "#3a3d49"
                        Behavior on color { ColorAnimation { duration: 300 } }
                    }
                }
            }
        }
    }

    // ================= 报警状态 =================
    component AlarmCard: GlassCard {
        id: alarmCard
        property color levelColor: dashboard.alarmLevel === 2 ? "#ef4444"
                                 : dashboard.alarmLevel === 1 ? "#f59e0b" : "#22c55e"

        border.color: dashboard.alarmLevel > 0 ? alarmCard.levelColor : "#2a2c37"
        Behavior on border.color { ColorAnimation { duration: 400 } }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 18
            spacing: 8

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: "报警状态"
                    color: "#f1f2f6"
                    font.pixelSize: 15
                    font.bold: true
                }
                Item { Layout.fillWidth: true }
                Rectangle {
                    visible: dashboard.alarmCount > 0
                    radius: 9
                    color: "#3a1c1c"
                    implicitWidth: 28
                    implicitHeight: 18
                    Text {
                        anchors.centerIn: parent
                        text: dashboard.alarmCount
                        color: "#ef4444"
                        font.pixelSize: 11
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Rectangle {
                    Layout.preferredWidth: 42
                    Layout.preferredHeight: 42
                    radius: 10
                    color: Qt.rgba(alarmCard.levelColor.r, alarmCard.levelColor.g,
                                   alarmCard.levelColor.b, 0.15)
                    Text {
                        anchors.centerIn: parent
                        text: dashboard.alarmLevel === 0 ? "✓" : "!"
                        color: alarmCard.levelColor
                        font.pixelSize: 22
                        font.bold: true
                    }
                    // 严重报警时闪烁
                    SequentialAnimation on opacity {
                        running: dashboard.alarmLevel === 2
                        loops: Animation.Infinite
                        NumberAnimation { to: 0.35; duration: 550 }
                        NumberAnimation { to: 1.0; duration: 550 }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    Text {
                        text: dashboard.alarmLevel === 0 ? "系统正常"
                            : dashboard.alarmLevel === 1 ? "警告" : "严重报警"
                        color: alarmCard.levelColor
                        font.pixelSize: 14
                        font.bold: true
                    }
                    Text {
                        Layout.fillWidth: true
                        text: dashboard.alarmText
                        color: "#8b90a0"
                        font.pixelSize: 11
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        elide: Text.ElideRight
                    }
                }
            }

            Item { Layout.fillHeight: true }

            Rectangle {
                visible: dashboard.alarmLevel > 0
                Layout.fillWidth: true
                implicitHeight: 30
                radius: 6
                color: ackArea.containsMouse
                       ? Qt.rgba(alarmCard.levelColor.r, alarmCard.levelColor.g,
                                 alarmCard.levelColor.b, 0.15)
                       : "transparent"
                border.width: 1
                border.color: alarmCard.levelColor
                Text {
                    anchors.centerIn: parent
                    text: "确认报警"
                    color: alarmCard.levelColor
                    font.pixelSize: 12
                }
                MouseArea {
                    id: ackArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: dashboard.acknowledgeAlarm()
                }
            }
        }
    }

    // ================= CPU / 内存 / 磁盘 =================
    component ResourcesCard: GlassCard {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 8

            Text {
                text: "资源占用"
                color: "#f1f2f6"
                font.pixelSize: 14
                font.bold: true
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 8

                RingGauge {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: "CPU"
                    value: dashboard.cpu
                    accent: "#3b82f6"
                }
                RingGauge {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: "内存"
                    value: dashboard.memory
                    accent: "#a855f7"
                }
                RingGauge {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    title: "磁盘"
                    value: dashboard.disk
                    accent: "#06b6d4"
                }
            }
        }
    }

    // ================= 网络流量 =================
    component TrafficCard: GlassCard {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 10

            RowLayout {
                Layout.fillWidth: true
                Text {
                    text: "网络流量"
                    color: "#f1f2f6"
                    font.pixelSize: 14
                    font.bold: true
                }
                Item { Layout.fillWidth: true }
                Text {
                    text: "实时"
                    color: "#22c55e"
                    font.pixelSize: 11
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: -4
                        radius: 4
                        color: "#14351f"
                        z: -1
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                RowLayout {
                    Layout.fillWidth: true
                    Text { text: "↑ 上行"; color: "#8b90a0"; font.pixelSize: 12 }
                    Item { Layout.fillWidth: true }
                    Text {
                        text: dashboard.netUp.toFixed(1) + " KB/s"
                        color: "#22c55e"
                        font.pixelSize: 13
                        font.bold: true
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 8
                    radius: 4
                    color: "#2a2c37"
                    Rectangle {
                        height: parent.height
                        radius: 4
                        color: "#22c55e"
                        width: parent.width * Math.min(1, dashboard.netUp / 1000)
                        Behavior on width { NumberAnimation { duration: 600; easing.type: Easing.OutCubic } }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                RowLayout {
                    Layout.fillWidth: true
                    Text { text: "↓ 下行"; color: "#8b90a0"; font.pixelSize: 12 }
                    Item { Layout.fillWidth: true }
                    Text {
                        text: dashboard.netDown.toFixed(1) + " KB/s"
                        color: "#3b82f6"
                        font.pixelSize: 13
                        font.bold: true
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 8
                    radius: 4
                    color: "#2a2c37"
                    Rectangle {
                        height: parent.height
                        radius: 4
                        color: "#3b82f6"
                        width: parent.width * Math.min(1, dashboard.netDown / 2000)
                        Behavior on width { NumberAnimation { duration: 600; easing.type: Easing.OutCubic } }
                    }
                }
            }

            Item { Layout.fillHeight: true }

            Row {
                Layout.fillWidth: true
                Layout.preferredHeight: 34
                spacing: 3
                Repeater {
                    model: 24
                    Rectangle {
                        width: (parent.width - 23 * 3) / 24
                        anchors.bottom: parent.bottom
                        radius: 1
                        color: "#2f6fe0"
                        height: {
                            var s = dashboard.series
                            var idx = s.length - 24 + index
                            var v = (idx >= 0 && idx < s.length) ? s[idx] : 0
                            return parent.height * Math.max(0.12, v / 100)
                        }
                        Behavior on height { NumberAnimation { duration: 500 } }
                    }
                }
            }
        }
    }

    // ================= 实时曲线 =================
    component RealTimeCard: GlassCard {
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 6

            RowLayout {
                Layout.fillWidth: true
                spacing: 8
                Text {
                    text: "实时曲线"
                    color: "#f1f2f6"
                    font.pixelSize: 14
                    font.bold: true
                }
                Item { Layout.fillWidth: true }
                Rectangle { width: 10; height: 10; radius: 5; color: "#3b82f6" }
                Text { text: "CPU 使用率 (%)"; color: "#8b90a0"; font.pixelSize: 11 }
            }

            RealTimeChart {
                Layout.fillWidth: true
                Layout.fillHeight: true
                points: dashboard.series
                accent: "#3b82f6"
            }
        }
    }
}
