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

    property int motorLimit: 200000

    Settings {
        id: settings
    }
    GridLayout {
        width: parent.width
        columns: 4
        columnSpacing: 8
        rowSpacing: 16

        // Axis
        Item {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 25
        }
        Custom.TextBody {
            Layout.preferredWidth: 200
            text: qsTr("X-Axis")
        }
        Custom.TextBody {
            Layout.preferredWidth: 200
            text: qsTr("Y-Axis")
        }
        Custom.TextBody {
            Layout.preferredWidth: 200
            text: qsTr("Z-Axis")
        }

        // Motor Velocity
        Custom.TextBody {
            Layout.columnSpan: 4
            text: qsTr("Motor velocity")
        }

        // Paremeter
        Column {
            Layout.leftMargin: 16
            spacing: 24
            Custom.TextBody {
                text: qsTr("Maximum velocity")
            }
            Custom.TextBody {
                text: qsTr("Minimum velocity")
            }
        }
        Column {
            Custom.TextField {
                id: xMaxVelocity
                placeholderText: qsTr("X maximum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
            Custom.TextField {
                id: xMinVelocity
                placeholderText: qsTr("X minimum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}

            }
        }
        Column {
            Custom.TextField {
                id: yMaxVelocity
                placeholderText: qsTr("Y maximum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
            Custom.TextField {
                id: yMinVelocity
                placeholderText: qsTr("Y minimum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: zMaxVelocity
                placeholderText: qsTr("Z maximum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
            Custom.TextField {
                id: zMinVelocity
                placeholderText: qsTr("Y minimum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }

        // Motor Resetting Velocity
        Custom.TextBody {
            Layout.columnSpan: 4
            text: qsTr("Motor resetting velocity")
        }

        // Paremeter
        Column {
            Layout.leftMargin: 16
            spacing: 24
            Custom.TextBody {
                text: qsTr("Maximum velocity")
            }
            Custom.TextBody {
                text: qsTr("Minimum velocity")
            }
        }
        Column {
            Custom.TextField {
                id: xResetMaxVelocity
                placeholderText: qsTr("X maximum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
            Custom.TextField {
                id: xResetMinVelocity
                placeholderText: qsTr("X minimum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}

            }
        }
        Column {
            Custom.TextField {
                id: yResetMaxVelocity
                placeholderText: qsTr("Y maximum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
            Custom.TextField {
                id: yResetMinVelocity
                placeholderText: qsTr("Y minimum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }
        Column {
            Custom.TextField {
                id: zResetMaxVelocity
                placeholderText: qsTr("Z maximum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
            Custom.TextField {
                id: zResetMinVelocity
                placeholderText: qsTr("Y minimum velocity")
                validator: IntValidator {bottom: 0; top: motorLimit;}
            }
        }


        Custom.Button {
            text: qsTr("Apply")
            onClicked: {
                settings.setValue("DMC/motorXMinVelcoty", xMaxVelocity.text);
                settings.setValue("DMC/motorXMaxVelcoty", xMinVelocity.text);
                settings.setValue("DMC/motorYMinVelcoty", yMaxVelocity.text);
                settings.setValue("DMC/motorYMaxVelcoty", yMinVelocity.text);
                settings.setValue("DMC/motorZMinVelcoty", zMaxVelocity.text);
                settings.setValue("DMC/motorZMaxVelcoty", zMinVelocity.text);
                settings.setValue("DMC/motorResetXMinVelcoty", xResetMaxVelocity.text);
                settings.setValue("DMC/motorResetXMaxVelcoty", xResetMinVelocity.text);
                settings.setValue("DMC/motorResetYMinVelcoty", yResetMaxVelocity.text);
                settings.setValue("DMC/motorResetYMaxVelcoty", yResetMinVelocity.text);
                settings.setValue("DMC/motorResetZMinVelcoty", zResetMaxVelocity.text);
                settings.setValue("DMC/motorResetZMaxVelcoty", zResetMinVelocity.text);
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
        xMaxVelocity.text = settings.value("DMC/motorXMinVelcoty")
        xMinVelocity.text = settings.value("DMC/motorXMaxVelcoty")
        yMaxVelocity.text = settings.value("DMC/motorYMinVelcoty")
        yMinVelocity.text = settings.value("DMC/motorYMaxVelcoty")
        zMaxVelocity.text = settings.value("DMC/motorZMinVelcoty")
        zMinVelocity.text = settings.value("DMC/motorZMaxVelcoty")
        xResetMaxVelocity.text = settings.value("DMC/motorResetXMinVelcoty")
        xResetMinVelocity.text = settings.value("DMC/motorResetXMaxVelcoty")
        yResetMaxVelocity.text = settings.value("DMC/motorResetYMinVelcoty")
        yResetMinVelocity.text = settings.value("DMC/motorResetYMaxVelcoty")
        zResetMaxVelocity.text = settings.value("DMC/motorResetZMinVelcoty")
        zResetMinVelocity.text = settings.value("DMC/motorResetZMaxVelcoty")
    }
}

