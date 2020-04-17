import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import QtCharts 2.13
import "CustomQML" as Custom

Custom.PageLower {
    anchors.fill: parent

    property var laserVoltageTextRef: []
    property var laserDistanceTextRef: []
    property var seriesColor: ["red", "green", "blue"]
    property var seriesCount: 0

    Connections {
        target: system
        onRegularJobFinished: {
            // Update the value
            var laserFinder = system.laserFinder;
            for(var i = 0; i < 3; i++)
            {
                laserVoltageTextRef[i].text = qsTr("Voltage: %1 V").arg(laserFinder[i * 2]);
                laserDistanceTextRef[i].text = qsTr("Distance: %1 m").arg(laserFinder[i * 2 + 1]);
            }
            // Update chart
            laser1.append(seriesCount, laserFinder[0 * 2 + 1]);
            laser2.append(seriesCount, laserFinder[1 * 2 + 1]);
            laser3.append(seriesCount, laserFinder[2 * 2 + 1]);
            seriesCount += 1;

            // Remove history
            if(laser1.count >= 200)
            {
                laser1.remove(0);
                laser2.remove(0);
                laser3.remove(0);
                chartView.axisX().min++;
                chartView.axisX().max++;
            }
        }
    }

    Custom.Pane {
        id: chart
        width: parent.width * 0.8 - 24
        height: parent.height - 32
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16

        ChartView {
            id: chartView
            //title: "Laser Finder Value"
            anchors.fill: parent
            antialiasing: true
            legend.visible: false

            LineSeries {
                id: laser1
                name: qsTr("Laser 1")
                color: seriesColor[0]
            }
            LineSeries {
                id: laser2
                name: qsTr("Laser 2")
                color: seriesColor[1]
            }
            LineSeries {
                id: laser3
                name: qsTr("Laser 3")
                color: seriesColor[2]
            }

            Component.onCompleted: {
                chartView.axisY().min = 0;
                chartView.axisY().max = 12;
                chartView.axisX().max = 200;
            }
        }
    }

    Custom.Pane {
        width: parent.width * 0.2 - 24
        height: parent.height - 32
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: chart.right
        anchors.leftMargin: 16

        ColumnLayout {
            anchors.fill: parent

            Repeater {
                model: 3
                delegate: Column {
                    spacing: 8
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Custom.TextHeading {
                        text: qsTr("Laser Finder %1".arg(index + 1))
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Rectangle {
                        width: 180
                        height: 180
                        color: seriesColor[index];
                        anchors.horizontalCenter: parent.horizontalCenter

                        Image {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.fill: parent
                            source: "Images/laser.png"
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                    Custom.TextBody {
                        text: qsTr("Voltage: 1V")
                        anchors.horizontalCenter: parent.horizontalCenter
                        Component.onCompleted: {
                            laserVoltageTextRef.push(this);
                        }
                    }
                    Custom.TextBody {
                        text: qsTr("Distance: 1m")
                        anchors.horizontalCenter: parent.horizontalCenter
                        Component.onCompleted: {
                            laserDistanceTextRef.push(this);
                        }
                    }
                    Item {
                        // Expand the height
                        width: parent.width
                        height: 24
                        visible: index != 2 ? true : false

                        Custom.HorizontalRule {
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }
            }
        }
    }
}
