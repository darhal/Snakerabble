import QtQuick
import Globals
import "Tools.js" as Tools

Item {
    id: snake
    property var size: [25, 25]
    //property var parts: [[20, 20], [20, 21], [20, 22], [21, 22], [22, 22]]
    //property var letters: ["A", "B", "C", "D", "E"]
    property int dir: Tools.Direction.UP
    property bool canChangeDir: true

    function move() {
        let sd = client.snakeController.snakeData
        var head = [sd.positions[0].x, sd.positions[0].y];

        // Bit itself ?
        //if (Tools.posInArray(head, body))
        //    return die();

        // Bit the food ?
        var foodIdx = foodSpawner.validPositions.get(Tools.vec2ToStr(head))
        if (foodIdx != null) {
            eat(foodIdx)
        }

        client.snakeController.move(dir, Globals.gridWidth, Globals.gridHeight)
        canChangeDir = true;
    }

    function eat(foodIdx) {
        let foodPos = foodSpawner.positions[foodIdx]
        client.snakeController.eat(foodSpawner.positions[foodIdx][0], foodSpawner.positions[foodIdx][1], foodSpawner.letters[foodIdx])
        foodSpawner.respawn(foodIdx)
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

    Repeater {
        model: client.snakeController.snakeData.positions.length

        Rectangle {
            width: size[0]
            height: size[1]
            radius: width
            color: 'white'

            Text {
                text: client.snakeController.snakeData.letters[index]
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                visible: Globals.debug
                anchors.left: parent.right
                color: "red"
                text: "("+client.snakeController.snakeData.positions[index].x+","+client.snakeController.snakeData.positions[index].y+")"
            }

            Component.onCompleted: {
                x = Qt.binding(function() { return client.snakeController.snakeData.positions[index].x * size[0]; });
                y = Qt.binding(function() { return client.snakeController.snakeData.positions[index].y * size[1]; });
            }
        }
    }
}
