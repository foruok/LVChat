#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

class SimpleAudioPlayer : public QObject
{
    Q_OBJECT
public:
    SimpleAudioPlayer();
    ~SimpleAudioPlayer();

    Q_INVOKABLE void play(const QString &filePath);
    Q_INVOKABLE void stop();

signals:
    void playError();
    void playFinished();

protected slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onError(QMediaPlayer::Error error);
    void onStateChanged(QMediaPlayer::State state);

private:
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;
};

#endif // AUDIOPLAYER_H
