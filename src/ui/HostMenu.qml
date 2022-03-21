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
            text: "Server ID : " + server.getLocalIp()
        }

        TextField {
            width: parent.width
            height: Globals.desktop_height * (1/32)
            placeholderText: "Playername"
        }

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/16)
            text: "Start"
            onClicked: {
                arena.hostGame()
                root.visible = false
            }
        }

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/16)
            text: "< Back"
            onClicked: {
                mainMenu.visible = true
                root.visible = false
            }
        }
    }
}
