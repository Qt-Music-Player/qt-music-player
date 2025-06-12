#include "musicplayer.h"
#include "ui_musicplayer.h"
#include <mpd/client.h>
#include <iostream>
#include <QTimer>

using namespace std;


MusicPlayer::MusicPlayer(QWidget *parent) : QWidget(parent), ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);
    con = mpd_connection_new(NULL, 0, 30000);
    if(mpd_connection_get_error(con)!= MPD_ERROR_SUCCESS)
    {
        cerr << "Failed to connect to MPD Server";
    }
    Paused =false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressLabel()));
    timer->start(1000);
    mpd_run_set_volume(con, 50);

}

MusicPlayer::~MusicPlayer()
{
    delete ui;
    mpd_connection_free(con);
    delete timer;
}

QString MusicPlayer::getCurrentSongTitle(mpd_song *song)
{
    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    if (title != NULL)
    {
        return QString(title);
    }
    return QString("No Title");
}

QString MusicPlayer::getCurrentArtist(mpd_song *song)
{
    const char *artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0);
    if (artist != NULL)
    {
        return QString(artist);
    }
    return QString("No Artist");
}

void MusicPlayer::on_Play_clicked()
{
    mpd_run_play(con);
    Paused =false;
    struct mpd_song *song=mpd_run_current_song(con);
    QString title = getCurrentSongTitle(song);
    QString artist = getCurrentArtist(song);
    mpd_song_free(song);
    ui->SongTitle->setText(title);
    ui->Artist->setText(artist);
}

void MusicPlayer::on_Pause_clicked()
{
    bool success = mpd_run_pause(con, !Paused);
    if(success)
    {
        Paused = !Paused;
    }
    else
    {
        cerr<<"Could Not Pause";
    }
}

void MusicPlayer::on_PrevSong_clicked()
{
    mpd_run_previous(con);
    struct mpd_song *song=mpd_run_current_song(con);
    QString title = getCurrentSongTitle(song);
    QString artist = getCurrentArtist(song);
    mpd_song_free(song);
    ui->SongTitle->setText(title);
    ui->Artist->setText(artist);
}

void MusicPlayer::on_NextSong_clicked()
{
    mpd_run_next(con);
    struct mpd_song *song=mpd_run_current_song(con);
    if (song!=NULL)
    {
        QString title = getCurrentSongTitle(song);
        QString artist = getCurrentArtist(song);
        mpd_song_free(song);
        ui->SongTitle->setText(title);
        ui->Artist->setText(artist);
    }
    else
    {
        mpd_run_play_pos(con,0); //goes back to first song if the next is empty.
        struct mpd_song *song=mpd_run_current_song(con);
        QString title = getCurrentSongTitle(song);
        QString artist = getCurrentArtist(song);
        mpd_song_free(song);
        ui->SongTitle->setText(title);
        ui->Artist->setText(artist);
    }

}

void MusicPlayer::updateProgressLabel()
{
    struct mpd_status *status = mpd_run_status(con);
    if(status!=NULL)
    {
        int elapsed = mpd_status_get_elapsed_time(status);
        int total = mpd_status_get_total_time(status);

        QString progress = QString("%1:%2 / %3:%4")
                .arg(elapsed/60, 2, 10, QLatin1Char('0'))
                .arg(elapsed%60, 2, 10, QLatin1Char('0'))
                .arg(total/60, 2, 10, QLatin1Char('0'))
                .arg(total%60, 2, 10, QLatin1Char('0'));
        ui->ProgressLabel->setText(progress);
        mpd_status_free(status);
    }
    else
    {
        ui->ProgressLabel->setText("0:00/0:00");
    }
}

void MusicPlayer::on_VolumeSlider_valueChanged(int value)
{
    mpd_run_set_volume(con, value);
}

void MusicPlayer::closeEvent(QCloseEvent *event)
{
    mpd_run_stop(con);
    QWidget::closeEvent(event);
}
