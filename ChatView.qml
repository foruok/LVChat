import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.2
import QtMultimedia 5.0

Item {
    id: chat;
    anchors.fill: parent;
    signal finished();
    objectName: "chat";
    property alias nick: nickName.text;
    property int peerPortraitIndex: 9;
    property int portraitIndex: 9;
    Rectangle {
        id: title;
        width: parent.width;
        height: Math.max(48, 8 + fontUtil.height(20));
        color: "#7378ba"; //"#203050";
        ImageButton {
            id: back;
            anchors.left: parent.left;
            anchors.verticalCenter: parent.verticalCenter;
            anchors.margins: 4;
            width: 48;
            height: 48;
            z: 2;
            iconNormal: "res/ic_back.png";
            iconPressed: "res/ic_back_pressed.png";
            onClicked: chat.finished();
        }
        Text {
            id: nickName;
            anchors.left: back.right;
            anchors.leftMargin: 6;
            anchors.right: parent.right;
            anchors.verticalCenter: back.verticalCenter;
            verticalAlignment: Text.AlignVCenter;
            font.pointSize: 20;
            color: "white";
        }
    }

    TextField {
        id: recordState;
        width: parent.width - 40;
        height: 20 + fontUtil.height(24);
        anchors.centerIn: parent;
        z: 10;
        readOnly: true;
        visible: false;
        horizontalAlignment: TextInput.AlignHCenter;
        verticalAlignment: TextInput.AlignVCenter;
        style: TextFieldStyle {
            textColor: "red";
            background: Rectangle {
                color: "lightgray";
                radius: 2
                border.color: "#444444";
                border.width: 1
            }
        }

        function onDurationChanged(duration){
            text = "%1\"".arg(Math.ceil(duration/1000));
        }
        states:[
            State {
                name: "hide";
                PropertyChanges{
                    target: recordState;
                    restoreEntryValues: false;
                    opacity: 0;
                }
            },
            State {
                name: "show";
                PropertyChanges{
                    target: recordState;
                    restoreEntryValues: false;
                    opacity: 1;
                    visible: true;
                }
            }
        ]
        state: "hide";
        transitions: [
            Transition {
                from: "hide";
                to: "show";
                NumberAnimation {
                    property: "opacity";
                    duration: 800;
                }
            },
            Transition {
                from: "show";
                to: "hide";
                SequentialAnimation {
                    NumberAnimation {
                        properties: "opacity";
                        duration: 1500;
                    }

                    PropertyAction {
                        target: recordState;
                        property: "visible";
                        value: false;
                    }
                }
            }
        ]
    }

    Connections {
        target: audioRecorder;
        onDurationChanged: recordState.onDurationChanged(duration);
    }

    Rectangle {
        id: pressTalk;
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 4;
        property string text: qsTr("Press & Talk");
        property bool pressed: false;
        height: Math.max(8 + fontUtil.height(20), 50);
        width: 64 + fontUtil.width(20, text);
        border.color: "gray";
        border.width: 1;
        radius: 4;
        color: pressTalk.pressed ? "lightgray" : "gray";
        Image {
            id: microPhone;
            anchors.left: parent.left;
            anchors.leftMargin: 4;
            anchors.verticalCenter: parent.verticalCenter;
            width: 48;
            height: 48;
            source: "res/ic_microphone.png";
        }
        Text{
            anchors.left: microPhone.right;
            anchors.leftMargin: 2;
            anchors.right: parent.right;
            height: parent.height;
            font.pointSize: 20;
            text: parent.text;
            color: parent.pressed ? "blue" : "black";
            verticalAlignment: Text.AlignVCenter;
            horizontalAlignment: Text.AlignHCenter;
        }

        MouseArea {
            anchors.fill: parent;
            onPressed: {
                pressTalk.pressed = true;
                audioRecorder.record();
                recordState.visible = true;
                recordState.text = qsTr("Recording...");
                recordState.state = "show";
            }
            onReleased: {
                pressTalk.pressed = false;
                audioRecorder.stop();
                if(audioRecorder.success()){
                    chatList.model.append(
                                {
                                    "direction": 0,
                                    "audioFile": audioRecorder.file,
                                    "duration": Math.round(audioRecorder.duration/1000)
                                });
                    console.log("recorded, ", audioRecorder.file);
                    msgManager.sendVoiceMessage(audioRecorder.file, audioRecorder.duration);
                }
                recordState.state = "hide";
            }
        }

        z: 2;
    }

    MediaPlayer {
        id: player;
    }

    ListView {
        id: chatList;
        anchors.left: parent.left;
        anchors.right: parent.right;
        anchors.top: title.bottom;
        anchors.bottom: pressTalk.top;
        anchors.margins: 4;
        clip: true;
        spacing: 6;
        model: ListModel {}
        delegate: Item{
            width: parent.width;
            height: Math.max(52, 8 + fontUtil.height(20));
            Image {
                id: headPortrait;
                source: "res/head_%1.png".arg(direction == 0 ? chat.portraitIndex : chat.peerPortraitIndex);
                width: 48;
                height: 48;
                x: direction == 0 ? parent.width - width -2 : 2;
                y: 2;
            }

            BorderImage {
                id: msgWrapper;
                source: direction == 0 ? (pressed ? "res/chat_to_bg_pressed.9.png":"res/chat_to_bg_normal.9.png") : (pressed ? "res/chat_from_bg_pressed.9.png":"res/chat_from_bg_normal.9.png");
                width: 140; //TODO: caculate via PPI
                height: parent.height;
                x: direction == 0 ? headPortrait.x - 2 - width : headPortrait.x + headPortrait.width + 2;
                border.left: direction == 0 ? 6 : 16;
                border.top: 38;
                border.right: direction == 0 ? 16 : 6;
                border.bottom: 6;
                property bool pressed: false;
                property string filePath: audioFile;
                Image {
                    id: voice;
                    source: "res/ic_voice.png";
                    mirror: direction == 0;
                    width: 45;
                    height: 45;
                    x: direction == 0 ? parent.width - width - 2 - msgWrapper.border.right : 2 + msgWrapper.border.left;
                    y: (parent.height - height)/2;
                }

                Text {
                    x: direction == 0 ? 2 : voice.x + voice.width + 4;
                    y: voice.y;
                    height: voice.height;
                    width: parent.width - 24 - voice.width;
                    text: "%1\"".arg(duration);
                    color: "black";
                    horizontalAlignment: direction == 0 ? Text.AlignRight : Text.AlignLeft;
                    verticalAlignment: Text.AlignVCenter;
                    font.pointSize: 16;
                }

                MouseArea {
                    anchors.fill: parent;
                    preventStealing: true;
                    onPressed: msgWrapper.pressed = true;
                    onReleased: msgWrapper.pressed = false;
                    onClicked: {
                        console.log("audioFile-", msgWrapper.filePath);
                        player.source = (Qt.platform.os === "windows" ? "file:///" : "file://") + msgWrapper.filePath;
                        player.play();
                    }
                }
            }
        }
    }

    Connections {
        target: msgManager;
        onVoiceMessageArrived:{
            chatList.model.append(
                        {
                            "direction": 1,
                            "audioFile": fileName,
                            "duration": Math.round(duration / 1000)
                        });
        }
    }

    function changePeer(nickName, portraitIndex){
        nick = nickName;
        peerPortraitIndex = portraitIndex;
        chatList.model.clear();
    }
}
