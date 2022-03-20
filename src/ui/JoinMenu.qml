import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Globals

Item {
    id: root

    Column {
        anchors.centerIn: parent
        width: Globals.desktop_width * (1/4)
        spacing: Globals.padding

        Text {
            width: parent.width
            height: Globals.desktop_height * (1/32)
            text: "Provide server id and name :"
        }

        TextField {
            id: serverId
            width: parent.width
            height: Globals.desktop_height * (1/32)
            placeholderText : "Server ID"
        }

        TextField {
            id: playerName
            width: parent.width
            height: Globals.desktop_height * (1/32)
            placeholderText: "Playername"
        }

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/16)
            text: "Start"
            onClicked: {
                arena.joinGame(playerName.text, server.getIpFromBase64(serverId.text))
                root.visible = false
            }
        }
    }
}
