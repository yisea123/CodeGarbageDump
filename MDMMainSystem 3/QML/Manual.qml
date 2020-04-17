import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "CustomQML" as Custom

Item {
    property var gripperSwitchRef: []
    property var vibrationSwitchRef: []

    property int xLimit: 6000
    property int yLimit: 110000
    property int zLimit: 700

    Connections {
        target: system
        onAutoModeChanged: {
            // Auto mode started, disable manual mode
            if(system.autoMode)
            {
                for(var i = 0; i < 15; i++)
                {
                    gripperSwitchRef[i].enabled = false;
                    vibrationSwitchRef[i].enabled = false;
                }
                xAxisButton.enabled = false;
                yAxisButton.enabled = false;
                zAxisButton.enabled = false;
                openAllGripper.enabled = false;
                closeAllGripper.enabled = false;
                vibrationOn.enabled = false;
                vibrationOff.enabled = false;

                startManualMode.enabled = true;
            }
        }
    }

    Rectangle {
        id: manualImage
        anchors.top: parent.top
        width: parent.width
        height: parent.height * 0.55
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#fafafa" }
            GradientStop { position: 0.6; color: "#fafafa" }
            GradientStop { position: 1.0; color: "#dedede" }
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            source: "Images/Manual.png"
            fillMode: Image.PreserveAspectFit
        }
    }

    Item {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: manualImage.bottom
        anchors.topMargin: 8
        Material.elevation: 2
        width: 950
        height: 400

        GridLayout {
            width: parent.width
            columns: 2
            columnSpacing: 16

            Custom.Button {
                id: startManualMode
                text: qsTr("Start manual mode")
                Layout.columnSpan: 2
                onClicked: {
                    system.pauseAuto();
                    var gripperStatus = system.gripperStatus;
                    var vibrationStatus = system.vibrationStatus;
                    for(var i = 0; i < 15; i++)
                    {
                        gripperSwitchRef[i].enabled = true;
                        if(gripperStatus[i])
                            gripperSwitchRef[i].checked = true;
                        else
                            gripperSwitchRef[i].checked = false;

                        vibrationSwitchRef[i].enabled = true;
                        if(vibrationStatus[i])
                            vibrationSwitchRef[i].checked = true;
                        else
                            vibrationSwitchRef[i].checked = false;
                    }
                    xAxisButton.enabled = true;
                    yAxisButton.enabled = true;
                    zAxisButton.enabled = true;
                    openAllGripper.enabled = true;
                    closeAllGripper.enabled = true;
                    vibrationOn.enabled = true;
                    vibrationOff.enabled = true;
                    startManualMode.enabled = false;
                }
            }

            /*
             * First Row
             */
            Custom.TextHeading {
               text: qsTr("Gripper")
            }
            Custom.TextHeading {
               text: qsTr("Motor")
            }

            /*
             * Second Row
             */
            /*
             *  Gripper, 3x5
             */
            Row {
                spacing: 16
                Repeater {
                    // Colunm
                    model: 3
                    Column {
                        // Contains 5 row
                        id: gripperRow
                        property int colunmCount: index
                        Repeater {
                            // Row
                            model: 6
                            RowLayout {
                                width: 200
                                // First row
                                Custom.TextBody {
                                    Layout.preferredWidth: 50
                                    text: ""
                                    visible: index === 0 ? true : false
                                }
                                Custom.TextBody {
                                    text: qsTr("Gripper")
                                    visible: index === 0 ? true : false
                                }
                                Custom.TextBody {
                                    text: qsTr("Vibration")
                                    visible: index === 0 ? true : false
                                }

                                // Other row
                                Custom.TextBody {
                                    Layout.fillWidth: true
                                    text: (gripperRow.colunmCount * 5 + index)
                                    visible: index === 0 ? false : true
                                    horizontalAlignment: Text.AlignRight
                                }
                                Switch {
                                    //text: qsTr("Gripper")
                                    id: gripperSwitch
                                    visible: index === 0 ? false : true
                                    enabled: false
                                    Component.onCompleted: {
                                        if(index > 0)
                                            // Prevent nonvisible switch
                                            gripperSwitchRef.push(this);
                                    }
                                    onToggled: {
                                        // Toggled by user
                                        if(gripperSwitch.checked)
                                        {
                                            system.manualGripper(gripperRow.colunmCount * 5 + index, true);
                                        }
                                        else
                                        {
                                            system.manualGripper(gripperRow.colunmCount * 5 + index, false);
                                        }
                                    }
                                }
                                Switch {
                                    //text: qsTr("Vibration")
                                    id: vibrationSwitch
                                    visible: index === 0 ? false : true
                                    enabled: false
                                    anchors.horizontalCenter: parent.Center
                                    Component.onCompleted: {
                                        if(index > 0)
                                            vibrationSwitchRef.push(this);
                                    }
                                    onToggled: {
                                        // Toggled by user
                                        if(vibrationSwitch.checked)
                                            system.manualVibration(gripperRow.colunmCount * 5 + index, true);
                                        else
                                            system.manualVibration(gripperRow.colunmCount * 5 + index, false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            /*
             *  Motor
             */
            Grid {
                width: 250
                columns: 2
                columnSpacing: 8
                TextField {
                    id: xAxisTextField
                    placeholderText: qsTr("X Location")
                    validator: IntValidator {bottom: 0; top: xLimit;}
                }
                Custom.Button {
                    id: xAxisButton
                    enabled: false
                    text: qsTr("Set X")
                    onClicked: {
                        system.manualMove(0, xAxisTextField.text);
                    }
                }
                TextField {
                    id: yAxisTextField
                    placeholderText: qsTr("Y Location")
                    validator: IntValidator {bottom: 0; top: yLimit;}
                }
                Custom.Button {
                    id: yAxisButton
                    enabled: false
                    text: qsTr("Set Y")
                    onClicked: {
                        system.manualMove(1, yAxisTextField.text);
                    }
                }
                TextField {
                    id: zAxisTextField
                    placeholderText: qsTr("Z Location")
                    validator: IntValidator {bottom: 0; top: zLimit;}
                }
                Custom.Button {
                    id: zAxisButton
                    enabled: false
                    text: qsTr("Set Z")
                    onClicked: {
                        system.manualMove(2, zAxisTextField.text);
                    }
                }
                /*
                Custom.Button {
                    text: qsTr("Set All")
                    onClicked: {
                        system.manualMove3Axis(xAxisTextField.text, yAxisTextField.text, zAxisTextField.text);
                    }
                }*/
            }
            /*
             * Switch all
             */
            Row {
                spacing: 8
                Custom.Button {
                    id: openAllGripper
                    enabled: false
                    text: qsTr("Open all gripper")
                    onClicked: {
                        for(var i = 0; i < 15; i++)
                        {
                            var skip = true;
                            if(i === 14)
                                skip = false
                            system.manualGripper(i + 1, true, skip);
                            gripperSwitchRef[i].checked = true;
                        }
                    }
                }
                Custom.Button {
                    id: closeAllGripper
                    enabled: false
                    text: qsTr("Close all gripper")
                    onClicked: {
                        for(var i = 0; i < 15; i++)
                        {
                            var skip = true;
                            if(i === 14)
                                skip = false
                            system.manualGripper(i + 1, false, skip);
                            gripperSwitchRef[i].checked = false;
                        }
                    }
                }
                Custom.Button {
                    id: vibrationOn
                    enabled: false
                    text: qsTr("Vibration on")
                    onClicked: {
                        for(var i = 0; i < 15; i++)
                        {
                            system.manualVibration(i + 1, true);
                            vibrationSwitchRef[i].checked = true;
                        }
                    }
                }
                Custom.Button {
                    id: vibrationOff
                    enabled: false
                    text: qsTr("Vibration off")
                    onClicked: {
                        for(var i = 0; i < 15; i++)
                        {
                            system.manualVibration(i + 1, false);
                            vibrationSwitchRef[i].checked = false;
                        }
                    }
                }
            }
        }
    }
}
