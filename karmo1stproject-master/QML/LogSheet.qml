import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "CustomQML" as Custom
import Karmo.Max.LogModel 0.1

Custom.PageLower {
    anchors.fill: parent

    LogModel {id: logModel}

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

            Custom.TableView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                property var columnWidths: [0.2, 0.1, 0.1, 0.2, 0.4]
                columnWidthProvider: function (column) { return columnWidths[column] * parent.width }

                clip: true
                model: logModel
            }
        }
    }
}
