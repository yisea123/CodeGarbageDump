import QtQuick 2.13

Item {
    Loader {
        id: mainWindowLoader
        active: false
        source: "MainWindow.qml"
        asynchronous: true
        onLoaded: {
            item.visible = true;
            splashScreenLoader.item.visible = false;
            splashScreenLoader.source = "";
        }
    }

    Loader {
        id: splashScreenLoader
        source: "SplashScreen.qml"
        onLoaded: {
            //system.start();
            mainWindowLoader.active = true;
        }
    }
}
