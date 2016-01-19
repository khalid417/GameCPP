#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QQuickPaintedItem>
#include <memory>
using namespace std;
class GameWidget : public QQuickPaintedItem
{
    Q_OBJECT
public:
    explicit GameWidget(QQuickItem *parent = 0);
    unique_ptr<int[]> generatePuzzle();
    unique_ptr<int[]> generatePuzzle2();
    ~GameWidget();
    Q_INVOKABLE void resetClicked();
    Q_INVOKABLE void newClicked();

private:
    int numCircles;
    unique_ptr<int[]> circleArray;
    bool inCircle(QPoint center, int radius, QPoint cursorPos);
    int side;
    int last;
    bool startState;
    int lastValue;
    QList<int> redList;
    QList<int> blueList;
    int timeRemaining;
    unique_ptr<QTimer> gameTimer;
    bool animationActive;
    int animationFrame;
    unique_ptr<QTimer> animationTimer;
    int lastBlue[2];
    bool lose;
    int win;
    bool gameTimerActive;
    unique_ptr<int[]> circleCache;
    int animationState;
    double ANIMATIONDELAY;
    int MAXCIRCLES;
    double rad;
    bool losingMove;
    enum difficulties{Easy = 0, Regular, Hard};
    QList<int> liveArray;
    QList<QPair<int, int>> neighbors;
protected:
    void paint(QPainter* painter) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event);
};

#endif // GAMEWIDGET_H
