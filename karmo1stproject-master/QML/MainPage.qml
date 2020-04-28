import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "CustomQML" as Custom

import Karmo.Max.ControlUnit 1.0
//import Karmo.Max.OrderItem 1.0

Custom.PageLower {
    anchors.fill: parent
    Controlunit{id: controlunit}

    /*
    Custom.HeaderText {
        Custom.ButtonToggle {
            id: qmlToggleButton
            height: 26
            width: window.width
            leftString: qsTr("User Mode")
            rightString: qsTr("Developer Mode")
            onToggleLeft: label.text = qmlToggleButton.leftString
            onToggleRight: label.text = qmlToggleButton.rightString;
        }
    }
    */
    Timer {
        interval: 5000; running: true; repeat: true
        onTriggered: notice.text = qsTr(" ")
    }

    Custom.Text {
        id: notice
        text: qsTr("")
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }


    Column {
        id: row
        anchors.centerIn: parent
        spacing: 8

        TextField {
            id: qrcode
            width: 900
            activeFocusOnPress: true
            placeholderTextColor: "#470A0A"
            placeholderText: qsTr("Waiting For QRCode...")
            Material.background: "#e5e5ff"
            //onTextChanged:
            //validator: RegExpValidator { regExp: /&/ }
            /*
            Text{
                    id: qrcodedescription
                    y: -(parent.height)
                    x: parent.x
                    text: qsTr("QR Code")
                    font.pixelSize: 16
                    }
                    */
        }

        ComboBox {
            id: priority
            width: 300
            displayText: qsTr("Priority: VIP Lv") + currentText
            model: [ "1", "2", "3" ]
            Material.background: "#e5e5ff"
        }


        Custom.Button {
            id: btninsert
            text: "Insert"
            width: 300
            highlighted: true
            //palette { button: "black"; buttonText: "white"}
            onClicked:
            {
                if(controlunit.btnInsert(priority.currentIndex+1,qrcode.text))
                {
                    notice.text = qsTr("Successfully Inserted Order with " + priority.displayText)
                }
                else
                {
                    notice.text = qsTr("Invalid QR Code!!!")
                }

            }
            Material.background: "#e5e5ff"
        }

        Row {
            id: dubugmode
            spacing: 20
            visible: true

            Custom.Button {
                id: btndummies
                text: "Generate Dummies"
                highlighted: true
                Material.background: "#ffcccc"
                onClicked: {
                    if(controlunit.debugAddDummies())
                        notice.text = qsTr("Dummy Successfully Added")

                }
            }

            Custom.Button {
                id: btnextradummies
                text: "Generate Extra Dummies"
                highlighted: true
                Material.background: "#ffcccc"
                onClicked: {
                    if(controlunit.debugAddExtraDummies())
                        notice.text = qsTr("ExtraDummy Successfully Added")
                }
            }

            Custom.Button {
                id: btnlog
                text: "Debug Log"
                highlighted: true
                Material.background: "#ffcccc"
                onClicked: {
                    if(controlunit.debugShowOrderList())
                        notice.text = qsTr("List shown in debug log")
                    else
                        notice.text = qsTr("Nothing in the debug log yet... Add Some Dummies and Try Again!")
                }
            }

            Custom.Button {
                id: btnDB
                text: "Connect Database"
                highlighted: true
                Material.background: "#ffcccc"
                onClicked: {
                    if(controlunit.connectDatabase())
                        notice.text = qsTr("Database Connected Successfully")
                    else
                        notice.text = qsTr("Failed to connect Database")
                }
            }

            Custom.Button {
                id: btnTCP
                text: "Send TCP msg"
                highlighted: true
                Material.background: "#ffcccc"
                onClicked: {
                    if(controlunit.sendTCPsignal())
                        notice.text = qsTr("TCP Signal sent")
                    else
                        notice.text = qsTr("Failed to send TCP Signal")
                }
            }

            Custom.Button {
                id: btnFinishall
                text: "Finish All"
                highlighted: true
                Material.background: "#ffcccc"
                onClicked: {
                    if(controlunit.debugFinishAll())
                        notice.text = qsTr("All items finished.")
                    else
                        notice.text = qsTr("No Order yet!!")
                }
            }
        }

    }

}




