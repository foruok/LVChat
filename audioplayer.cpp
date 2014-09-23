#include "audioplayer.h"
#include <QDebug>

SimpleAudioPlayer::SimpleAudioPlayer()
{
    m_player = new QMediaPlayer(this);
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus))
            ,this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(m_player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(onStateChanged(QMediaPlayer::State)));
    connect(m_player, SIGNAL(error(QMediaPlayer::Error)),
            this, SLOT(onError(QMediaPlayer::Error)));

    m_playlist = new QMediaPlaylist(this);
    m_player->setPlaylist(m_playlist);
}

SimpleAudioPlayer::~SimpleAudioPlayer()
{

}

void SimpleAudioPlayer::play(const QString &filePath)
{
    qDebug() << "try to play - " << filePath;
    stop();
    m_playlist->addMedia(QUrl::fromLocalFile(filePath));
    m_playlist->setCurrentIndex(0);
    m_player->play();
}

void SimpleAudioPlayer::stop()
{
    m_player->stop();
    m_playlist->clear();
}

void SimpleAudioPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
    case QMediaPlayer::EndOfMedia:
        qDebug() << "EndOfMedia";
        emit playFinished();
        break;
    case QMediaPlayer::InvalidMedia:
        qDebug() << "invalidMedia";
        emit playError();
        break;
    case QMediaPlayer::BufferingMedia:
        break;
    case QMediaPlayer::BufferedMedia:
        break;
    case QMediaPlayer::LoadingMedia:
        break;
    case QMediaPlayer::LoadedMedia:
        break;
    }
}

void SimpleAudioPlayer::onError(QMediaPlayer::Error error)
{
    qDebug() << "play error " << error;
    emit playError();
}

void SimpleAudioPlayer::onStateChanged(QMediaPlayer::State state)
{

}

