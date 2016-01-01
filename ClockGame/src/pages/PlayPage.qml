import QtQuick 2.0
import QtQuick.Layouts 1.1
import Game 1.0
import "../buttons"
import "../variables/fontawesome.js" as FontAwesome
Item {
    width: parent.width
    height: parent.height
    GameWidget{
        id: aGame
        anchors.centerIn: parent
        width: 800; height: 800
    }
    Column{
        anchors{ horizontalCenter: parent.horizontalCenter; bottom: parent.bottom; bottomMargin: 20;}
        width: parent.width
        spacing: 40
        ButtonDefault{
            class_name: "calm"
            text: "Reset"
            onClicked: aGame.resetClicked()
            width: parent.width
        }

        ButtonDefault{
            class_name: "calm"
            text: "New Puzzle"
            onClicked: aGame.newClicked()
            width: parent.width
        }
    }
}
