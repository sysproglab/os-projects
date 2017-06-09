import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window{
    id: phone
    height: Screen.height
    maximumHeight: Screen.height
    minimumHeight: Screen.height
    width: 480
    minimumWidth: 480
    maximumWidth: 480
    title: "Calling"
    visible: true

    Rectangle{
        id: call_info
        color: "blue"
        border.color: "black"
        border.width: 5
        anchors{
            top: parent.top
        }
        height: parent.height / 2
        width: parent.width

        Text {
            id: dialingText
            text: "Dialing:"
            color: "#fbf1c7"
            anchors{
                top: parent.top
                left: parent.left

                topMargin: parent.height / 3
                leftMargin: 20
            }
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "SF"
            font.pointSize: 20
        }

        Text {
            id: call_number
            objectName: "call_number"
            color: "#fbf1c7"
            width: parent.height / 2
            anchors{
                top: dialingText.bottom
                left: parent.left

                topMargin: 10
                leftMargin: 20
            }
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "SF"
            font.pointSize: 20
        }
    }

    Text{
        id: call_timer
        objectName: "call_timer"
        color: "#000000"
        anchors{
            top: parent.top
            right: parent.right

            topMargin: parent.height / 3
            leftMargin: 10
        }
        renderType: Text.NativeRendering
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.family: "SF"
        font.pointSize: 20

    }

    Image {
        id: buttonHang
        width: 100
        height: 50
        source: "qrc:///pics/hang.png"

        anchors {
            bottom: parent.bottom
            bottomMargin: 20
            topMargin: height / 2
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.varticalCenter
        }

        MouseArea{
            anchors.fill: parent
            onClicked: hang()
        }
    }

    Timer{
        interval:1000
        running: true
        onTriggered: getTime()
    }

    function hang(){
        window.hangUp();
    }

    function getTime(){
        window.getTime();
    }

}
