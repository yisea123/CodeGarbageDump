import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "CustomQML" as Custom

Custom.PageLower {
    anchors.fill: parent
    Column {
        width: 500
        anchors.horizontalCenter: parent.horizontalCenter
        Custom.Text {
            text: "For debugging purpose"
        }
        TextField {
            id: ipAddress
            placeholderText: qsTr("IP Address")
        }
        TextField {
            id: port
            placeholderText: qsTr("Port")
        }
        TextField {
            id: boxRequest
            placeholderText: qsTr("Box Request 1 for yes")
        }
        Custom.Button {
            text: "Send Request"
            onClicked: {
                if(boxRequest.length != 15)
                    result.text = "Incorrect length for request"
                else
                {
                    result.text = "Sent"
                    system.boxesSupplierRequest(ipAddress.text, port.text, boxRequest.text);
                }
            }
        }
        Custom.Text {
            id: result
        }
        Custom.Button {
            text: "Analog"
            onClicked: {
                 system.analog();
            }
        }
    }


}
