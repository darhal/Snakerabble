import QtQuick
import Globals
import "Tools.js" as Tools

Item {
    id: snakeProp
    property var size: [25, 25]
    visible: otherClient.snakeData.positions.length != 0

    Repeater {
        model: otherClient.snakeData.positions.length

        Rectangle {
            width: size[0]
            height: size[1]
            radius: width
            color: 'white'

            Text {
                text: otherClient.snakeData.letters[index]
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                visible: Globals.debug
                anchors.left: parent.right
                color: "red"
                text: "("+otherClient.snakeData.positions[index].x+","+otherClient.snakeData.positions[index].y+")"
            }

            Component.onCompleted: {
                x = Qt.binding(function() { return otherClient.snakeData.positions[index].x * size[0]; });
                y = Qt.binding(function() { return otherClient.snakeData.positions[index].y * size[1]; });
            }
        }
    }
}
