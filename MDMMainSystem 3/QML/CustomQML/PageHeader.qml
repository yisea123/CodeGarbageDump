import QtQuick 2.13
import QtQuick.Layouts 1.12

ColumnLayout {
    property alias pageTitle: myHeader.text
    spacing: 16

    HeaderText {
        id: myHeader
        Layout.topMargin: 16
        Layout.leftMargin: 16
    }

    HorizontalRule {
        width: parent.width
    }
}
