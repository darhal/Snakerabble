import QtQuick 2.3
import "Tools.js" as Tools

Item {
    id: game

    width: 1920
    height: 1080
    property int gridWidth: 80
    property int gridHeight: 45
    property int boxWidth: width / gridWidth
    property int boxHeight: height / gridHeight

    Rectangle {
        id: background

        anchors.fill: parent
        color: 'blue'
    }

    Item {
        id: snake

        property var parts: [[20, 20], [20, 21], [20, 22], [21, 22], [22, 22]]
        property int dir: Tools.Direction.UP
        property bool canChangeDir: true

        function move() {
            var head = parts[0].slice(0);
            var body = parts;
            body.pop(); // Move body
            Tools.move(head, dir, gridWidth, gridHeight); // Move head
            // Bit itself ?
            if (Tools.posInArray(head, body))
                return die();
            // Bit the food ?
            if (Tools.checkPos(head, food.pos))
                eat(body);
            parts = [head].concat(body);
            canChangeDir = true;
        }

        function eat(pos) {
            pos.unshift(food.pos.slice(0));
            food.place();
            gameTimer.interval -= (5 * gameTimer.interval) / 100;
        }

        function die() {
            print('game over');
            gameTimer.running = false;
        }

        function changeDir(newDir) {
            if (canChangeDir) {
                if (Tools.OPPOSITES[newDir] != dir) {
                    dir = newDir;
                    canChangeDir = false;
                }
            }
        }
    }

    Repeater {
        model: snake.parts

        Rectangle {
            width: boxWidth
            height: boxHeight
            radius: 10
            color: 'red'

            Component.onCompleted: {
                x = Qt.binding(function() { return snake.parts[index][0] * boxWidth; });
                y = Qt.binding(function() { return snake.parts[index][1] * boxHeight; });
            }
        }
    }

    Rectangle {
        id: food

        color: 'yellow'
        property var pos
        width: boxWidth
        height: boxHeight

        function place() {
            pos = [Math.floor(Math.random() * gridWidth),
                   Math.floor(Math.random() * gridHeight)];
            x = pos[0] * boxWidth;
            y = pos[1] * boxHeight;
        }

        Component.onCompleted: place()
    }

    Timer {
        id: gameTimer

        interval: 100
        running: true
        repeat: true
        onTriggered: snake.move()
    }

    /*
    ** Keyboard inputs
    */
    focus: true
    property var keyMap: new Array

    Keys.onPressed: {
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
