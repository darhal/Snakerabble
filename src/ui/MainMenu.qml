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
                /*arena.visible = true
                arena.focus = true*/
                root.visible = false
                arena.launchSP()
            }
        }

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/8)
            text: "Host a game"
            onClicked: {
                root.visible = false
                hostMenu.visible = true
            }
        }

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/8)
            text: "Join a game"
            onClicked: {
                root.visible = false
                joinMenu.visible = true
            }
        }
    }
}
