import QtQuick
import Globals
import "Tools.js" as Tools

Item {
    id: game
    width: Globals.screen_width
    height: Globals.screen_height
    property int boxWidth: width / Globals.gridWidth
    property int boxHeight: height / Globals.gridHeight
    property alias snake: snake
    property alias foodSpawner: foodSpawner

    Rectangle {
        id: background
        anchors.fill: parent
        color: 'black'
    }

    Snake {
        id: snake
        visible: true
        size: [boxWidth, boxHeight]
    }

    Repeater {
        id: otherPlayersProp
        model: client.otherPlayers.length

        SnakeProp {
            clientId: index
            size: [boxWidth, boxHeight]
        }
    }

    FoodSpawner {
        id: foodSpawner
        size: [boxWidth, boxHeight]
    }

    Timer {
        id: gameTimer
        interval: 200
        running: true
        repeat: true
        onTriggered: {
            client.sendGameData()
            if (server.isRunning())
                server.broadcastGameData()
            snake.move()
        }
    }

    // Keyboard inputs
    focus: true
    property var keyMap: new Array

    Keys.onPressed: (event) => {
        var func = keyMap[event.key];
        if (func)
            func()
    }

    Component.onCompleted: {
        keyMap[Qt.Key_Up]    = snake.changeDir.bind(undefined, Tools.Direction.UP);
        keyMap[Qt.Key_Down]  = snake.changeDir.bind(undefined, Tools.Direction.DOWN);
        keyMap[Qt.Key_Left]  = snake.changeDir.bind(undefined, Tools.Direction.LEFT);
        keyMap[Qt.Key_Right] = snake.changeDir.bind(undefined, Tools.Direction.RIGHT);
    }
}
