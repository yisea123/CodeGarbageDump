import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import QtQuick.Layouts 1.12
//import QtQuick.VirtualKeyboard 2.4
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.13


import "CustomQML" as Custom


ApplicationWindow {
    id: window
    visible: true
    //width: 960
    width: 1920
    //height: 720
    height: 1080
    title: qsTr("Control Unit System")

    //


    // Adjustable parameters
    property variant pagesList : [qsTr("Control Unit Main"), qsTr("Order List"), qsTr("LogSheet")]
    property variant pagesFileNameList  : ["MainPage","OrderSheet","LogSheet"]
    property string  currentPage : pagesList[0]

    Material.theme: Material.Light
    Material.primary: "#00004c"
    Material.accent: "#00FF00"
    //Material.background: "#E1E2E1"
    color: "#E1E2E1"

     //Background

    Rectangle {
        anchors.fill: parent
        color: Material.primary
    }

    Rectangle {
        id: mainPane
        // Contains header and content, to display a radius bottom
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: footer.top
        radius: 10

        // Hide the corner radius
        Rectangle{
            anchors.top: parent.top
            height: parent.radius
            width: parent.width
        }

        // Content
        Item {
            id: appContent
            width: parent.width
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Repeater {
                model: pagesFileNameList;
                delegate: Loader {
                    active: false;
                    asynchronous: true;
                    anchors.fill: parent;
                    visible: (currentPage === pagesList[index]);
                    source: "%1.qml".arg(modelData)
                    onVisibleChanged:      { loadIfNotLoaded(); }
                    Component.onCompleted: { loadIfNotLoaded(); }

                    function loadIfNotLoaded () {
                        // to load the file at first show
                        if (visible && !active) {
                            active = true;
                        }
                    }
                }
            }
        }

        // Header
        Item {
            id: header
            width: parent.width
            height: 50
            anchors.top: parent.top
            anchors.topMargin: 8

            RowLayout {
                width: parent.width - 48
                anchors.horizontalCenter: parent.horizontalCenter

                // Menu
                TabBar {
                    font.capitalization: Font.MixedCase // Upper case by default
                    background: Rectangle {
                        color: "transparent"
                    }
                    Repeater {
                        model: pagesList;
                        delegate: TabButton {
                            height: parent.height
                            width: implicitWidth
                            text: modelData
                            contentItem: Text {
                                text: modelData
                                font: parent.font
                                color: "black"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                            // Change current page
                            onClicked: {currentPage = modelData}
                            checked: index === 0 ? true : false
                        }
                    }
                }

                // Clock
                Text {
                    id: clock
                    font.pixelSize: 15
                    horizontalAlignment: Text.AlignRight
                    Layout.alignment: Qt.AlignRight
                }
                Timer {
                    running: true
                    repeat: true
                    interval: 1000
                    onTriggered: {
                        var date = new Date();
                        clock.text = date.toTimeString() + "\n" + date.toLocaleString(Qt.locale(), "yyyy/MMMM/dd (dddd)");
                    }
                }
            }
        }
    }

    DropShadow {
        anchors.fill: mainPane
        horizontalOffset: 0
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#aa000000"
        source: mainPane
    }
    // Footer
    RowLayout {
        id: footer
        width: parent.width - 48
        height: 48
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        // Application name
        Custom.Text {
            text: qsTr("Control Unit System")
            color: "white"
        }

        // Company Logo
        Row {
            spacing: 8
            Layout.alignment: Qt.AlignRight
            Custom.Text {
                text: qsTr("Powered by")
                color: "white"

            }
            Image {
                height: 30
                fillMode: Image.PreserveAspectFit
                source: "Images/CompanyLogo.png"
            }
        }
    }
}

