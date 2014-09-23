import QtQuick 2.2

Rectangle {
    id: btn;
    implicitWidth: 48;
    implicitHeight: 48;
    color: "transparent";
    property var iconNormal;
    property var iconPressed;
    signal clicked();
    Image {
        id: icon;
        anchors.fill: parent;
        anchors.margins: 1;
        fillMode: Image.PreserveAspectFit;
    }

    onIconNormalChanged: icon.source = iconNormal;

    MouseArea {
        anchors.fill: parent;
        onPressed: {
            if(btn.iconPressed != undefined){
                icon.source = iconPressed;
            }else{
                border.width = 1;
                border.color = "gray";
            }
        }
        onReleased: {
            if(btn.iconPressed == undefined){
                border.width = 0;
                border.color = "gray";
            }
            icon.source = iconNormal;
        }
        onClicked: btn.clicked();
    }

    Component.onCompleted: {
        icon.source = iconNormal;
    }
}
