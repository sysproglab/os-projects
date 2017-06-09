import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Rectangle {

	property string caption: ""
	property int spacing

	id: button1
	width: parent.width / 3 - 2 * spacing / 3
	height: parent.height / 4 - 3 * spacing / 4
			
	Text {
		renderType: Text.NativeRendering
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
		font.family: "SF"
		font.pointSize: 20
		text: caption 
		width: parent.width
		height: parent.height
	 }


	MouseArea{
		opacity:1
		anchors.fill: parent
		onClicked: phoneNumber.text += caption
		onDoubleClicked: {
			if(caption != 0) return
			phoneNumber.text = phoneNumber.text.substr(0, phoneNumber.text.length-1)
			phoneNumber.text += "+"
		}
	}
}
