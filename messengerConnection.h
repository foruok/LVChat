#ifndef ServerConnection_H
#define ServerConnection_H
#include <QObject>
#include <QTcpSocket>
#include "protocol.h"
#include <QByteArray>
#include <QHostAddress>
#include <QPointer>

class Contact;
class Peer;
class AccessPoint;

class DiscoverConnection : public QTcpSocket
{
    Q_OBJECT
public:
    DiscoverConnection(QObject *parent = 0);
    ~DiscoverConnection();

    void connectAp(quint32 addr, quint16 port);

signals:
    void newAccessPoint(AccessPoint *ap);

protected:
    void timerEvent(QTimerEvent *e);

protected slots:
    void onConnected();
    void onReadyRead();

private:
    int m_nTimeout;
    QByteArray m_data;
};

class TalkingConnection : public QObject
{
    Q_OBJECT
public:
    TalkingConnection(QTcpSocket *sock = 0, QObject *parent = 0);
    ~TalkingConnection();

    void accept();
    void reject();
    void talkTo(quint32 ip, quint16 port);
    void sendVoice(QByteArray &data, int duration, char *format);
    QString peerAddress();

signals:
    void talkingAccepted();
    void talkingRejected();
    void incomingMessenger(Peer *peer);
    void incomingVoice(QString fileName, int duration);
    void peerGone(quint32 peerAddr); //error or peer quit app

protected slots:
    void onConnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError code);

protected:
    void timerEvent(QTimerEvent *e);

private:
    void processPacket();
    void replyRequest(bool agree);
    void discoverReply();
    int processMessage();
    void closeConnection();
    void setupSignalSlots();

private:
    QTcpSocket *m_socket;
    bool m_bReady;
    int m_nTimeout;
    int m_packetType;
    quint32 m_messageType;
    quint32 m_messageLength;
    QByteArray m_data;
    qint32 m_duration;
    QByteArray m_format;
    quint32 m_peerAddress;
};

class Contact
{
public:
    Contact(): m_portraitIndex(0)
    {
    }
    Contact(const QString &nickName, int portraitIndex)
        : m_nickName(nickName), m_portraitIndex(portraitIndex)
    {}

    virtual ~Contact(){}

    QString m_nickName;
    int m_portraitIndex;
};

class AccessPoint : public Contact
{
public:
    AccessPoint(const QString &nickName, int portraitIndex, const QHostAddress &addr)
        : Contact(nickName, portraitIndex)
        ,m_address(addr)
    {
    }
    ~AccessPoint()
    {
    }

    QHostAddress m_address;
};

class Peer : public Contact
{
public:
    Peer(const QString &nickName, int portraitIndex, TalkingConnection *conn)
        : Contact(nickName, portraitIndex)
        ,m_connection(conn)
    {

    }
    ~Peer()
    {
        if(m_connection)m_connection->reject();
    }

    QPointer<TalkingConnection> m_connection;
};

#endif // ServerConnection_H
