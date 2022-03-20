import QtQuick
import Globals
import "Tools.js" as Tools

Item {
    id: snakeProp
    property int clientId: 0
    property var size: [25, 25]
    visible: client.otherPlayers.length != 0

    Repeater {
        model: client.otherPlayers[clientId].pdata.positions.length

        Rectangle {
            width: size[0]
            height: size[1]
            radius: width
            color: 'white'

            Text {
                text: client.otherPlayers[clientId].pdata.letters[index]
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                visible: Globals.debug
                anchors.left: parent.right
                color: "red"
                text: "("+client.otherPlayers[clientId].pdata.positions[index].x+","+client.otherPlayers[clientId].pdata.positions[index].y+")"
            }

            Component.onCompleted: {
                x = Qt.binding(function() { return client.otherPlayers[clientId].pdata.positions[index].x * size[0]; });
                y = Qt.binding(function() { return client.otherPlayers[clientId].pdata.positions[index].y * size[1]; });
            }
        }
    }
}
