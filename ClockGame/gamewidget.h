#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QtMultimedia/QMediaPlayer>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);
    int * generatePuzzle();
    int * generatePuzzle2();
    ~GameWidget();

private:
    Ui::GameWidget *ui;
    int numCircles;
    int * circleArray;
    bool inCircle(QPoint center, int radius, QPoint cursorPos);
    int side;
    int last;
    bool startState;
    int lastValue;
    QList<int> redList;
    QList<int> blueList;
    int timeRemaining;
    QTimer *gameTimer;
    bool animationActive;
    int animationFrame;
    QTimer *animationTimer;
    int lastBlue[2];
    bool lose;
    int win;
    bool paused;
    bool gameStarted;
    bool gameOver;
    QHBoxLayout *topLayout;
    QVBoxLayout *pauseLayout;
    QVBoxLayout *auxLayout;
    QMediaPlayer *player;
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event);
};

#endif // GAMEWIDGET_H
