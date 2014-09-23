import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Item {
    id: login;
    anchors.fill: parent;
    anchors.margins: 10;
    implicitWidth: 300;
    implicitHeight: 102;
    signal go(string nick, int portraitIndex);
    property int portraitIndex: 9;
    RowLayout {
        anchors.fill: parent;
        spacing: 2;
        ImageButton {
            id: headPortrait;
            width: 48;
            height: 48;
            Layout.alignment: Qt.AlignVCenter;
            iconNormal: "res/head_9.png";
            function onHeadPortraitSelected(index){
                login.portraitIndex = index;
                iconNormal = "res/head_%1.png".arg(index);
            }
            onClicked: login.showHeadPortraits(onHeadPortraitSelected);
        }
        Rectangle {
            border.width: 1;
            border.color: "gray";
            Layout.fillWidth: true;
            Layout.alignment: Qt.AlignVCenter;
            height: 12 + fontUtil.height(22);
            color: "transparent";
            TextInput {
                anchors.margins: 2;
                anchors.fill: parent;
                id: nickEditor;
                text: "nickname";
                font.pointSize: 22;
                verticalAlignment: TextInput.AlignVCenter;
            }
        }
        ImageButton {
            width: 48;
            height: 48;
            Layout.alignment: Qt.AlignVCenter;
            iconNormal: "res/ic_ok.png";
            iconPressed: "res/ic_ok_pressed.png";
            onClicked: login.go(nickEditor.text, login.portraitIndex);
        }
    }

    property var selector: null;
    function showHeadPortraits(fn){
        selector = headComp.createObject(login,
                                         {
                                             "z":2,
                                             "callback":fn,
                                             "width": width
                                         });
        selector.selected.connect(onHeadPanelClosed);
    }
    function onHeadPanelClosed(){
        selector.destroy();
        selector = null;
    }

    Component {
        id: headComp;
        Rectangle {
            id: wrapper;
            property var callback;
            signal selected();
            anchors.centerIn: parent;
            implicitWidth: 260;
            implicitHeight: 104;
            color: "lightgray";
            border.width: 1;
            border.color: "gray";
            radius: 4;
            Grid {
                id: headPortraits;
                anchors.centerIn: parent;
                anchors.margins: 2;
                columns: 5;
                columnSpacing: 2;
                rows: 2;
                rowSpacing: 2;

                Repeater {
                    delegate: ImageButton{
                        width: 48;
                        height: 48;
                        iconNormal: "res/head_%1.png".arg(index);
                        onClicked: {
                            wrapper.callback(index);
                            wrapper.selected();
                            wrapper.visible = false;
                        }
                    }
                    model: 10;
                }
            }
        }
    }
}
