import QtQuick 2.0
import QtQuick.Layouts 1.1
import "../variables/fontawesome.js" as FontAwesome
import "../content"
import "../variables/base.js" as StyleHelper
Item {
    width: parent.width
    height: parent.height
    Item {
        width: parent.width
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        ColumnLayout {
            spacing: 20
            width: parent.width - 20
            TextContent{
                class_name: "h4"
                text: "Begin by pressing one of the blue circles"
                color: "#4a87ee"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Image{
                width: 80; height: 80
                fillMode: Image.PreserveAspectFit
                source: "qrc:///Resources/Images/HelpImage1.png"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            TextContent{
                class_name: "h4"
                text: "The pressed circle will disappear and the next blue circles will be that\n\t number of spaces clockwise and counter clockwise"
                color: "#4a87ee"
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 20
            }
            Image{
                width: 80; height: 80
                fillMode: Image.PreserveAspectFit
                source: "qrc:///Resources/Images/HelpImage2.png"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            TextContent{
                class_name: "h4"
                text: "Win by getting to the last circle before time runs out!"
                color: "#4a87ee"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
