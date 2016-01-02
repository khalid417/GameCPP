#include "gamewidget.h"
#include "random.h"
#include <QTime>
#include <QList>
#include <math.h>
#define PI 3.14159265358979
#define TIMERESET 12
#define ANIMATIONENDFRAME 60.0

extern double globalDelay;
extern int globalDifficulty;
extern int mode;

GameWidget::GameWidget(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    int gameTimerDelay;
    ANIMATIONDELAY = globalDelay/ANIMATIONENDFRAME;
    switch(globalDifficulty)
    {
        case difficulties::Easy:
            gameTimerDelay = 2000;
            MAXCIRCLES = 8;
            rad = 5;
            break;
        case difficulties::Regular:
            gameTimerDelay = 1000;
            MAXCIRCLES = 12;
            rad = 4;
            break;
        case difficulties::Hard:
            gameTimerDelay = 200;
            MAXCIRCLES = 18;
            rad = 2.7;
            break;
        default:
            gameTimerDelay = 1000;
            MAXCIRCLES = 12;
            rad = 4;
    }
    setOpaquePainting(false);
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    lose = false;
    win = false;
    animationState = 0;
    numCircles = MAXCIRCLES;
    gameTimer = new QTimer(this);
    animationTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, [this](){if(gameTimerActive) timeRemaining--;});
    connect(animationTimer, &QTimer::timeout, [this](){
        if(animationActive) {
            animationFrame++;
        }
        update();
    });
    gameTimer->start(gameTimerDelay);
    animationTimer->start(ANIMATIONDELAY);
    side = 800;
    startState = true;
    timeRemaining = 0;
    animationActive = false;
    losingMove = false;
    gameTimerActive = false;
    if(mode == 1)
    {
        circleArray = generatePuzzle2();
        circleCache = new int [MAXCIRCLES];
        for(int i = 0; i < MAXCIRCLES; ++i)
            circleCache[i] = circleArray[i];
    }
    else
    {
        timeRemaining = TIMERESET * 2;
        circleArray = generatePuzzle();
        for(int i = 0; i < MAXCIRCLES; ++i)
            liveArray.append(i);
        for(int i = 0; i < MAXCIRCLES; ++i)
            neighbors.append(QPair<int, int>((i+circleArray[i])%MAXCIRCLES, ((i+MAXCIRCLES-circleArray[i])%MAXCIRCLES)));
    }
    update();
}

GameWidget::~GameWidget()
{
    delete gameTimer;
    delete animationTimer;
    delete circleArray;
    if(mode == 1) delete circleCache;
}
int * GameWidget::generatePuzzle2()
{
    int * puzzleArray = new int[numCircles];
    Random rng(QTime::currentTime().msecsSinceStartOfDay());
    QList<int> puzzleList;
    int currentNodeValue = rng.nextInt(numCircles / 2) + 1;
    puzzleList.push_back(currentNodeValue);
    int prevPos = 0;
    for (int i = 2; i <= numCircles; i++) {
        int * jArray = new int[i - 1];
        int jIndex = 0;
        for (int j = 1; j <= i / 2; j++) {
            jArray[jIndex++] = j;
            if (jIndex < i - 1) {
                jArray[jIndex++] = i - j;
            }
        }
        jIndex = rng.nextInt(i - 1);
        int j = jArray[jIndex];
        int m = rng.nextInt(numCircles / 2);
        currentNodeValue = (i * m + j) % ((numCircles / 2))+1;
        int cw = rng.nextInt(2);
        int insertLoc = jIndex / 2 + 1;
        puzzleList.insert((i + prevPos + (cw < 1 ? insertLoc : -insertLoc)) % i, currentNodeValue);
        prevPos = (i + prevPos + (cw < 1 ? insertLoc : -insertLoc)) % i;
    }
    for (int i = 0; i < numCircles; i++) {
        puzzleArray[i] = puzzleList.at(i);
    }
    return puzzleArray;
}

int * GameWidget::generatePuzzle()
{
    int * puzzleArray = new int[numCircles];
            for (int i = 0; i < numCircles; i++) {
                puzzleArray[i] = 0;
            }
            Random rng(QTime::currentTime().msecsSinceStartOfDay());
            int currentNode = rng.nextInt(numCircles);
            while (puzzleArray[currentNode] == 0) {
                int currentNodeValue = rng.nextInt(numCircles / 2) + 1;
                bool clockwise = rng.nextBoolean();
                if (clockwise) {
                    if (puzzleArray[(currentNode + currentNodeValue + numCircles) % numCircles] == 0) {
                        puzzleArray[currentNode] = currentNodeValue;
                        currentNode = (currentNode + currentNodeValue + numCircles) % numCircles;
                        continue;
                    }
                }
                if (puzzleArray[(currentNode - currentNodeValue + numCircles) % numCircles] == 0) {
                    puzzleArray[currentNode] = currentNodeValue;
                    currentNode = (currentNode - currentNodeValue + numCircles) % numCircles;
                    continue;
                }
                if (!clockwise) {
                    if (puzzleArray[(currentNode + currentNodeValue + numCircles) % numCircles] == 0) {
                        puzzleArray[currentNode] = currentNodeValue;
                        currentNode = (currentNode + currentNodeValue + numCircles) % numCircles;
                        continue;
                    }
                }
                //Exit criterion
                int i = currentNode + 1;
                while (i != currentNode) {
                    i = (i + numCircles) % numCircles;
                    if (puzzleArray[i] == 0) {
                        break;
                    }
                    i = (i + 1 + numCircles) % numCircles;
                    if (i == currentNode) {
                        puzzleArray[currentNode] = currentNodeValue;
                    }
                }
            }
            return puzzleArray;
}

void GameWidget::paint(QPainter *painter)
{
    QPoint minuteHand[3] =
    {
        QPoint(5, 8),
        QPoint(-5, 8),
        QPoint(0, -50*(4.0/rad))
    };
    static const QPoint secondHand[3] =
    {
        QPoint(4, 8),
        QPoint(-4, 8),
        QPoint(0, -80)
    };
    if (mode == 1 && startState)
    {
        for(int i = 0; i < numCircles; ++i)
            blueList << i;
    }
    int radius = side / 16;
    QColor circleColorBlue(Qt::GlobalColor::blue);
    QColor circleColorRed(Qt::GlobalColor::red);
    QColor textColor(Qt::GlobalColor::black);
    QColor circleColorGreen(Qt::GlobalColor::green);
    QList<QPoint> circleGraph;
    for (int i = 0; i < numCircles; ++i)
    {
        double degree = (2 * PI / numCircles) * i;
        QPoint circle = QPoint(cos(degree) * side / rad, sin(degree) * side / rad);
        circleGraph.append(circle);
    }
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(width() / 2, height() / 2);
    if (win)
    {
        painter->setPen(textColor);
        painter->setFont(QFont("Times New Roman", 60));
        painter->drawText(-width() / 4, 0, "YOU WIN!");
        return;
    }
    if (lose || (timeRemaining <= 0 && gameTimerActive))
    {
        painter->setPen(textColor);
        painter->setFont(QFont("Times New Roman", 60));
        painter->drawText(-width() / 4, 0, "YOU LOSE!");
        return;
    }
    if(mode == 1)
    {
        if(!animationActive || (animationState != 2 && animationState != 3))
        {
            int circleNum = 0;
            for (QPoint circle : circleGraph)
            {
                if(redList.contains(circleNum) || (animationActive && (animationState == 0 || animationState == 1))) painter->setBrush(circleColorRed);
                else painter->setBrush(circleColorBlue);
                if (animationActive == true && circleNum == last) painter->setBrush(circleColorGreen);
                painter->drawEllipse(circle, radius, radius);
                circleNum++;
            }

            painter->setPen(textColor);
            painter->setFont(QFont("Times New Roman", 30));
            for (int i = 0 ; i < numCircles ; i++)
            {
                painter->drawText(circleGraph.at(i), QString::number(circleArray[i]));
            }
        }
    }
    else
    {
        QColor circleColorGray(Qt::GlobalColor::gray);
        int circleNum = 0;
        for (QPoint circle : circleGraph)
        {
            painter->setBrush(circleColorGray);
            if(startState) painter->setBrush(circleColorBlue);
            if(!startState && liveArray.contains(circleNum))
            {
                painter->setBrush(circleColorRed);
                if(circleNum == neighbors.at(last).first || circleNum == neighbors.at(last).second) painter->setBrush(circleColorBlue);
                if(animationActive) painter->setBrush(circleColorRed);
            }
            painter->drawEllipse(circle, radius, radius);
            circleNum++;
        }

        painter->setPen(textColor);
        painter->setFont(QFont("Times New Roman", 30));
        for (int i = 0 ; i < numCircles ; i++)
        {
            painter->drawText(circleGraph.at(i), QString::number(circleArray[i]));
        }
    }
    if(animationActive)
    {
        QColor ColorAnimate(Qt::GlobalColor::green);
        if (animationState == 0) //move hands to last
        {
            painter->scale(side / 200.0, side / 200.0);
            painter->setPen(Qt::NoPen);
            painter->setBrush(ColorAnimate);
            if(mode == 1 || liveArray.contains(lastBlue[0]))
            {
                painter->save();
                painter->rotate((360.0/numCircles) * (lastBlue[0] + (last - lastBlue[0])*animationFrame/ANIMATIONENDFRAME) + 90);
                painter->drawConvexPolygon(minuteHand, 3);
                painter->restore();
            }
            if(mode == 1 || liveArray.contains(lastBlue[1]))
            {
                painter->rotate((360.0/numCircles) * (lastBlue[1] + (last - lastBlue[1])*animationFrame/ANIMATIONENDFRAME) + 90);
                painter->drawConvexPolygon(minuteHand, 3);
            }
            if(mode == 1)
            {
                if(animationFrame >= ANIMATIONENDFRAME || lastBlue[0] == lastBlue[1] || numCircles == MAXCIRCLES)
                {
                    animationFrame = 0;
                    animationState = 1;
                }
            }
            else
            {
                if(animationFrame >= ANIMATIONENDFRAME || liveArray.size() == numCircles || !liveArray.contains(lastBlue[1]) || !liveArray.contains(lastBlue[0]))
                {
                    animationFrame = 0;
                    animationState = 1;
                }
            }
        }
        else if (animationState == 1) //move hands to next blue
        {
            painter->scale(side / 200.0, side / 200.0);
            double degree = (360.0/numCircles)*(lastValue);
            painter->setPen(Qt::NoPen);
            painter->setBrush(ColorAnimate);
            if(mode == 1 || liveArray.contains(neighbors.at(last).second))
            {
                painter->save();
                painter->rotate((360.0/numCircles) * (last) - (degree * animationFrame/ANIMATIONENDFRAME) + 90);
                painter->drawConvexPolygon(minuteHand, 3);
                painter->restore();
            }
            if(mode == 1 || liveArray.contains(neighbors.at(last).first))
            {
                painter->rotate((360.0/numCircles) * (last) + (degree * animationFrame/ANIMATIONENDFRAME) + 90);
                painter->drawConvexPolygon(minuteHand, 3);
            }
            if(animationFrame >= ANIMATIONENDFRAME)
            {
                animationFrame = 0;
                if (mode == 1)
                    animationState = 2;
                else animationState = 4;
            }
        }
        else if (animationState == 2) //move old last out
        {
            int circleNum = 0;
            for (QPoint circle : circleGraph)
            {
                if(redList.contains(circleNum)) painter->setBrush(circleColorRed);
                else painter->setBrush(circleColorBlue);
                if (circleNum == last)
                {
                    painter->save();
                    painter->setBrush(circleColorGreen);
                    painter->setOpacity((ANIMATIONENDFRAME-animationFrame)/ANIMATIONENDFRAME);
                    painter->drawEllipse(circle, radius, radius);
                    circleNum++;
                    painter->restore();
                    continue;
                }
                painter->drawEllipse(circle, radius, radius);
                circleNum++;
            }

            painter->setPen(textColor);
            painter->setFont(QFont("Times New Roman", 30));
            for (int i = 0 ; i < numCircles ; i++)
            {
                if(i == last)
                {
                    painter->save();
                    painter->setOpacity((ANIMATIONENDFRAME-animationFrame)/ANIMATIONENDFRAME);
                    painter->drawText(circleGraph.at(i), QString::number(circleArray[i]));
                    painter->restore();
                    continue;
                }
                painter->drawText(circleGraph.at(i), QString::number(circleArray[i]));
            }
            painter->scale(side / 200.0, side / 200.0);

            painter->setPen(Qt::NoPen);
            painter->setBrush(ColorAnimate);

            painter->save();
            painter->rotate((360.0/numCircles) * (blueList.at(0)) + 90);
            painter->drawConvexPolygon(minuteHand, 3);
            painter->restore();
            painter->rotate((360.0/numCircles) * (blueList.at(1)) + 90);
            painter->drawConvexPolygon(minuteHand, 3);
            if(animationFrame >= ANIMATIONENDFRAME)
            {
                animationFrame = 0;
                animationState = (losingMove ? 4 : 3);
            }
        }
        else if (animationState == 3) //move circles to new position
        {
            int circleNum = 0;
            for (QPoint circle : circleGraph)
            {
                if(redList.contains(circleNum)) painter->setBrush(circleColorRed);
                else painter->setBrush(circleColorBlue);
                double degree = (circleNum > last) ? (2 * PI / (numCircles - 1)) * (circleNum - 1) : (2 * PI / (numCircles - 1)) * (circleNum);
                QPoint pos(circle.x() + ((cos(degree) * side / rad) - circle.x())*animationFrame/ANIMATIONENDFRAME,
                           circle.y() + ((sin(degree) * side / rad) - circle.y())*animationFrame/ANIMATIONENDFRAME);
                if (circleNum == last)
                {
                    circleNum++;
                    continue;
                }
                painter->drawEllipse(pos, radius, radius);
                circleNum++;
            }

            painter->setPen(textColor);
            painter->setFont(QFont("Times New Roman", 30));
            for (int i = 0 ; i < numCircles ; i++)
            {
                double degree = (i > last) ? (2 * PI / (numCircles - 1)) * (i - 1) : (2 * PI / (numCircles - 1)) * (i);
                QPoint pos(circleGraph.at(i).x() + ((cos(degree) * side / rad) - circleGraph.at(i).x())*animationFrame/ANIMATIONENDFRAME,
                           circleGraph.at(i).y() + ((sin(degree) * side / rad) - circleGraph.at(i).y())*animationFrame/ANIMATIONENDFRAME);
                if(i == last)
                    continue;
                painter->drawText(pos, QString::number(circleArray[i]));
            }
            painter->scale(side / 200.0, side / 200.0);

            painter->setPen(Qt::NoPen);
            painter->setBrush(ColorAnimate);

            painter->save();
            painter->rotate((360.0/numCircles) * (blueList.at(0)) + ((360.0/(numCircles - 1))*(blueList.at(0) + ((blueList.at(0) > last) ? -1 : 0))
                                                                    - (360.0/numCircles) * (blueList.at(0)))*animationFrame/ANIMATIONENDFRAME + 90);
            painter->drawConvexPolygon(minuteHand, 3);
            painter->restore();
            painter->rotate((360.0/numCircles) * (blueList.at(1)) + ((360.0/(numCircles - 1))*(blueList.at(1) + ((blueList.at(1) > last) ? -1 : 0))
                                                                    - (360.0/numCircles) * (blueList.at(1)))*animationFrame/ANIMATIONENDFRAME + 90);
            painter->drawConvexPolygon(minuteHand, 3);

            if(animationFrame >= ANIMATIONENDFRAME)
            {
                animationState = 4;
            }
        }
        //exit
        if(animationState == 4)
        {
            animationState = 0;
            if(mode == 1)
            {
                for(int i = 0; i < blueList.size(); ++i)
                    if(blueList.at(i) > last)
                        blueList[i] = blueList[i] - 1;
                numCircles--;
                redList.clear();
                for(int i = 0; i < numCircles; ++i)
                {
                    if(!blueList.contains(i)) redList << i;
                }
                int * temp = new int[numCircles];
                for(int i = 0; i < numCircles; ++i)
                {
                    temp[i] = circleArray[i < last ? i : i + 1];
                }
                delete circleArray;
                circleArray = temp;
                lose = losingMove;
                timeRemaining = TIMERESET;
            }
            else
            {
                liveArray.removeOne(last);
                timeRemaining = 2*TIMERESET;
            }
            animationActive = false;
            gameTimerActive = true;
        }
    }
    else
    {
        // Clock
        if (!startState)
        {
            QColor secondColor(127, 0, 127);
            QColor minuteColor(0, 127, 127, 191);

            painter->scale(side / 200.0, side / 200.0);

            painter->setPen(Qt::NoPen);
            painter->setBrush(secondColor);

            painter->save();
            if(mode == 1)
                painter->rotate((360.0/TIMERESET) * (TIMERESET - timeRemaining));
            else
                painter->rotate((360.0/(TIMERESET*2)) * (2*TIMERESET - timeRemaining));
            painter->drawConvexPolygon(secondHand, 3);
            painter->restore();

            painter->setPen(secondColor);

            for (int i = 0; i < 12; ++i)
            {
                painter->drawLine(88, 0, 96, 0);
                painter->rotate(30.0);
            }

            painter->setPen(Qt::NoPen);
            painter->setBrush(minuteColor);
            if(mode == 1)
            {
                painter->save();
                painter->rotate((360.0/numCircles) * blueList.at(0) + 90);
                painter->drawConvexPolygon(minuteHand, 3);
                painter->restore();
                painter->rotate((360.0/numCircles) * blueList.at(1) + 90);
                painter->drawConvexPolygon(minuteHand, 3);
            }
            else
            {
                if(liveArray.contains(neighbors.at(last).first))
                {
                    painter->save();
                    painter->rotate((360.0/numCircles) * neighbors.at(last).first + 90);
                    painter->drawConvexPolygon(minuteHand, 3);
                    painter->restore();
                }
                if(liveArray.contains(neighbors.at(last).second))
                {
                    painter->rotate((360.0/numCircles) * neighbors.at(last).second + 90);
                    painter->drawConvexPolygon(minuteHand, 3);
                }
            }
        }
    }
}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
    if(animationActive || lose || win || (timeRemaining <= 0 && gameTimerActive)) return;
    QPoint cursorPos(event->x() - width() / 2, event->y() - height() / 2);
    QList<QPoint> circleGraph;
    for (int i = 0; i < numCircles; ++i)
    {
        double degree = (2 * PI / numCircles) * i;
        QPoint circle = QPoint(cos(degree) * side / rad, sin(degree) * side / rad);
        circleGraph.append(circle);
    }
    for (int i = 0; i < numCircles; ++i)
    {
        if (inCircle(circleGraph.at(i), side/16, cursorPos))
        {
            if (mode == 1)
            {
                if (blueList.contains(i))
                {
                    if(numCircles == 1)
                    {
                        win = true;
                        gameTimerActive = false;
                        update();
                        return;
                    }
                    last = i;
                    lastValue = circleArray[i];
                    if(startState)
                    {
                        lastBlue[0] = last;
                        lastBlue[1] = last;
                    }
                    else
                    {
                        lastBlue[0] = blueList.at(0);
                        lastBlue[1] = blueList.at(1);
                    }
                    startState = false;
                    if ((last + lastValue) % (numCircles) == last) losingMove = true;
                    blueList.clear();
                    redList.clear();
                    blueList << ((last + lastValue) % numCircles);
                    blueList << ((last - lastValue + (numCircles)*MAXCIRCLES) % numCircles);
                    for(int i = 0; i < numCircles; ++i)
                    {
                        if(!blueList.contains(i)) redList << i;
                    }
                    gameTimerActive = false;
                    animationActive = true;
                    animationFrame = 0;
                    update();
                    break;
                }
            }
            else
            {
                if(startState || (liveArray.contains(i) && (i == neighbors.at(last).first || i == neighbors.at(last).second)))
                {
                    if(liveArray.size() == 1)
                    {
                        win = true;
                        gameTimerActive = false;
                        update();
                        return;
                    }
                    if(startState)
                    {
                        lastBlue[0] = i;
                        lastBlue[1] = i;
                    }
                    else
                    {
                        lastBlue[0] = neighbors.at(last).first;
                        lastBlue[1] = neighbors.at(last).second;
                    }
                    if(!liveArray.contains(neighbors.at(i).first) && !liveArray.contains(neighbors.at(i).second))
                        lose = true;
                    last = i;
                    lastValue = circleArray[i];
                    startState = false;
                    if(!liveArray.contains(neighbors.at(i).first) && !liveArray.contains(neighbors.at(i).second)) losingMove = true;
                    gameTimerActive = false;
                    animationActive = true;
                    animationFrame = 0;
                    update();
                    break;
                }
            }
        }
    }
}

bool GameWidget::inCircle(QPoint center, int radius, QPoint cursorPos)
{
    // simplification using square bounding box
    return cursorPos.x() < (center.x() + radius) && cursorPos.x() > (center.x() - radius) && cursorPos.y() < (center.y() + radius) && cursorPos.y() > (center.y() - radius);
}

void GameWidget::resetClicked()
{
    lose = false;
    losingMove = false;
    win = false;
    if(mode == 1)
    {
        numCircles = MAXCIRCLES;
        delete circleArray;
        circleArray = new int[numCircles];
        for(int i = 0; i < numCircles; ++i)
            circleArray[i] = circleCache[i];
        gameTimerActive = false;
        blueList.clear();
        redList.clear();
        timeRemaining = 0;
    }
    else
    {
        liveArray.clear();
        for(int i = 0; i < MAXCIRCLES; ++i)
            liveArray.append(i);
        gameTimerActive = true;
        timeRemaining = 2 * TIMERESET;
    }
    startState = true;
    animationState = 0;
    animationActive = false;
    update();
}

void GameWidget::newClicked()
{
    lose = false;
    win = false;
    losingMove = false;
    if(mode == 1)
    {
        numCircles = MAXCIRCLES;
        delete circleArray;
        circleArray = generatePuzzle2();
        for(int i = 0; i < numCircles; ++i)
            circleCache[i] = circleArray[i];
        blueList.clear();
        redList.clear();
        timeRemaining = 0;
    }
    else
    {
        delete circleArray;
        circleArray = generatePuzzle();
        liveArray.clear();
        neighbors.clear();
        for(int i = 0; i < MAXCIRCLES; ++i)
            liveArray.append(i);
        for(int i = 0; i < MAXCIRCLES; ++i)
            neighbors.append(QPair<int, int>((i+circleArray[i])%MAXCIRCLES, ((i+MAXCIRCLES-circleArray[i])%MAXCIRCLES)));
        timeRemaining = 2 * TIMERESET;
    }
    gameTimerActive = false;
    startState = true;
    animationState = 0;
    animationActive = false;
    update();
}
