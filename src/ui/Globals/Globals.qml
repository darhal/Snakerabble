pragma Singleton

import QtQuick 2.15
import QtQuick.Window 2.15

QtObject {
    readonly property bool is_mobile: Qt.platform.os === "ios" || Qt.platform.os === "android"
    readonly property int desktop_width : 375
    readonly property int desktop_height: 667
    readonly property int screen_width : is_mobile ? Screen.width  : desktop_width
    readonly property int screen_height: is_mobile ? Screen.height : desktop_height
    readonly property int padding: 10
}
