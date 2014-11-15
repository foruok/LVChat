import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {
    id: contacts;
    anchors.fill: parent;
    signal talkTo(int peerIndex);
    signal startScan();
    objectName: "contacts";
    function scanAccessPoint(){
        startScan();
        scanButton.enabled = false;
        apmodel.scan();
    }

    Rectangle {
        id: title;
        width: parent.width;
        height: 48;
        color: "#7378ba";
        ImageButton {
            id: scanButton;
            anchors.centerIn: parent;
            width: 48;
            height: 48;
            z: 2;
            iconNormal: "res/ic_search.png";
            iconPressed: "res/ic_search_pressed.png";
            onClicked: scanAccessPoint();
        }
    }

    ListView {
        id: peoples;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: title.bottom;
        anchors.bottom: requestTalk.top;
        anchors.margins: 4;
        clip: true;
        spacing: 6;
        highlight: Rectangle {
            width: peoples.width;
            color: "lightsteelblue";
        }

        model: apmodel;
        delegate: Item{
            id: wrapper;
            width: parent.width;
            height: Math.max(52, 8 + fontUtil.height(20));
            Image {
                id: headPortrait;
                source: "res/head_%1.png".arg(portraitIndex);
                width: 48;
                height: 48;
                x: 2;
                y: 2;
            }

            Text {
                id: nick;
                font.pointSize: 20;
                anchors.left: headPortrait.right;
                anchors.leftMargin: 4;
                anchors.verticalCenter: headPortrait.verticalCenter;
                text: nickName;
            }

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    wrapper.ListView.view.currentIndex = index;
                }
            }
        }
    }

    Rectangle {
        id: requestTalk;
        width: parent.width;
        height: 64;
        anchors.bottom: parent.bottom;
        color: "gray"; //"#605550";
        ImageButton {
            anchors.centerIn: parent;
            width: 60;
            height: 64;
            z: 2;
            iconNormal: "res/ic_chat.png";
            iconPressed: "res/ic_chat_pressed.png";
            onClicked: {
                var cur = peoples.currentIndex;
                if(cur >= 0){
                    contacts.talkTo(cur);
                }
            }
        }
    }

    Connections {
        target: apmodel;
        onScanFinished:{
            console.log("scanFinished");
            scanButton.enabled = true;
        }
    }
}
