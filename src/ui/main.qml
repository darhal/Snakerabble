import QtQuick
import Globals
import "Tools.js" as Tools

Window {
    id: mainWindow
    width: Globals.screen_width
    height: Globals.screen_height
    visible: true
    title: "Snakerabble"

    Arena {
        id: arena
        visible: false
    }

    SinglePlayerMenu {
        id: spMenu
        anchors.fill: parent
        visible: false
    }

    HostMenu {
        id: hostMenu
        anchors.fill: parent
        visible: false
    }

    JoinMenu {
        id: joinMenu
        anchors.fill: parent
        visible: false
    }

    MainMenu {
        id: mainMenu
        anchors.fill: parent
    }

    Component.onCompleted: {
        server.init(Globals.gridWidth, Globals.gridHeight)
    }
}


/*import QtQuick
import QtQuick.Window
import Globals

Window {
    width: Globals.screen_width
    height: Globals.screen_height
    visible: true
    title: qsTr("Snordle")

    Snake {
        //width: Math.min(parent.height, parent.width) - 2 * Globals.padding
        //height: Math.min(parent.height, parent.width)
        //anchors.top: parent.top
        //anchors.topMargin: Globals.padding
        //anchors.horizontalCenter: parent.horizontalCenter

        anchors.fill: parent
    }
}*/
