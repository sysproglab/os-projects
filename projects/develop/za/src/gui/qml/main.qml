import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import "core"

Window{
	id: phone
	height: Screen.height
	maximumHeight: Screen.height
	minimumHeight: Screen.height
    width:  480
    minimumWidth: 480
    maximumWidth: 480
	title: "Phone"
	visible: true

	Image {
		id: buttonBack
		width: 170
		height: 70
		source: "qrc:///pics/back.png"

   	    anchors{
			top: parent.top
			left: parent.left
			leftMargin: 10
		}

		MouseArea{
			anchors.fill: parent
			onClicked:Qt.quit()
		}
	}

	Image {
		id: buttonDelete
		width: 70
		height: 70
   	    source: "qrc:///pics/erase.png"

   	    anchors{
			top: parent.top
			topMargin: 20
			right: parent.right
			rightMargin: 10
		}

   	    MouseArea{
   	    	anchors.fill: parent
   	   		onClicked: phoneNumber.text = phoneNumber.text.substr(0, phoneNumber.text.length - 1)
   	   	}
   	}

	Text {
		id: phoneNumber
		objectName: "number"
		text: ""
		font.pixelSize: 30
		wrapMode: Text.WrapAnywhere
		anchors {
			left: parent.left
			leftMargin: 10
			right: buttonDelete.left
			rightMargin: 10
			top: buttonBack.bottom
			topMargin: 20
		}
	}

	Rectangle {
		id: buttons
		width: parent.width
		height: 2 *  parent.height / 3
		color: "#fbf1c7"
		anchors{
			bottom: parent.bottom
		}
		
	Rectangle{
			id: table
			width: parent.width - parent.width * 0.07 * 2
			height: parent.height / 2 
			color: "#282828"
			
			anchors{
				top: parent.top
				left: parent.left
				leftMargin: parent.width * 0.07
				rightMargin: parent.width * 0.07
				topMargin: height / 3 
			}

			Grid {
				id: numbers
				spacing: 2 
				columns: 3
				width: parent.width
				height: parent.height
				anchors{
					horizontalCenter: parent.horizontalCenter
					verticalCenter: parent.verticalCenter
				}

				Button {caption : "1"; spacing: parent.spacing; color: buttons.color}
				Button {caption : "2"; spacing: parent.spacing; color: buttons.color}
				Button {caption : "3"; spacing: parent.spacing; color: buttons.color}

				Button {caption : "4"; spacing: parent.spacing; color: buttons.color}
				Button {caption : "5"; spacing: parent.spacing; color: buttons.color}
				Button {caption : "6"; spacing: parent.spacing; color: buttons.color}

				Button {caption : "7"; spacing: parent.spacing; color: buttons.color}
				Button {caption : "8"; spacing: parent.spacing; color: buttons.color}
				Button {caption : "9"; spacing: parent.spacing; color: buttons.color}

				Button {caption : "*"; spacing: parent.spacing; color: buttons.color}
				Button {caption : "0"; spacing: parent.spacing; color: buttons.color}
				Button {caption : "#"; spacing: parent.spacing; color: buttons.color}
			}
		}

		Image {
			id: buttonDial
			width: 70
			height: 70
			source: "qrc:///pics/dial.png"

			anchors {
				top: table.bottom
				topMargin: height / 2
				horizontalCenter: parent.horizontalCenter
				verticalCenter: parent.varticalCenter
			}

			MouseArea{
				anchors.fill: parent
				onClicked:dial(phoneNumber)
			}
		}


	}

	function dial(object){
        window.dialNumber(object.text);
	}
}
