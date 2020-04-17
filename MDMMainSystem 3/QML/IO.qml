import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "CustomQML" as Custom
import ArmIOModel 0.1
import DMCIOModel 0.1

Item {
    anchors.fill: parent

    ArmIOModel {id: armIOModel}
    DMCIOModel {id: dmcIOModel}

    Rectangle {
        id: images
        anchors.top: parent.top
        width: parent.width
        height: parent.height * 0.6
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#fafafa" }
            GradientStop { position: 0.6; color: "#fafafa" }
            GradientStop { position: 1.0; color: "#dedede" }
        }

        Image {
            id: structureImage
            width: parent.width/2
            height: parent.height
            anchors.left: parent.left
            anchors.top: parent.top
            source: "Images/Strcture.png"
            fillMode: Image.PreserveAspectFit
        }

        Image {
            width: parent.width/2
            height: parent.height
            anchors.left: structureImage.right
            anchors.top: parent.top
            source: "Images/Manual.png"
            fillMode: Image.PreserveAspectFit
        }

    }

    // Structure I/O
    Item {
        id: structureIO
        width: parent.width/2
        height: parent.height * 0.4
        anchors.left: parent.left
        anchors.top: images.bottom

        Custom.TableView {
            width: parent.width - 16
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            columnWidthProvider: function (column) { return parent.width / dmcIOModel.columnCount() }
            interactive: false
            clip: true
            model: dmcIOModel

            delegate: Rectangle {
                color: "transparent"
                implicitHeight: 50
                implicitWidth: 150
                Custom.Text {
                    text: tabledata
                    anchors.centerIn: parent
                    font.bold: (heading == true) ? true : false
                    visible: (heading == true) ? true : false
                }
                Rectangle {
                    anchors.centerIn: parent
                    width: 20
                    height: 20
                    color: "green"
                    visible: (narole == false && heading == false && truerole == true) ? true : false
                    radius: width*0.5
                }
                Rectangle {
                    anchors.centerIn: parent
                    width: 20
                    height: 20
                    color: "red"
                    visible: (narole == false && heading == false && truerole == false) ? true : false
                    radius: width*0.5
                }
                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#000000"
                    opacity: 0.12
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    // ARM I/O
    Item {
        width: parent.width/2
        height: parent.height * 0.4
        anchors.left: structureIO.right
        anchors.top: images.bottom

        Custom.TableView {
            width: parent.width - 16
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            interactive: false
            columnWidthProvider: function (column) {
                if (column === 0)
                    return parent.width * 0.1
                else
                    return parent.width * 0.06
            }
            clip: true
            model: armIOModel
            delegate: Rectangle {
                color: "transparent"
                implicitHeight: 50
                implicitWidth: 150
                Custom.Text {
                    text: tabledata
                    anchors.centerIn: parent

                    font.bold: (heading == true) ? true : false
                    visible: (heading == true) ? true : false
                }
                Rectangle {
                    anchors.centerIn: parent
                    width: 20
                    height: 20
                    color: "green"
                    visible: (heading == false && truerole == true) ? true : false
                    radius: width*0.5
                }
                Rectangle {
                    anchors.centerIn: parent
                    width: 20
                    height: 20
                    color: "red"
                    visible: (heading == false && truerole == false) ? true : false
                    radius: width*0.5
                }
                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#000000"
                    opacity: 0.12
                    anchors.bottom: parent.bottom
                }
            }
        }
    }
}
