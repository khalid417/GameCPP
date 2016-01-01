#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QQuickPaintedItem>

class GameWidget : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit GameWidget(QQuickItem *parent = 0, int difficulty = 0, int animationSpeed = 0);
    int * generatePuzzle();
    int * generatePuzzle2();
    ~GameWidget();
    Q_INVOKABLE void resetClicked();
    Q_INVOKABLE void newClicked();

private:
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
    bool gameTimerActive;
    int * circleCache;
    int animationState;
protected:
    void paint(QPainter* painter) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event);
};

#endif // GAMEWIDGET_H
