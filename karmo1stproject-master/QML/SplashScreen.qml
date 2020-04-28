import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import QtQuick.Window 2.13

Window {
    modality: Qt.ApplicationModal
    flags: Qt.SplashScreen | Qt.WindowStaysOnTopHint
    width: 500
    height: 300
    color: "transparent"
    Material.accent: "#ffffff"

    Rectangle {
        id: background
        anchors.fill: parent
        radius: 10
        color: "#000051"

        // Company logo
        Image {
            id: companyLogo
            x: 386
            y: 8
            height: 30
            fillMode: Image.PreserveAspectFit
            source: "Images/CompanyLogo.png"
            opacity: 0

            ParallelAnimation {
                id: companyLogoAnimation
                OpacityAnimator {
                    target: companyLogo;
                    from: 0;
                    to: 1;
                    duration: 300
                }

                XAnimator {
                    target: companyLogo;
                    from: 386;
                    to: 436;
                    duration: 300
                    easing.type: Easing.OutQuad
                }
            }
        }

        // App name
        Text {
            id: appName
            x: -6
            y: 110
            color: "#ffffff"
            text: qsTr("Control Unit System")
            font.pixelSize: 50
            opacity: 0

            ParallelAnimation {
                id: appNameAnimation
                OpacityAnimator {
                    target: appName;
                    from: 0;
                    to: 1;
                    duration: 300
                }

                XAnimator {
                    target: appName;
                    from: -6;
                    to: 44;
                    duration: 300
                    easing.type: Easing.OutQuad
                }
            }
        }

        // App version
        Text {
            id: appVersion
            x: 181
            y: 176
            color: "#ffffff"
            text: qsTr("V1.0")
            font.pixelSize: 20
            opacity: 0

            ParallelAnimation {
                id: appVersionAnimation
                OpacityAnimator {
                    target: appVersion;
                    from: 0;
                    to: 1;
                    duration: 300
                }

                XAnimator {
                    target: appVersion;
                    from: 181;
                    to: 231;
                    duration: 300
                    easing.type: Easing.OutQuad
                }
            }
        }

        BusyIndicator {
            id: busyIndicator
            x: 8
            y: 252
            width: 40
            height: 40
        }

        Text {
            x: 54
            y: 260
            color: "#ffffff"
            text: qsTr("The System is starting...")
            font.pixelSize: 20
        }
    }

    Component.onCompleted: {
        visible = true;
        companyLogoAnimation.start();
        appNameAnimation.start();
        appVersionAnimation.start();
    }
}
