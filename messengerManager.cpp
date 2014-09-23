#include "messengerManager.h"
#include <QFile>
#include <QDebug>

Contact *MessengerManager::s_contact = 0;
MessengerManager::MessengerManager(QObject *parent)
    : QTcpServer(parent), m_currentPeer(0), m_pendingPeer(0)
{
    if(!s_contact)
    {
        s_contact = new Contact();
    }
    connect(this, SIGNAL(newConnection()),
            this, SLOT(onNewConnection()));
}

MessengerManager::~MessengerManager()
{

}

void MessengerManager::acceptNewMessenger()
{
    if(m_currentPeer)
    {
        delete m_currentPeer;
    }
    m_currentPeer = m_pendingPeer;
    m_currentPeer->m_connection->accept();
    m_pendingPeer = 0;
    setupSignalSlots(m_currentPeer->m_connection);
}

void MessengerManager::rejectNewMessenger()
{
    if(m_pendingPeer)
    {
        delete m_pendingPeer;
        m_pendingPeer = 0;
    }
}

void MessengerManager::closeCurrentSession()
{
    if(m_currentPeer)
    {
        delete m_currentPeer;
        m_currentPeer = 0;
    }
}

void MessengerManager::start(QString nickName, int portraitIndex)
{
    s_contact->m_nickName = nickName;
    s_contact->m_portraitIndex = portraitIndex;
    listen(QHostAddress::AnyIPv4, SERVER_PORT);
}

void MessengerManager::talkTo(AccessPoint *ap)
{
    rejectNewMessenger();
    closeCurrentSession();

    TalkingConnection *conn = new TalkingConnection();
    m_currentPeer = new Peer(ap->m_nickName, ap->m_portraitIndex, conn);
    conn->talkTo(ap->m_address.toIPv4Address(), SERVER_PORT);
    setupSignalSlots(conn);
}

MessengerManager *MessengerManager::instance()
{
    static MessengerManager *server = new MessengerManager();
    return server;
}

void MessengerManager::sendVoiceMessage(QString fileName, qint64 duration)
{
    if(m_currentPeer)
    {
        QFile file(fileName);
        if( file.open(QFile::ReadOnly) )
        {
            int sep = fileName.lastIndexOf('.');
            QString format = fileName.mid(sep+1);
            QByteArray data = file.readAll();
            file.close();
            m_currentPeer->m_connection->sendVoice(data, duration, format.toLatin1().data());
        }
        else
        {
            qDebug() << "could not open - " << fileName << " error - " << file.errorString();
        }
    }
}

void MessengerManager::onNewConnection()
{
    while(hasPendingConnections())
    {
        TalkingConnection *m = new TalkingConnection(nextPendingConnection());
        connect(m, SIGNAL(incomingMessenger(Peer*)),
                this, SLOT(onIncomingMessenger(Peer*)));
    }
}

void MessengerManager::onIncomingMessenger(Peer *peer)
{
    if(m_pendingPeer)
    {
        //sorry...we just allow one pending peer
        delete peer;
    }
    else
    {
        m_pendingPeer = peer;
        emit newMessenger(peer->m_nickName,
                          peer->m_portraitIndex,
                          peer->m_connection->peerAddress());
    }
}

void MessengerManager::setupSignalSlots(TalkingConnection *conn)
{
    connect(conn, SIGNAL(talkingAccepted()), this, SIGNAL(chatAccepted()));
    connect(conn, SIGNAL(talkingRejected()), this, SIGNAL(chatRejected()));
    connect(conn, SIGNAL(incomingVoice(QString,int)), this, SIGNAL(voiceMessageArrived(QString,int)));
    connect(conn, SIGNAL(peerGone(quint32)), this, SIGNAL(peerGone()));
}
