import QtQuick 2.2

Rectangle {
    id: acceptView;
    anchors.fill: parent;
    implicitWidth: 320;
    implicitHeight: 240;
    color: "darkgray";
    property alias text: content.text;
    signal accepted();
    signal rejected();

    Text {
        id: content;
        anchors.centerIn: parent;
        verticalAlignment: Text.AlignVCenter;
        horizontalAlignment: Text.AlignHCenter;
        font.pointSize: 24;
        height: 10 + fontUtil.height(24);
        wrapMode: Text.WordWrap;
    }

    Row {
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottom: parent.bottom;
        anchors.bottomMargin: 12;
        spacing: 8;

        FlatButton {
            iconWidth: 48;
            iconHeight: 48;
            width: 60 + fontUtil.width(20, text);
            height: Math.max(48, 8 + fontUtil.height(20));
            iconSource: "res/ic_accept.png";
            font.pointSize: 20;
            text: qsTr("Accept");
            onClicked: acceptView.accepted();
        }
        FlatButton{
            iconWidth: 48;
            iconHeight: 48;
            width: 60 + fontUtil.width(20, text);
            height: Math.max(48, 8 + fontUtil.height(20));
            iconSource: "res/ic_reject.png";
            font.pointSize: 20;
            text: qsTr("Reject");
            onClicked: acceptView.rejected();
        }
    }
}
