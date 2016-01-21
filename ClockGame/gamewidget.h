#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QtWidgets>
#include <QQuickPaintedItem>
#include <memory>
using namespace std;
class GameWidget : public QQuickPaintedItem
{
    Q_OBJECT
public:
    // Contructor
    explicit GameWidget(QQuickItem *parent = 0);
    ~GameWidget() = default;

    // QML invokable functions
    Q_INVOKABLE void resetClicked();
    Q_INVOKABLE void newClicked();

private:
    // Member variables
    bool startState;
    bool animationActive;
    bool lose;
    bool gameTimerActive;
    bool losingMove;
    int numCircles;
    int side;
    int last;
    int lastValue;
    int timeRemaining;
    int animationFrame;
    int win;
    int animationState;
    int maxCircles;
    double ANIMATIONDELAY;
    double rad;
    unique_ptr<QTimer> gameTimer;
    unique_ptr<QTimer> animationTimer;

    // Member arrays and lists
    int lastBlue[2];
    unique_ptr<int[]> circleArray;
    unique_ptr<int[]> circleCache;
    QList<int> redList;
    QList<int> blueList;
    QList<int> liveArray;
    QList<QPair<int, int>> neighbors;

    // Enumerations
    enum difficulties{Easy = 0, Regular, Hard};

    // Private Functions
    bool inCircle(QPoint center, int radius, QPoint cursorPos);
    unique_ptr<int[]> generatePuzzle();
    unique_ptr<int[]> generatePuzzle2();

protected:
    // QQuick inherited
    void paint(QPainter* painter) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event);
};

#endif // GAMEWIDGET_H
