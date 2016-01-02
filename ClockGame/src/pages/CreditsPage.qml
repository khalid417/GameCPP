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
            spacing: 40
            width: parent.width
            TextContent{
                class_name: "h3"
                text: "Made By: Khalid Brown-Walker"
                color: "#4a87ee"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            Image{
                width: 24; height: 24
                fillMode: Image.PreserveAspectFit
                source: "qrc:///Resources/Images/ic_email_black_24dp_1x.png"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            TextContent{
                class_name: "h3"
                text: "Contact email: khalid1@mail.usf.edu"
                color: "#43cee6"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
