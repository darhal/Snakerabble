import QtQuick
import Globals
import "Tools.js" as Tools

Item {
    id: snake
    property var size: [25, 25]
    property var parts: [[20, 20], [20, 21], [20, 22], [21, 22], [22, 22]]
    property var letters: ["A", "B", "C", "D", "E"]
    property int dir: Tools.Direction.UP
    property bool canChangeDir: true

    function move() {
        var head = parts[0].slice(0);
        var body = parts;
        body.pop(); // Move body
        Tools.move(head, dir, Globals.gridWidth, Globals.gridHeight); // Move head

        // Bit itself ?
        if (Tools.posInArray(head, body))
            return die();

        var foodIdx = foodSpawner.validPositions[head]

        if (foodIdx) {
            eat(body, foodIdx)
        }

        // Bit the food ?
        //if (Tools.checkPos(head, food.pos))
        //    eat(body);
        parts = [head].concat(body);
        canChangeDir = true;
    }

    function eat(body, foodIdx) {
        let foodPos = foodSpawner.positions[foodIdx]
        body.unshift(foodPos);
        letters.unshift(foodSpawner.letters[foodIdx])
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
        model: snake.parts

        Rectangle {
            width: size[0]
            height: size[1]
            radius: width
            color: 'white'

            Text {
                text: letters[index]
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                visible: Globals.debug
                anchors.left: parent.right
                color: "red"
                text: "("+snake.parts[index][0]+","+snake.parts[index][1]+")"
            }

            Component.onCompleted: {
                x = Qt.binding(function() { return snake.parts[index][0] * size[0]; });
                y = Qt.binding(function() { return snake.parts[index][1] * size[1]; });
            }
        }
    }
}
