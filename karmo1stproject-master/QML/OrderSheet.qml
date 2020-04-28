import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Styles 1.2
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "CustomQML" as Custom
import Karmo.Max.OrderModel 0.1

Custom.PageLower {
    anchors.fill: parent

    OrderModel {id: orderModel}

    Pane {
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: parent.width - 32
        height: parent.height - 32

        ColumnLayout {
            anchors.fill: parent

            /*Custom.Button {
                text: "Test Log"
                onClicked: {system.newLog(0);}
            }*/

            //https://doc.qt.io/qt-5.12/qml-qtquick-tableview.html

            Custom.TableView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                property var columnWidths: [0.175, 0.075, 0.1, 0.1, 0.2, 0.35]
                columnWidthProvider: function (column) { return columnWidths[column] * parent.width }

                clip: true
                model: orderModel

            }
        }
    }
}

