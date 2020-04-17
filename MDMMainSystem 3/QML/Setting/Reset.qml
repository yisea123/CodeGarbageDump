import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "../CustomQML" as Custom

ScrollView {
    anchors.fill: parent
    clip: true

    Custom.Button{
        text: qsTr("Reset and quit appliacation")
        onClicked: {
            resetDialog.open();
        }
    }

    Dialog {
        id: resetDialog
        visible: false
        title: qsTr("Application reset")
        x: (window.width - width) / 2
        y: (window.height - height) / 2
        parent: ApplicationWindow.overlay
        width: parent.width * 0.5
        height: parent.height * 0.2
        focus: true
        modal: true
        closePolicy: Popup.NoAutoClose

        Custom.Text {
            text: qsTr("You are about to erase the data which is impossible to recover. Are you sure?");
        }

        onAccepted: {
            system.resetApplication();
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
    }

}
