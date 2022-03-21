import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Globals

Item {
    id: root

    Column {
        anchors.centerIn: parent
        width: row.width
        spacing: 5 * Globals.padding

        Row {
            id: row
            spacing: Globals.padding
            Image {
                source: "qrc:/assets/logo.png"
                sourceSize: Qt.size(156,155)
            }

            Text {
                text: "Snakerabble"
                font.pixelSize: 45
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Column {
            id: btnsCol
            width: parent.width
            spacing: Globals.padding

            Button {
                width: Globals.desktop_width * (1/4)
                height: Globals.desktop_height * (1/16)
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Singleplayer"
                onClicked: {
                    //server.broadcastGameData();
                    /*arena.visible = true
                    arena.focus = true*/
                    root.visible = false
                    spMenu.visible = true
                }
            }

            Button {
                width: Globals.desktop_width * (1/4)
                height: Globals.desktop_height * (1/16)
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Host a game"
                onClicked: {
                    root.visible = false
                    hostMenu.visible = true
                }
            }

            Button {
                width: Globals.desktop_width * (1/4)
                height: Globals.desktop_height * (1/16)
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Join a game"
                onClicked: {
                    root.visible = false
                    joinMenu.visible = true
                }
            }
        }
    }
}
