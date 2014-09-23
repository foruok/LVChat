import QtQuick 2.2
import QtQuick.Window 2.1
import QtQuick.Controls 1.2

Window {
    id: root;
    visible: true;
    width: 320;
    height: 480;
    title: qsTr("LVChat");
    color: "darkgray";
    property var currentView: null;
    property string peerNickName;
    property int portraitIndex: 9;
    property int peerPortraitIndex: 9;
    property string queuedPeerNickName;
    property int queuedPeerPortraitIndex: 9;
    function onBack(){
        if(currentView.objectName == "talk") {
            currentView.back();
        }else{
            Qt.quit();
        }
    }

    function populateContacts(){
        currentView.destroy();
        var contactComp = Qt.createComponent("Contacts.qml");
        currentView = contactComp.createObject(root);
        currentView.talkTo.connect(onTalkTo);
        currentView.startScan.connect(onStartScan);
    }

    function onLogin(nickName, index){
        portraitIndex = index;
        msgManager.start(nickName, index);
        populateContacts();
        currentView.scanAccessPoint();
    }

    function onTalkTo(peerIndex){
        peerNickName = apmodel.currentNick;
        peerPortraitIndex = apmodel.currentPortraitIndex;
        apmodel.talkTo(peerIndex);
        currentView.enabled = false;
        waiting.running = true;
    }

    /* relative to talkTo
     *
     */
    function onAccepted(){
        waiting.running = false;
        currentView.destroy();
        var chatComp = Qt.createComponent("ChatView.qml");
        currentView = chatComp.createObject(root,
                                            {
                                                "nick": root.peerNickName,
                                                "peerPortraitIndex": root.peerPortraitIndex,
                                                "portraitIndex": root.portraitIndex
                                            });
        currentView.finished.connect(onChatFinished);
    }

    function onRejected(){
        if(currentView.objectName == "contacts"){
            currentView.enabled = true;
            waiting.running = false;
            //TODO: show sorry
        }else if(currentView.objectName == "chat"){
            onChatFinished();
        }
    }

    function onChatFinished(){
        msgManager.closeCurrentSession();
        populateContacts();
    }

    function onStartScan(){
        waiting.running = true;
    }

    BusyIndicator {
        id: waiting;
        anchors.centerIn: parent;
        z: 10;
        running: false;
    }

    property var questionView;
    function onAcceptPeer(){
        questionView.destroy();
        msgManager.acceptNewMessenger();
        peerNickName = queuedPeerNickName;
        peerPortraitIndex = queuedPeerPortraitIndex;
        if(currentView.objectName == "chat"){
            currentView.changePeer(root.peerNickName, root.peerPortraitIndex);
        }else if(currentView.objectName == "contacts"){
            onAccepted();
        }
    }
    function onRejectPeer(){
        questionView.destroy();
        msgManager.rejectNewMessenger();
    }

    /*
     * a Peer want to chat with me!
     */
    function onChatRequest(name, portraitIndex, address){
        queuedPeerNickName = name;
        queuedPeerPortraitIndex = portraitIndex;
        var comp = Qt.createComponent("AcceptView.qml");
        questionView = comp.createObject(root,
                                         {
                                             "text": "[%1]%2".arg(name).arg(qsTr("want to talk with you."))
                                         });
        questionView.accepted.connect(onAcceptPeer);
        questionView.rejected.connect(onRejectPeer);
    }

    function onPeerGone(){
        if(currentView.objectName == "chat"){
            populateContacts();
            currentView.scanAccessPoint();
        }
    }

    Component.onCompleted: {
        var loginComp = Qt.createComponent("LoginView.qml", Component.PreferSynchronous);
        currentView = loginComp.createObject(root);
        currentView.go.connect(onLogin);
        msgManager.chatAccepted.connect(onAccepted);
        msgManager.chatRejected.connect(onRejected);
        msgManager.newMessenger.connect(onChatRequest);
        msgManager.peerGone.connect(onPeerGone);
    }

    Connections {
        target: apmodel;
        onScanFinished: waiting.running = false;
    }
}
