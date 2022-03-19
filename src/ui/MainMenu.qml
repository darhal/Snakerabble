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

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/8)
            text: "Singleplayer"
            onClicked: {
                //server.broadcastGameData();
                root.visible = false
                arena.visible = true
                arena.focus = true
            }
        }

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/8)
            text: "Host a game"
            onClicked: {
                server.hostGame();
            }
        }

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/8)
            text: "Join a game"
            onClicked: {
                client.joinGame()
                root.visible = false
                arena.visible = true
            }
        }
    }
}
