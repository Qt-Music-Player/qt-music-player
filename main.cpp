#include <mpd/client.h>
#include <iostream>
#include <QApplication>
#include "musicplayer.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
//    struct mpd_connection *con;
//    con = mpd_connection_new(NULL, 0, 30000);
//    if(mpd_connection_get_error(con)!= MPD_ERROR_SUCCESS)
//    {
//        cerr << "Failed to connect to MPD Server";
//        return 1;
//    }
//    else
//    {
//        cout << "Connected YAY!";
//    }

//    mpd_connection_free(con);

    MusicPlayer mp;
    mp.show();


    return a.exec();
}
