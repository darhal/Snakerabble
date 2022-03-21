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

        TextField {
            id: speedField
            width: parent.width
            height: Globals.desktop_height * (1/32)
            placeholderText : "Speed (1-10)"
        }

        TextField {
            id: foodField
            width: parent.width
            height: Globals.desktop_height * (1/32)
            placeholderText: "Food Abundancy"
        }

        Button {
            width: Globals.desktop_width * (1/4)
            height: Globals.desktop_height * (1/16)
            text: "Start"
            onClicked: {
                var speed = parseInt(speedField.text)
                var food = parseInt(foodField.text)
                arena.gameTimer.interval = (10/speed) * 10;
                //arena.gameTimer.start();
                arena.launchSP(food)
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
