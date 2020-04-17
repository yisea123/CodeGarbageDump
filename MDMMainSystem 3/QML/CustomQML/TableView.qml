import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import QtQuick 2.13  as Quick

Quick.TableView {
    //Layout.fillHeight: true
    //Layout.preferredWidth: parent.width
    //model: orderCurrentModel
    clip: true
    //property var columnWidths: [0.15, 0.15, 0.4, 0.15, 0.15]
    //columnWidthProvider: function (column) { return columnWidths[column] * parent.width }

    delegate: Quick.Rectangle {
        color: "transparent"
        implicitHeight: 50
        implicitWidth: 150
        TextBody {
            text: tabledata
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            font.bold: (heading == true) ? true : false
        }
        Quick.Rectangle {
            width: parent.width
            height: 1
            color: "#000000"
            opacity: 0.12
            anchors.bottom: parent.bottom
        }
    }
}
