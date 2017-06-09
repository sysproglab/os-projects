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
            anchors{
                top: dialingText.bottom
                left: parent.left

                topMargin: dialingText.height
                leftMargin: 20
            }
            renderType: Text.NativeRendering
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.family: "SF"
            font.pointSize: 20
        }
	}
}
