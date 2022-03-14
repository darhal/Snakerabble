import QtQuick

Canvas {
    id: root
    focus: true

    Component {
        id: comp
        PathCurve { }
    }

    property var points : [
        comp.createObject(myPath, {"x": 75, "y": 75})
    ]
    contextType: "2d"

    Path {
        id: myPath
        startX: points[0].x; startY: points[0].y
        pathElements: points
    }

    onPaint: {
        context.lineWidth = 5;
        context.strokeStyle = Qt.rgba(.4,.6,.8);
        context.path = myPath;
        context.stroke();
    }

    function move(x, y)
    {
        for (var i = 0; i < points.length - 1; i++) {
            points[i].x = points[i+1].x
            points[i].y = points[i+1].y
        }

        points[points.length - 1].x += x
        points[points.length - 1].y += y
    }

    Keys.onPressed: (event) => {
        var i = 0;

        switch (event.key) {
            case Qt.Key_Left:
                move(-10, 0)
            break;
            case Qt.Key_Right:
                move(10, 0)
            break;
            case Qt.Key_Up:
                move(0, -10)
            break;
            case Qt.Key_Down:
                move(0, 10)
            break;
            case Qt.Key_Return:
                var last = points[0]
                points.unshift(comp.createObject(myPath, {"x":last.x, "y":last.y}))
                console.log("crate")
            break;
        }

        myPath.pathElements = points;
        context.reset();
        root.requestPaint()
        event.accepted = true;
    }
}
