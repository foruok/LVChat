#ifndef AccessPointModel_H
#define AccessPointModel_H
#include <QAbstractListModel>

class PeerInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nickName READ nickName)
    Q_PROPERTY(QString address READ address)
    Q_PROPERTY(int portraitIndex READ portraitIndex)
public:
    PeerInfo()
        : m_portraitIndex(9)
    {}
    PeerInfo(const QString &nick, int portraitIndex, const QString &strAddr)
        : m_nickName(nick)
        , m_portraitIndex(portraitIndex)
        , m_address(strAddr)
    {}

    PeerInfo(const PeerInfo &r)
        : m_nickName(r.m_nickName)
        , m_portraitIndex(r.m_portraitIndex)
        , m_address(r.m_address)
    {}

    PeerInfo & operator=(const PeerInfo &rhs)
    {
        if(&rhs != this)
        {
            m_nickName = rhs.m_nickName;
            m_portraitIndex = rhs.m_portraitIndex;
            m_address = rhs.m_address;
        }
        return *this;
    }

    ~PeerInfo(){}

    QString nickName() { return m_nickName; }
    int portraitIndex() { return m_portraitIndex; }
    QString address() { return m_address; }

private:
    QString m_nickName;
    int m_portraitIndex;
    QString m_address;
};

class AccessPoint;
class AccessPointModelPrivate;
class AccessPointModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString currentNick READ currentNick)
    Q_PROPERTY(int currentPortraitIndex READ currentPortraitIndex)
public:
    AccessPointModel(QObject *parent = 0);
    ~AccessPointModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void scan();
    Q_INVOKABLE void talkTo(int index);
    Q_INVOKABLE void setCurrent(int index);
    QString currentNick();
    int currentPortraitIndex();

signals:
    void scanFinished();

private slots:
    void onNewAccessPoint(AccessPoint *ap);

private:
    AccessPointModelPrivate *m_dptr;
};

#endif // AccessPointModel_H
