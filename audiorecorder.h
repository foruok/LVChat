#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H
#include <QString>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>

class SimpleAudioRecorder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QString file READ file)
public:
    SimpleAudioRecorder(const QString &saveDir);
    ~SimpleAudioRecorder();

    Q_INVOKABLE bool isValid()
    {
        return m_bValid;
    }

    QString device()
    {
        return m_recorder.audioInput();
    }

    QString codec()
    {
        return m_settings.codec();
    }

    int sampleRate()
    {
        return m_settings.sampleRate();
    }

    QString container()
    {
        return m_recorder.containerFormat();
    }

    int bitrate()
    {
        return m_settings.bitRate();
    }

    int channels()
    {
        return m_settings.channelCount();
    }

    QMultimedia::EncodingQuality quality()
    {
        return m_settings.quality();
    }

    /*
     * current record
     */
    Q_INVOKABLE void record();
    Q_INVOKABLE void stop();
    /* return the full path of the current record */
    QString file();
    qint64 duration();
    Q_INVOKABLE bool success();
signals:
    void durationChanged(qint64 duration);

protected slots:
    void onDurationChanged(qint64 duration);
    void onStatusChanged(QMediaRecorder::Status);
    void onError(QMediaRecorder::Error);

private:
    bool setupRecorder();

private:
    QString m_baseDir;
    QAudioRecorder m_recorder;
    QAudioEncoderSettings m_settings;
    bool m_bValid;
    QString m_device;
    QString m_container;
    qint64 m_duration;
    bool m_bSuccess;
    QString m_filePath;
};

#endif // AUDIORECORDER_H
