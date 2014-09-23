#include "scanner.h"
#include <QNetworkInterface>
#include <math.h>
#include <QTimer>
#include "messengerConnection.h"
#include <QDebug>

AccessPointScanner::AccessPointScanner(QObject *parent)
    : QObject(parent)
    ,m_index(0), m_finished(0)
{
    initializeScanList();
}

AccessPointScanner::~AccessPointScanner()
{

}

void AccessPointScanner::startScan()
{
    if(m_scanList.size() > 0)
    {
        scanAps();
    }
    else
    {
        qDebug() << "none scan list";
        emit scanFinished();
    }
}

void AccessPointScanner::onDiscoverSocketDestroyed()
{
    m_finished++;
    if(++m_index < m_scanList.size())
    {
        scanOneAp();
        return;
    }
    if(m_finished == m_scanList.size())
    {
        m_index = 0;
        m_finished = 0;
        emit scanFinished();
        qDebug() << "emit scanFinished";
    }
}

void AccessPointScanner::scanOneAp()
{
    DiscoverConnection *conn = new DiscoverConnection();
    connect(conn, SIGNAL(newAccessPoint(AccessPoint *)),
            this, SIGNAL(newAccessPoint(AccessPoint *)));
    connect(conn, SIGNAL(destroyed()), this, SLOT(onDiscoverSocketDestroyed()));
    conn->connectAp(m_scanList.at(m_index), SERVER_PORT);
    //qDebug() << "scaning " << QHostAddress(m_scanList.at(m_index));
}

void AccessPointScanner::scanAps()
{
    int limit = qMin(m_scanList.size(), 10);
    for(; m_index < limit; ++m_index)
    {
        scanOneAp();
    }
    --m_index;
}

void AccessPointScanner::initializeScanList()
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface, interfaces)
    {
        int flags = interface.flags();
        if(flags & QNetworkInterface::IsLoopBack) continue;
        if(!(flags &(QNetworkInterface::IsUp | QNetworkInterface::IsRunning))) continue;

        QList<QNetworkAddressEntry> entries = interface.addressEntries();
        int count = entries.size();
        for(int i = 0; i < count; i++)
        {
            const QNetworkAddressEntry &entry = entries.at(i);
            QHostAddress ip = entry.ip();
            if(!ip.isLoopback() && ip.protocol() == QAbstractSocket::IPv4Protocol)
            {
                quint32 myself = ip.toIPv4Address();
                int numberOfIps = ipCount(entry.prefixLength());
                quint32 startIp = (ip.toIPv4Address() & entry.netmask().toIPv4Address())+1;
                for(int j = 1; j < numberOfIps; j++, startIp++)
                {
                    if(myself != startIp)
                    {
                        m_scanList.append(startIp);
                        //QHostAddress address(startIp);
                        //qDebug() << address;
                    }
                }
            }
        }
    }
}

int AccessPointScanner::ipCount(int prefixLength)
{
    int count = 0;
    int unmask = 32 - prefixLength;
    for(int i = 0; i < unmask; i++)
    {
        count += pow(2, i);
    }
    return count;
}
