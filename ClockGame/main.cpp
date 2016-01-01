#include "gamewidget.h"
#include "optionscontroller.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtMultimedia/QMediaPlayer>

QMediaPlayer *player;
double globalDelay;
int globalDifficulty;

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    qmlRegisterType<GameWidget>("Game", 1, 0, "GameWidget");
    qmlRegisterType<OptionsController>("Options", 1, 0, "Controller");
    player = new QMediaPlayer;
    player->setMedia(QUrl(QStringLiteral("qrc:/Resources/Sounds/theme.mp3")));
    player->setVolume(50);
    player->play();
    globalDelay = 1000;
    globalDifficulty = 1;
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return a.exec();
}
