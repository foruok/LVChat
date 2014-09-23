#ifndef VOICEMESSAGE_H
#define VOICEMESSAGE_H
#include <QDateTime>
#include <QString>
class VoiceMessage
{
public:
    VoiceMessage(const QString &fileName, int duration)
        : m_fileName(fileName), m_duration(duration)
        , m_time(QDateTime::currentDateTime())
    {
    }

    QString m_fileName;
    int m_duration;
    QDateTime m_time;
};

#endif // VOICEMESSAGE_H
