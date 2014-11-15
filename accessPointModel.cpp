#include "accessPointModel.h"
#include <QVector>
#include "scanner.h"
#include "messengerConnection.h"
#include <QDebug>
#include "messengerManager.h"

typedef QVector<QString> VideoData;
class AccessPointModelPrivate
{
public:
    AccessPointModelPrivate()
        : m_current(0)
    {
        int role = Qt::UserRole;
        m_roleNames.insert(role++, "nickName");
        m_roleNames.insert(role++, "portraitIndex");
        m_roleNames.insert(role++, "address");
    }

    ~AccessPointModelPrivate()
    {
        clear();
    }

    void clear()
    {
        int count = m_accessPoints.size();
        if(count > 0)
        {
            for(int i = 0; i < count; i++)
            {
                delete m_accessPoints.at(i);
            }
            m_accessPoints.clear();
        }
    }

    QHash<int, QByteArray> m_roleNames;
    QVector<AccessPoint*> m_accessPoints;
    AccessPointScanner m_scanner;
    int m_current;
};

AccessPointModel::AccessPointModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_dptr(new AccessPointModelPrivate)
{
    connect(&(m_dptr->m_scanner), SIGNAL(scanFinished()),
            this, SIGNAL(scanFinished()));
    connect(&(m_dptr->m_scanner), SIGNAL(newAccessPoint(AccessPoint*)),
            this, SLOT(onNewAccessPoint(AccessPoint*)));
}

AccessPointModel::~AccessPointModel()
{
    delete m_dptr;
}

int AccessPointModel::rowCount(const QModelIndex &parent) const
{
    return m_dptr->m_accessPoints.size();
}

QVariant AccessPointModel::data(const QModelIndex &index, int role) const
{
    AccessPoint *d = m_dptr->m_accessPoints[index.row()];
    switch(role - Qt::UserRole)
    {
    case 0:
        return d->m_nickName;
    case 1:
        return d->m_portraitIndex;
    case 2:
        return d->m_address.toString();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> AccessPointModel::roleNames() const
{
    return m_dptr->m_roleNames;
}

void AccessPointModel::scan()
{
    beginResetModel();

    m_dptr->clear();
    m_dptr->m_scanner.startScan();

    endResetModel();
}

void AccessPointModel::talkTo(int index)
{
    m_dptr->m_current = index;
    MessengerManager::instance()->talkTo(m_dptr->m_accessPoints.at(index));
}

QString AccessPointModel::currentNick()
{
    AccessPoint *ap = m_dptr->m_accessPoints.at(m_dptr->m_current);
    return ap->m_nickName;
}

int AccessPointModel::currentPortraitIndex()
{
    AccessPoint *ap = m_dptr->m_accessPoints.at(m_dptr->m_current);
    return ap->m_portraitIndex;
}

void AccessPointModel::onNewAccessPoint(AccessPoint *ap)
{
    int count = m_dptr->m_accessPoints.size();
    beginInsertRows(QModelIndex(), count, count);
    m_dptr->m_accessPoints.append(ap);
    endInsertRows();
}



