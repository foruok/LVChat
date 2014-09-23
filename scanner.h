#ifndef SCANNER_H
#define SCANNER_H
#include <QObject>
#include <QTcpSocket>
#include <QList>

class AccessPoint;
class AccessPointScanner : public QObject
{
    Q_OBJECT
public:
    AccessPointScanner(QObject *parent = 0);
    ~AccessPointScanner();

    void startScan();

signals:
    void newAccessPoint(AccessPoint *ap);
    void scanFinished();

protected slots:
    void onDiscoverSocketDestroyed();

private:
    void scanAps();
    void initializeScanList();
    int ipCount(int prefixLength);
    bool compareAccessPoints();
    void scanOneAp();

private:
    QList<quint32> m_scanList;
    int m_index;
    int m_finished;
};

#endif // SCANNER_H
