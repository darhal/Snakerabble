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
    property alias gameTimer: gameTimer

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

    Text {
        id: scoreText
        color: "white"
        text: "Score : "+client.snakeController.snakeData.score+"\n"+"Current word : "+client.snakeController.snakeData.word
        font.pixelSize: 14
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: Globals.padding/3
        anchors.topMargin: Globals.padding/3
    }

    PopupText {
        id: popupText
        anchors.centerIn: parent
    }

    PopupText {
        id: endScreenText
        anchors.centerIn: parent
        activate: false
    }

    Connections {
        target: client
        function onPopupScoreAnimation(word, combo) {
            popupText.textItem.text = word
            popupText.comboItem.text = "x"+combo;
        }
        function onResetAnimation(){
            popupText.textItem.text = "Reset"
            popupText.comboItem.text = "x0";
        }
        function onDeath()
        {
            gameTimer.stop();
            endScreenText.textItem.text = "You have lost!"
            endScreenText.comboItem.text = "Final Score : "+client.snakeController.snakeData.score
        }
    }

    Timer {
        id: gameTimer
        interval: Globals.refresh_rate
        running: false
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

    function launchSP(food)
    {
        server.startGame(food);
        arena.visible = true;
        arena.focus = true;
    }

    function hostGame()
    {
        server.hostGame();
        server.startGame(10, false);
        arena.visible = true;
        arena.focus = true;
        mainWindow.title = "Snordle : Server";
    }

    function joinGame(playername, ip)
    {
        client.joinGame(playername, ip);
        arena.visible = true;
        arena.focus = true;
    }
}
