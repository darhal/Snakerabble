import QtQuick 2.0
import Globals
import "Tools.js" as Tools

Item {
    id: foodSpawner
    property var size: [25, 25]

    Repeater {
        id: repeater
        model: server.foodSpawner.foodData.length

        Rectangle {
            width: size[0]
            height: size[1]
            radius: width
            color: 'yellow'

            Text {
                text: server.foodSpawner.foodData[index].letter
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                visible: Globals.debug
                anchors.left: parent.right
                color: "red"
                text: "("+server.foodSpawner.foodData[index].x+","+server.foodSpawner.foodData[index].y+")"
            }

            Component.onCompleted: {
                x = Qt.binding(function() { return server.foodSpawner.foodData[index].x * size[0]; });
                y = Qt.binding(function() { return server.foodSpawner.foodData[index].y * size[1]; });
            }
        }
    }

    /*function init(n)
    {
        for (let i = 0; i < n; i++) {
            let pos = [Math.floor(Math.random() * Globals.gridWidth), Math.floor(Math.random() * Globals.gridHeight)]
            positions.push(pos)
            letters.push(Tools.randomLetter())
            // validPositions[pos] = i
            validPositions.set(Tools.vec2ToStr(pos), i)
        }

        // console.log(JSON.stringify([...validPositions.entries()]))
        repeater.model = positions
        // console.log(positions)
    }

    function respawn(idx)
    {
        validPositions.delete(Tools.vec2ToStr(positions[idx]))
        positions[idx] = [Math.floor(Math.random() * Globals.gridWidth), Math.floor(Math.random() * Globals.gridHeight)]
        letters[idx] = Tools.randomLetter()
        validPositions.set(Tools.vec2ToStr(positions[idx]), idx)
        repeater.model = positions
        // console.log(JSON.stringify([...validPositions.entries()]))
    }

    Component.onCompleted: init(10)*/
}
