#include "gamewidget.h"
#include "optionscontroller.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QMediaPlayer *player;
double globalDelay;
int globalDifficulty;
QMediaPlaylist *playlist;

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    qmlRegisterType<GameWidget>("Game", 1, 0, "GameWidget");
    qmlRegisterType<OptionsController>("Options", 1, 0, "Controller");
    player = new QMediaPlayer;
    playlist = new QMediaPlaylist;
    playlist->addMedia(QUrl(QStringLiteral("qrc:/Resources/Sounds/theme.mp3")));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    playlist->setCurrentIndex(1);
    player->setPlaylist(playlist);
    player->setVolume(50);
    player->play();
    globalDelay = 1000;
    globalDifficulty = 1;
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return a.exec();
}
