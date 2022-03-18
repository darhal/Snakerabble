import QtQuick 2.0
import Globals
import "Tools.js" as Tools

Item {
    id: foodSpawner
    property var size: [25, 25]
    property var positions : []
    property var letters: []
    property var validPositions: []

    Repeater {
        id: repeater
        model: foodSpawner.positions

        Rectangle {
            width: size[0]
            height: size[1]
            radius: width
            color: 'yellow'

            Text {
                text: foodSpawner.letters[index]
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                visible: Globals.debug
                anchors.left: parent.right
                color: "red"
                text: "("+foodSpawner.positions[index][0]+","+foodSpawner.positions[index][1]+")"
            }

            Component.onCompleted: {
                x = Qt.binding(function() { return foodSpawner.positions[index][0] * size[0]; });
                y = Qt.binding(function() { return foodSpawner.positions[index][1] * size[1]; });
            }
        }
    }

    function init(n)
    {
        for (var i = 0; i < n; i++) {
            let pos = [Math.floor(Math.random() * Globals.gridWidth), Math.floor(Math.random() * Globals.gridHeight)]
            positions.push(pos)
            letters.push(Tools.randomLetter())
            validPositions[pos] = i
        }

        repeater.model = positions
        // console.log(positions)
    }

    function respawn(idx)
    {
        console.log("eat")
        positions[idx] = [Math.floor(Math.random() * Globals.gridWidth), Math.floor(Math.random() * Globals.gridHeight)]
        letters[idx] = Tools.randomLetter()
        repeater.model = positions
        validPositions[positions[idx]] = idx
        delete validPositions[positions[idx]]
    }

    Component.onCompleted: init(10)
}
