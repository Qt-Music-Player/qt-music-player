#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <mpd/client.h>

namespace Ui {
class MusicPlayer;
}

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:

    explicit MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();
    QString getCurrentSongTitle(mpd_song *song);
    QString getCurrentArtist(mpd_song *song);

private slots:

    void on_Play_clicked();
    void on_Pause_clicked();
    void on_PrevSong_clicked();
    void on_NextSong_clicked();
    void updateProgressLabel();
    void on_VolumeSlider_valueChanged(int value);
    void closeEvent(QCloseEvent *end);

private:

    Ui::MusicPlayer *ui;
    struct mpd_connection *con;
    bool Paused;
    int CurrentPosition;
    QTimer *timer;

};

#endif // MUSICPLAYER_H
