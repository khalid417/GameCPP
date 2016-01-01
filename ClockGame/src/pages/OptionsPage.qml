import QtQuick 2.0
import QtQuick.Layouts 1.1
import Options 1.0
import "../variables/fontawesome.js" as FontAwesome
import "../bars"
import "../content"
import "../buttons"
import "../variables/base.js" as StyleHelper
Item {
    width: parent.width
    height: parent.height
    Controller{
        id:aController
        volume: 50
        difficulty: 1
        muted: false
        animationSpeed: 2
    }
    Item {
        width: parent.width - 20
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        Column {
            spacing: 100
            width: parent.width
            TextContent{
                class_name: "h3"
                text: "Difficulty"
                color: "#4a87ee"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            ButtonBar{
               class_name: "positive"
               checkable: true
               model: [
                   {
                       text: 'Easy'
                   },
                   {
                       text: 'Regular'
                   },
                   {
                       text: 'Hard'
                   }
               ]
               checkedIndex: 1
               width: parent.width
               onCheckedIndexChanged: {
                   console.log('Difficulty: ' + model[checkedIndex].text);
                   aController.difficultyClicked(checkedIndex);
               }
            }
            TextContent{
                class_name: "h3"
                text: "Animation Speed"
                color: "#43cee6"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            ButtonBar{
               class_name: "calm"
               checkable: true
               model: [
                   {
                       text: 'Very Slow'
                   },
                   {
                       text: 'Slow'
                   },
                   {
                       text: 'Medium'
                   },
                   {
                       text: 'Fast'
                   },
                   {
                       text: 'Very Fast'
                   }

               ]
               checkedIndex: 2
               width: parent.width
               onCheckedIndexChanged: {
                   console.log('Speed: ' + model[checkedIndex].text);
                   aController.animationSpeedClicked(checkedIndex);
               }
            }
            TextContent{
                class_name: "h3"
                text: "Sound"
                color: "#f0b840"
                anchors.horizontalCenter: parent.horizontalCenter
            }
            ButtonBar{
               class_name: "energized"
               checkable: true
               model: [
                   {
                       text: 'On'
                   },
                   {
                       text: 'Off'
                   }
               ]
               checkedIndex: 0
               width: parent.width
               onCheckedIndexChanged: {
                   console.log('Sound ' + model[checkedIndex].text);
                   aController.volumeToggleClicked(checkedIndex);
               }
            }
            RowLayout{
                width: parent.width - 20
                anchors.horizontalCenter: parent.horizontalCenter
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Column{
                    ButtonDefault {
                        class_name: "energized outline"
                        text: "Down"
                        icon: FontAwesome.icons.fa_volume_down
                        onClicked: {
                            aController.volumeDownClicked();
                        }
                    }
                }
                Column{
                    ButtonDefault {
                        class_name: "energized outline"
                        text: "Up"
                        icon: FontAwesome.icons.fa_volume_up
                        onClicked: {
                            aController.volumeUpClicked();
                        }
                    }
                }
            }
        }
    }
}
