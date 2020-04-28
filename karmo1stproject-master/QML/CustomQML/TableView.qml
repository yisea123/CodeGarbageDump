import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import QtQuick 2.13 as Quick

Quick.TableView {
    id: table
    clip: true
    columnWidthProvider: function (column) { return parent.width / table.model.columnCount() }

    delegate: Quick.Rectangle {
        color: "transparent"
        implicitHeight: 50
        implicitWidth: 150
        Quick.MouseArea {
            width: table.width
            height: parent.height
            enabled: false//index < table.model.rowCount() ? true : false // First column id = row id
            TextBody {
                text: tabledata
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                font.bold: (heading == true) ? true : false
            }
        }

        Quick.Rectangle {
            width: parent.width
            height: 1
            color: "#000000"
            opacity: 0.12
            anchors.bottom: parent.bottom
        }
    }

    Quick.Keys.onUpPressed: scrollBar.decrease()
    Quick.Keys.onDownPressed: scrollBar.increase()
    ScrollBar.vertical: ScrollBar {
        id: scrollBar
        //parent: parent.parent
        //anchors.top: parent.top
        //anchors.left: parent.right
        //anchors.bottom: parent.bottom
    }
}


