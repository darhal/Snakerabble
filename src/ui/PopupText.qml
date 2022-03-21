import QtQuick 2.0

Item {
    property alias textItem: textItem
    property alias comboItem: comboItem
    property bool activate: true
    width: Math.max(textItem.paintedWidth, comboItem.paintedWidth)
    height: textItem.paintedHeight + comboItem.paintedHeight

    Text {
        id: textItem
        font.pixelSize: 80
        color: "white"
        text: ""
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        Behavior on text {
            FadeAnimation {
                target: textItem
            }
        }

        Text {
            id: comboItem
            color: "white"
            text: ""
            font.pixelSize: 45
            anchors.top: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Behavior on text {
                FadeAnimation {
                    target: comboItem
                }
            }
        }

        Timer {
            interval: 2000
            running: activate && textItem.text != ""
            repeat: false
            onTriggered: {
                textItem.text = "";
                comboItem.text = "";
            }
        }
    }
}
