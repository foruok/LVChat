#ifndef MESSENGER_MANAGER_H
#define MESSENGER_MANAGER_H
#include <QTcpServer>
#include "messengerConnection.h"

class MessengerManager : public QTcpServer
{
    Q_OBJECT
    MessengerManager(QObject *parent = 0);
public:
    ~MessengerManager();

    Q_INVOKABLE void acceptNewMessenger();
    Q_INVOKABLE void rejectNewMessenger();
    Q_INVOKABLE void closeCurrentSession();

    static Contact * s_contact;
    Q_INVOKABLE void start(QString nickName, int portraitIndex);

    void talkTo(AccessPoint *ap);

    static MessengerManager *instance();
    Q_INVOKABLE void sendVoiceMessage(QString fileName, qint64 duration);

signals:
    void newMessenger(QString name, int portraitIndex, QString address);
    void chatAccepted();
    void chatRejected();
    void voiceMessageArrived(QString fileName, int duration);
    void peerGone();

protected slots:
    void onNewConnection();
    void onIncomingMessenger(Peer *peer);

private:
    void setupSignalSlots(TalkingConnection *conn);

private:
    Peer * m_currentPeer;
    Peer * m_pendingPeer;
};

#endif // MessengerManager_H
