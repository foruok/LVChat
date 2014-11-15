#ifndef AccessPointModel_H
#define AccessPointModel_H
#include <QAbstractListModel>

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
