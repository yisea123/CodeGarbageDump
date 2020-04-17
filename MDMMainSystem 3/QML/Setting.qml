import QtQuick 2.13
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import Qt.labs.settings 1.0
import QtQuick.Dialogs 1.1
import "CustomQML" as Custom

Custom.PageLower {
    anchors.fill: parent

    /*
     * Adjustable parameter
     */

    // Setting Menu
    ListModel {
        id: settingMenuItem
        ListElement { title: qsTr("Auto Mode Setting"); }
        ListElement { title: qsTr("Motor"); }
        ListElement { title: qsTr("Reset");}
    }
    property variant pagesFileNameList  : ["AutoSetting", "Motor", "Reset"]

    /*
     * Adjustable parameter end
     */

    // Parameter
    property int currentPageIndex: 0
    property string currentPageName: pagesFileNameList[0]

    Custom.Pane {
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16
        width: parent.width - 32
        height: parent.height - 32

        background: Rectangle {
            radius: 10
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "#fafafa" }
                GradientStop { position: 0.16; color: "#fafafa" }
                GradientStop { position: 0.1601; color: "#eeeeee" }
                GradientStop { position: 1.0; color: "#eeeeee" }
            }
        }

        // Menu
        ListView {
            id: settingMenu

            focus: true
            currentIndex: currentPageIndex
            width: parent.width * 0.15
            height: parent.height
            anchors.left: parent.left
            clip: true

            delegate: ItemDelegate {
                width: parent.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    currentPageIndex = index;
                    currentPageName = pagesFileNameList[index];
                }
            }

            model: settingMenuItem
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        // Setting Area
        Custom.TextHeading {
            id: settingAreaHeading
            anchors.top: parent.top
            anchors.topMargin: 16
            anchors.left: settingMenu.right
            anchors.leftMargin: 32
            text: settingMenuItem.get(currentPageIndex).title
        }

        Item {
            id: settingArea
            anchors.top: settingAreaHeading.bottom
            anchors.topMargin: 16
            anchors.left: settingMenu.right
            anchors.leftMargin: 32
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Repeater {
                model: pagesFileNameList;
                delegate: Loader {
                    id: settingPages
                    active: false;
                    asynchronous: true;
                    anchors.fill: parent;
                    visible: (currentPageName === pagesFileNameList[index])
                    source: "Setting/%1.qml".arg(modelData)
                    onVisibleChanged:      { loadIfNotLoaded(); }
                    Component.onCompleted: { loadIfNotLoaded(); }

                    function loadIfNotLoaded () {
                        // to load the file at first show
                        if (visible && !active) {
                            active = true;
                        }
                    }
                }
            }
        }
    }
}
