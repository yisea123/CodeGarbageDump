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

    property int xLimit: 6000
    property int yLimit: 110000
    property int zLimit: 700
    property int ySafeLimit: 5000
    property int motorLimit: 200000

    Settings {
        id: settings
    }

    GridLayout {
        width: 1056
        columns: 5
        columnSpacing: 8
        rowSpacing: 16

        // Boxes image
        Item {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 25
        }
        Custom.TextBody {
            text: qsTr("Box A")
        }
        Custom.TextBody {
            text: qsTr("Box B")
        }
        Custom.TextBody {
            text: qsTr("Box C")
        }
        Custom.TextBody {
            text: qsTr("Box D")
        }

        // Box Supply Location
        Custom.TextBody {
            Layout.columnSpan: 5
            text: qsTr("Box Supplier Location")
        }
        // Paremeter
        Column {
            Layout.leftMargin: 16
            spacing: 24
            Custom.TextBody {
                text: qsTr("X Location")
            }
            Custom.TextBody {
                text: qsTr("Y Location")
            }
            Custom.TextBody {
                text: qsTr("Z Location")
            }
        }
        Column {
            Custom.TextField {
                id: b1SupplyX
                placeholderText: qsTr("X Location")
                validator: IntValidator {bottom: 0; top: xLimit;}
            }
            Custom.TextField {
                id: b1SupplyY
                placeholderText: qsTr("Y Location")
                validator: IntValidator {bottom: 0; top: yLimit;}

            }
            Custom.TextField {
                id: b1SupplyZ
                placeholderText: qsTr("Z Location")
                validator: IntValidator {bottom: 0; top: zLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b2SupplyX
                placeholderText: qsTr("X Location")
                validator: IntValidator {bottom: 0; top: xLimit;}
            }
            Custom.TextField {
                id: b2SupplyY
                placeholderText: qsTr("Y Location")
                validator: IntValidator {bottom: 0; top: yLimit;}
            }
            Custom.TextField {
                id: b2SupplyZ
                placeholderText: qsTr("Z Location")
                validator: IntValidator {bottom: 0; top: zLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b3SupplyX
                placeholderText: qsTr("X Location")
                validator: IntValidator {bottom: 0; top: xLimit;}
            }
            Custom.TextField {
                id: b3SupplyY
                placeholderText: qsTr("Y Location")
                validator: IntValidator {bottom: 0; top: yLimit;}
            }
            Custom.TextField {
                id: b3SupplyZ
                placeholderText: qsTr("Z Location")
                validator: IntValidator {bottom: 0; top: zLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b4SupplyX
                placeholderText: qsTr("X Location")
                validator: IntValidator {bottom: 0; top: xLimit;}
            }
            Custom.TextField {
                id: b4SupplyY
                placeholderText: qsTr("Y Location")
                validator: IntValidator {bottom: 0; top: yLimit;}
            }
            Custom.TextField {
                id: b4SupplyZ
                placeholderText: qsTr("Z Location")
                validator: IntValidator {bottom: 0; top: zLimit;}
            }
        }

        // Grab box waiting location
        Custom.TextBody {
            Layout.columnSpan: 5
            text: qsTr("Waiting location")
        }
        // Paremeter
        Column {
            Layout.leftMargin: 16
            spacing: 24
            Custom.TextBody {
                text: qsTr("X Location")
            }
            Custom.TextBody {
                text: qsTr("Y Location")
            }
        }
        Column {
            Custom.TextField {
                id: b1WaitX
                placeholderText: qsTr("X Location")
                validator: IntValidator {bottom: 0; top: xLimit;}
            }
            Custom.TextField {
                id: b1WaitY
                placeholderText: qsTr("Y Location")
                validator: IntValidator {bottom: 0; top: ySafeLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b2WaitX
                placeholderText: qsTr("X Location")
                validator: IntValidator {bottom: 0; top: xLimit;}
            }
            Custom.TextField {
                id: b2WaitY
                placeholderText: qsTr("Y Location")
                validator: IntValidator {bottom: 0; top: ySafeLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b3WaitX
                placeholderText: qsTr("X Location")
                validator: IntValidator {bottom: 0; top: xLimit;}
            }
            Custom.TextField {
                id: b3WaitY
                placeholderText: qsTr("Y Location")
                validator: IntValidator {bottom: 0; top: ySafeLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b4WaitX
                placeholderText: qsTr("X Location")
                validator: IntValidator {bottom: 0; top: xLimit;}
            }
            Custom.TextField {
                id: b4WaitY
                placeholderText: qsTr("Y Location")
                validator: IntValidator {bottom: 0; top: ySafeLimit;}
            }
        }

        // Grab Boxes
        Custom.TextBody {
            Layout.columnSpan: 5
            text: qsTr("Grab Boxes Setting")
        }
        Column {
            Layout.leftMargin: 16
            spacing: 24
            Custom.TextBody {
                text: qsTr("Speed")
            }
        }

        Column {
            Custom.TextField {
                id: b1GrabSpeed
                placeholderText: qsTr("Speed")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b2GrabSpeed
                placeholderText: qsTr("Speed")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b3GrabSpeed
                placeholderText: qsTr("Speed")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b4GrabSpeed
                placeholderText: qsTr("Speed")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }

        // Embed Height
        Custom.TextBody {
            Layout.columnSpan: 5
            text: qsTr("Embed Movement")
        }
        Column {
            Layout.leftMargin: 16
            spacing: 24
            Custom.TextBody {
                text: qsTr("Settle Height")
            }
            Custom.TextBody {
                text: qsTr("Speed")
            }
        }
        Column {
            Custom.TextField {
                id: b1SettleHeight
                placeholderText: qsTr("Height")
                validator: IntValidator {bottom: 0; top: zLimit;}
            }
            Custom.TextField {
                id: b1EmbedSpeed
                placeholderText: qsTr("Speed")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b2SettleHeight
                placeholderText: qsTr("Height")
                validator: IntValidator {bottom: 0; top: zLimit;}
            }
            Custom.TextField {
                id: b2EmbedSpeed
                placeholderText: qsTr("Speed")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b3SettleHeight
                placeholderText: qsTr("Height")
                validator: IntValidator {bottom: 0; top: zLimit;}
            }
            Custom.TextField {
                id: b3EmbedSpeed
                placeholderText: qsTr("Speed")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: b4SettleHeight
                placeholderText: qsTr("Height")
                validator: IntValidator {bottom: 0; top: zLimit;}
            }
            Custom.TextField {
                id: b4EmbedSpeed
                placeholderText: qsTr("Speed")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }

        Custom.Button {
            text: qsTr("Apply")
            onClicked: {
                // Box Supplier Location
                settings.setValue("DMC/box1SupplyX", b1SupplyX.text);
                settings.setValue("DMC/box1SupplyY", b1SupplyY.text);
                settings.setValue("DMC/box1SupplyZ", b1SupplyZ.text);
                settings.setValue("DMC/box2SupplyX", b2SupplyX.text);
                settings.setValue("DMC/box2SupplyY", b2SupplyY.text);
                settings.setValue("DMC/box2SupplyZ", b2SupplyZ.text);
                settings.setValue("DMC/box3SupplyX", b3SupplyX.text);
                settings.setValue("DMC/box3SupplyY", b3SupplyY.text);
                settings.setValue("DMC/box3SupplyZ", b3SupplyZ.text);
                settings.setValue("DMC/box4SupplyX", b4SupplyX.text);
                settings.setValue("DMC/box4SupplyY", b4SupplyY.text);
                settings.setValue("DMC/box4SupplyZ", b4SupplyZ.text);
                // Waiting Location
                settings.setValue("DMC/box1WaitX", b1WaitX.text);
                settings.setValue("DMC/box1WaitY", b1WaitY.text);
                settings.setValue("DMC/box2WaitX", b2WaitX.text);
                settings.setValue("DMC/box2WaitY", b2WaitY.text);
                settings.setValue("DMC/box3WaitX", b3WaitX.text);
                settings.setValue("DMC/box3WaitY", b3WaitY.text);
                settings.setValue("DMC/box4WaitX", b4WaitX.text);
                settings.setValue("DMC/box4WaitY", b4WaitY.text);
                // Grab box
                settings.setValue("DMC/box1GrabSpeed", b1GrabSpeed.text);
                settings.setValue("DMC/box2GrabSpeed", b2GrabSpeed.text);
                settings.setValue("DMC/box3GrabSpeed", b3GrabSpeed.text);
                settings.setValue("DMC/box4GrabSpeed", b4GrabSpeed.text);
                // Embed
                settings.setValue("DMC/box1EmbedHeight", b1SettleHeight.text);
                settings.setValue("DMC/box1EmbedSpeed", b1EmbedSpeed.text);
                settings.setValue("DMC/box2EmbedHeight", b2SettleHeight.text);
                settings.setValue("DMC/box2EmbedSpeed", b2EmbedSpeed.text);
                settings.setValue("DMC/box3EmbedHeight", b3SettleHeight.text);
                settings.setValue("DMC/box3EmbedSpeed", b3EmbedSpeed.text);
                settings.setValue("DMC/box4EmbedHeight", b4SettleHeight.text);
                settings.setValue("DMC/box4EmbedSpeed", b4EmbedSpeed.text);

                system.updateSetting();
                savedText.text = qsTr("Saved");
                hideSavedText.start();
            }
        }

        Custom.Text {
            id: savedText
            text: ""
            color: "green"
        }

        Timer {
            id: hideSavedText
            interval: 5000;
            running: false;
            repeat: false
            onTriggered: savedText.text = ""
        }
    }

    Component.onCompleted: {
        var autoSetting = system.autoSetting;
        b1SupplyX.text = settings.value("DMC/box1SupplyX")
        b1SupplyY.text = settings.value("DMC/box1SupplyY")
        b1SupplyZ.text = settings.value("DMC/box1SupplyZ")
        b2SupplyX.text = settings.value("DMC/box2SupplyX")
        b2SupplyY.text = settings.value("DMC/box2SupplyY")
        b2SupplyZ.text = settings.value("DMC/box2SupplyZ")
        b3SupplyX.text = settings.value("DMC/box3SupplyX")
        b3SupplyY.text = settings.value("DMC/box3SupplyY")
        b3SupplyZ.text = settings.value("DMC/box3SupplyZ")
        b4SupplyX.text = settings.value("DMC/box4SupplyX")
        b4SupplyY.text = settings.value("DMC/box4SupplyY")
        b4SupplyZ.text = settings.value("DMC/box4SupplyZ")

        b1WaitX.text = settings.value("DMC/box1WaitX")
        b1WaitY.text = settings.value("DMC/box1WaitY")
        b2WaitX.text = settings.value("DMC/box2WaitX")
        b2WaitY.text = settings.value("DMC/box2WaitY")
        b3WaitX.text = settings.value("DMC/box3WaitX")
        b3WaitY.text = settings.value("DMC/box3WaitY")
        b4WaitX.text = settings.value("DMC/box4WaitX")
        b4WaitY.text = settings.value("DMC/box4WaitY")

        b1GrabSpeed.text = settings.value("DMC/box1GrabSpeed")
        b2GrabSpeed.text = settings.value("DMC/box2GrabSpeed")
        b3GrabSpeed.text = settings.value("DMC/box3GrabSpeed")
        b4GrabSpeed.text = settings.value("DMC/box4GrabSpeed")

        b1SettleHeight.text = settings.value("DMC/box1EmbedHeight")
        b1EmbedSpeed.text = settings.value("DMC/box1EmbedSpeed")
        b2SettleHeight.text = settings.value("DMC/box2EmbedHeight")
        b2EmbedSpeed.text = settings.value("DMC/box2EmbedSpeed")
        b3SettleHeight.text = settings.value("DMC/box3EmbedHeight")
        b3EmbedSpeed.text = settings.value("DMC/box3EmbedSpeed")
        b4SettleHeight.text = settings.value("DMC/box4EmbedHeight")
        b4EmbedSpeed.text = settings.value("DMC/box4EmbedSpeed")
    }
}
