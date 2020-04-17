import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "CustomQML" as Custom
import DxfImportModel 0.1
import CurrentJobModel 0.1

Custom.PageLower {
    anchors.fill: parent

    DxfImportModel{id: dxfImportModel}
    CurrentJobModel{id: currentJobModel}

    Connections {
        target: system
        onAutoModeChanged: {
            // Auto mode stopped, disable manual mode
            if(!system.autoMode && !system.emergencyMode)
            {
                startAutoButton.enabled = true;
                pasueAutoButton.enabled = false;
                resetMotor.enabled = true;
            }
        }
        onDmcMotorResetFinished: {
            resetMotor.enabled = true;
            startAutoButton.enabled = true;
            debugStart.enabled = true;
        }
        onEmergencyModeChanged: {
            if(system.emergencyMode)
            {
                emergencyStop.Material.primary = "red";
                startAutoButton.enabled = false;
                pasueAutoButton.enabled = false;
                resetMotor.enabled = false;
                debugStart.enabled = false;
                debugNext.enabled = false;
            }
            else
            {
                emergencyStop.Material.primary = Material.Pink;
                startAutoButton.enabled = true;
                pasueAutoButton.enabled = false;
                resetMotor.enabled = true;
                debugStart.enabled = true;
                debugNext.enabled = false;
            }
        }
    }

    /*
     * Current Job Detail
     */
    Custom.Pane {
        id: currentJobDetail
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: (parent.width / 2) - 24
        height: (parent.height / 2) - 24
        Column {
            Row{
                spacing: 8
                Custom.Button {
                    id: startAutoButton
                    text: qsTr("Start Auto Mode")
                    onClicked: {
                        system.runAuto();
                        pasueAutoButton.enabled = true;
                        resetMotor.enabled = false;
                        startAutoButton.enabled = false;
                    }
                }
                Custom.Button {
                    id: pasueAutoButton
                    text: qsTr("Pause Auto Mode")
                    enabled: false
                    onClicked: {
                        system.pauseAuto();
                        pasueAutoButton.enabled = false;
                        resetMotor.enabled = true;
                        startAutoButton.enabled = true;
                    }
                }
                Custom.Button {
                    id: resetMotor
                    text: qsTr("Reset Motor")
                    onClicked: {
                        resetMotor.enabled = false;
                        startAutoButton.enabled = false;
                        debugStart.enabled = false;
                        system.resetMotor();
                    }
                }
                Custom.Button {
                    visible: false
                    id: debugStart
                    text: qsTr("Debug Start")
                    onClicked: {
                        system.debugStart();
                        startAutoButton.enabled = false;
                        debugStart.enabled = false;
                        resetMotor.enabled = false;
                        debugNext.enabled = true;
                    }
                }
                Custom.Button {
                    visible: false
                    id: debugNext
                    text: qsTr("Debug Next")
                    enabled: false
                    onClicked: {
                        system.debugNextOperation();
                    }
                }
                Custom.Button {
                    id: emergencyStop
                    text: qsTr("Emergency Stop")
                    Material.primary: Material.Pink
                    onClicked: {
                        if(system.emergencyMode)
                        {
                            // Yes to no
                            system.emergencyStopOff();
                        }
                        else
                        {
                            // no to yes
                            system.emergencyStop();
                        }
                    }
                }
            }

            Custom.TextHeading {
                text: qsTr("Current Job Detail")
            }
            Custom.TextBody {
                text: qsTr("ID: %1").arg(system.currentJobID)
            }
            Custom.TextBody {
                text: qsTr("File: %1").arg(system.currentJobFilePath)
            }
            Custom.TextBody {
                text: system.currentJobProcedureSize > 0 ? qsTr("Procedure Size: %1").arg(system.currentJobProcedureSize) : qsTr("Procedure Size: N/A")
            }
            Custom.TextBody {
                text: qsTr("Box Type: %1").arg(system.CurrentBoxType)
            }
            Custom.TextBody {
                text: qsTr("Panel height: %1").arg(system.CurrentPanelHeight)
            }
            /*Custom.Text {
                id: createTime
                Component.onCompleted: {
                    createTime.text = qsTr("Create Time: %1").arg(model.modelData.createTime.toLocaleString(Qt.locale(), "yyyy年MM月dd日 (dddd) HH:mm:ss"))
                }
            }*/
            Custom.TextBody {
                text: system.currentJobProcedureSize > 0 ? qsTr("Progress: %1 out of %2").arg(system.currentJobProcedure + 1).arg(system.currentJobProcedureSize) : qsTr("Progress: N/A")
            }
            Custom.TextBody {
                text: qsTr("Current operation: %1").arg(system.systemStatus)
            }
        }
    }

    /*
     * Current Job Procedure
     */
    Custom.Pane {
        id: currentJobProcedure
        anchors.top: currentJobDetail.bottom
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: (parent.width / 2) - 24
        height: (parent.height / 2) - 24
        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            Custom.TextHeading {
                text: qsTr("Current Job Procedure")
            }
            Custom.TableView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                columnWidthProvider: function (column) {
                    if (column === 1)
                        return parent.width * 0.2
                    else
                        return parent.width * 0.05
                }
                clip: true
                model: currentJobModel
            }
        }
    }

    /*
     * Job List
     */
    Custom.Pane {
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: currentJobDetail.right
        anchors.leftMargin: 16
        width: (parent.width / 2) - 24
        height: parent.height - 32

        ColumnLayout {
            anchors.fill: parent

            Custom.TextHeading {
                text: qsTr("Job Queue")
            }

            Custom.Button {
                id: newJobButton
                text: qsTr("New Job")
                onClicked: {
                    newJobDialog.open();
                }
            }

            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: system.jobList
                clip: true
                delegate: Item {
                    id: jobPane
                    width: parent.width
                    height: index > 0 ? 156 : 140

                    Column {
                        width: parent.width
                        height: parent.height

                        Item {
                            // Expand the height
                            width: parent.width
                            height: 16
                            visible: index > 0 ? true : false

                            Custom.HorizontalRule {
                                width: parent.width
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        Row {
                            width: parent.width
                            spacing: 16
                            Rectangle {
                                width: 140 * 1.5
                                height: 140
                                color: "#000000"

                                Image {
                                    width: parent.width
                                    height: parent.height
                                    fillMode: Image.PreserveAspectFit
                                    source: "file:///" + system.currentPath + "\\" + model.modelData.id + ".png"
                                }
                            }

                            Column {
                                Custom.HeaderText {
                                    text: qsTr("Job %1").arg(index + 1)
                                }
                                Custom.Text {
                                    text: qsTr("ID %1").arg(model.modelData.id)
                                }
                                Custom.Text {
                                    text: qsTr("File: %1").arg(model.modelData.file)
                                }
                                Custom.Text {
                                    text: qsTr("Step: %1").arg(model.modelData.step)
                                }
                                Custom.Text {
                                    id: createTimeList
                                    Component.onCompleted: {
                                        createTimeList.text = qsTr("Create Time: %1").arg(model.modelData.createTime.toLocaleString(Qt.locale(), "yyyy年MM月dd日 (dddd) HH:mm:ss"))
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /*
     * Job Dialog
     */
    Dialog {
        id: newJobDialog
        visible: false
        title: qsTr("New Job")
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        width: parent.width * 0.6
        height: parent.height * 0.9
        focus: true
        modal: true
        closePolicy: Popup.NoAutoClose

        ColumnLayout {
            width: parent.width
            height: parent.height
            spacing: 8
            RowLayout {
                Layout.preferredWidth: parent.width
                Custom.Button {
                    text: qsTr("Browse...")
                    onClicked: dxfImportFileDialog.open()
                    FileDialog {
                        id: dxfImportFileDialog
                        title:  qsTr("Please choose a file to import")
                        folder: shortcuts.home
                        nameFilters: [ "AutoCAD DXF (*.dxf)" ]
                        onAccepted: {
                            // Read file
                            var result = dxfImport.nameCheck(dxfImportFileDialog.fileUrl);
                            switch(result)
                            {
                                case 1:
                                    errorText.text = qsTr("Error: Incorrect name format");
                                    return;
                                case 2:
                                    errorText.text =  qsTr("Incorrect box type");
                                    return;
                                case 3:
                                    errorText.text = qsTr("Incorrect height");
                                    return;
                                default:
                                    errorText.text = qsTr("");
                                    break;
                            }

                            dxfImport.import(dxfImportFileDialog.fileUrl);
                            var ctx = dxfCanvas.getContext("2d");
                            var box = dxfImport.dxfBox;
                            ctx.clearRect(0, 0, dxfCanvas.width, dxfCanvas.height);
                            ctx.fillStyle = Qt.rgba(0, 0, 0, 1.0);
                            ctx.fillRect(0, 0, dxfCanvas.width, dxfCanvas.height);
                            ctx.fillStyle = Qt.rgba(1, 1, 1, 1.0);

                            var dxfDivideFactor = 10;
                            var boxSize = 20;
                            // Find highest y
                            var highestY = 0;
                            for (var i=0; i<box.length; i++){
                                if((box[i].y)/dxfDivideFactor - boxSize > highestY)
                                    highestY = (box[i].y)/dxfDivideFactor - boxSize;
                            }

                            // Draw the dxf
                            for (i=0; i<box.length; i++){
                                //console.log("QML: Box loc: ", box[i]);
                                //console.log("QML: Box locx: ", (box[i].x)/dxfDivideFactor - boxSize, "Box locy: ", (box[i].y)/dxfDivideFactor - boxSize);
                                // Reflect the y axis
                                ctx.fillRect((box[i].x)/dxfDivideFactor - boxSize/2, (highestY + boxSize * 2) - ((box[i].y)/dxfDivideFactor - boxSize/2), boxSize, boxSize);
                            }
                            dxfCanvas.requestPaint();
                            dxfCanvas.save("temp.png");

                            // File name
                            dxfImportPath.text = dxfImportFileDialog.fileUrl;
                        }
                    }

                }
                Custom.Text {
                    id: dxfImportPath
                    Layout.fillWidth: true
                    clip: true
                }
            }
            /*
            Custom.Text {
                id: boxTypeText
                Layout.fillWidth: true
                clip: true
                text: qsTr("Box type: ")
            }
            Custom.Text {
                id: panelHeightText
                Layout.fillWidth: true
                clip: true
                text: qsTr("Concrete panel height: ")
            }
            */
            Custom.Text {
                id: errorText
                Layout.fillWidth: true
                clip: true
                color: "red"
                //text: qsTr("Error:")
            }
            Canvas {
                id: dxfCanvas
                Layout.fillWidth: true
                Layout.fillHeight: true

                property bool firstRun: true
                onPaint: {
                    if(dxfCanvas.firstRun)
                    {
                        // Ask user to import a dxf file
                        dxfCanvas.firstRun = false;
                        var ctx = dxfCanvas.getContext("2d");
                        ctx.fillStyle = Qt.rgba(0, 0, 0, 1.0);
                        ctx.fillRect(0, 0, dxfCanvas.width, dxfCanvas.height);
                        ctx.fillStyle = Qt.rgba(1, 1, 1, 1.0);
                        ctx.textAlign = "center";
                        ctx.font = "50px Arial";
                        ctx.fillText(qsTr("Import a .dxf file"), dxfCanvas.width/2, dxfCanvas.height/2);
                    }
                }
            }

            Custom.TableView {
                //id: dxfCanvas
                Layout.fillWidth: true
                Layout.fillHeight: true
                columnWidthProvider: function (column) {
                    if (column === 1)
                        return parent.width * 0.2
                    else
                        return parent.width * 0.05
                }
                clip: true
                model: dxfImportModel
            }
        }

        onAccepted: {
            if(dxfImportPath.text.length === 0)
                return;
            else
            {
                // Save dxf to image
                dxfCanvas.save((system.jobCount + 1) + ".png");
                // Create Job
                dxfImport.newJob();
            }
        }

        onClosed: {
            // Reset the dialog
            dxfCanvas.firstRun = true;
            dxfImportPath.text = "";
            dxfImport.clear();
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
    }
}
