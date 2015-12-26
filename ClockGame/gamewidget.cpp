#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "random.h"
#include <QTime>
#include <QList>
#include <math.h>
#define PI 3.14159265358979
#define MAXCIRCLES 12
#define TIMERESET 12
#define ANIMATIONENDFRAME 60.0
#define ANIMATIONDELAY 1000/ANIMATIONENDFRAME
#define DEV 1
#ifdef DEV
const QString RESOURCEPATH = "../../GameCPP/ClockGame/Resources/";
#else
const QString RESOURCEPATH = "./";
#endif
GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    pauseLayout = new QVBoxLayout;
    topLayout = new QHBoxLayout;
    auxLayout = new QVBoxLayout;
    QPushButton *start = new QPushButton("Generate");
    QPushButton *options = new QPushButton("Options");
    connect(options, &QPushButton::clicked, [this](){
        if(!paused)
        {
            paused = true;
            gameTimer->stop();
            animationTimer->stop();
            QPushButton *help = new QPushButton("Help");
            QPushButton *animationSpeed = new QPushButton("Animation Speed");
            QPushButton *difficulty = new QPushButton("Difficulty");
            QPushButton *sound = new QPushButton("Sound");
            QPushButton *credits = new QPushButton("Credits");
            connect(help, &QPushButton::clicked, [](){});
            connect(animationSpeed, &QPushButton::clicked, [](){});
            connect(difficulty, &QPushButton::clicked, [](){});
            connect(sound, &QPushButton::clicked, [this](){
                static bool statesound = false;
                if(!statesound)
                {
                    QPushButton *volumeup = new QPushButton();
                    QPushButton *volumedown = new QPushButton();
                    QPushButton *mute = new QPushButton();
                    QPixmap pixmapup(RESOURCEPATH + "Icons/ic_volume_up_black_24dp_1x.png");
                    QIcon ButtonIconup(pixmapup);
                    volumeup->setIcon(ButtonIconup);
                    volumeup->setIconSize(pixmapup.rect().size());
                    QPixmap pixmapdown(RESOURCEPATH + "Icons/ic_volume_down_black_24dp_1x.png");
                    QIcon ButtonIcondown(pixmapdown);
                    volumedown->setIcon(ButtonIcondown);
                    volumedown->setIconSize(pixmapdown.rect().size());
                    QPixmap pixmapoff(RESOURCEPATH + "Icons/ic_volume_off_black_24dp_1x.png");
                    QIcon ButtonIconoff(pixmapoff);
                    mute->setIcon(ButtonIconoff);
                    mute->setIconSize(pixmapoff.rect().size());
                    connect(volumeup, &QPushButton::clicked, [this](){
                        player->setVolume(player->volume() + 2);
                    });
                    connect(volumedown, &QPushButton::clicked, [this](){
                        player->setVolume(player->volume() - 2);
                    });
                    connect(mute, &QPushButton::clicked, [this](){
                        static bool statemute = false;
                        if(!statemute)
                        {
                            player->stop();
                            statemute = true;
                        }
                        else
                        {
                            player->play();
                            statemute = false;
                        }
                    });
                    auxLayout->addWidget(volumeup, 0, Qt::AlignmentFlag::AlignTop);
                    auxLayout->addWidget(volumedown, 0, Qt::AlignmentFlag::AlignTop);
                    auxLayout->addWidget(mute, 0, Qt::AlignmentFlag::AlignTop);
                    statesound = true;
                }
                else
                {
                    while (QLayoutItem *menuItem = auxLayout->takeAt(1))
                    {
                        delete menuItem->widget();
                        delete menuItem;
                    }
                    statesound = false;
                }
            });
            connect(credits, &QPushButton::clicked, [](){});
            pauseLayout->addWidget(help, 0, Qt::AlignmentFlag::AlignTop);
            pauseLayout->addWidget(animationSpeed, 0, Qt::AlignmentFlag::AlignTop);
            pauseLayout->addWidget(difficulty, 0, Qt::AlignmentFlag::AlignTop);
            pauseLayout->addWidget(sound, 0, Qt::AlignmentFlag::AlignTop);
            pauseLayout->addWidget(credits, 0, Qt::AlignmentFlag::AlignTop);
        }
        else
        {
            if(animationActive)
                animationTimer->start();
            if(gameStarted == true)
                gameTimer->start();
            paused = false;
            while (QLayoutItem *menuItem = pauseLayout->takeAt(1))
            {
                delete menuItem->widget();
                delete menuItem;
            }
        }

    });
    connect(start, &QPushButton::clicked, [this](){
        if(gameOver)
        {
            lose = false;
            win = false;
            numCircles = MAXCIRCLES;
            delete circleArray;
            circleArray = generatePuzzle2();
            startState = true;
            timeRemaining = 0;
            animationActive = false;
            paused = false;
            gameStarted = false;
            gameOver = false;
            gameTimer->stop();
            blueList.clear();
            redList.clear();
            update();
        }
    });
    auxLayout->addWidget(start, 0, Qt::AlignmentFlag::AlignTop);
    pauseLayout->addWidget(options, 0, Qt::AlignmentFlag::AlignTop);
    topLayout->addLayout(auxLayout);
    topLayout->addLayout(pauseLayout);
    setLayout(topLayout);
    lose = false;
    win = false;
    numCircles = MAXCIRCLES;
    QTimer *timer = new QTimer(this);
    gameTimer = new QTimer(this);
    animationTimer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(gameTimer, &QTimer::timeout, [this](){timeRemaining--;});
    connect(animationTimer, &QTimer::timeout, [this](){animationFrame++;update();});
    timer->start(1000);
    circleArray = generatePuzzle2();
    setWindowTitle(tr("Game"));
    resize(800, 800);
    side = qMin(width(), height());
    startState = true;
    timeRemaining = 0;
    animationActive = false;
    paused = false;
    gameStarted = false;
    gameOver = false;
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(RESOURCEPATH + "Sounds/theme.mp3"));
    player->setVolume(50);
    player->play();
    update();
}

GameWidget::~GameWidget()
{
    delete gameTimer;
    delete animationTimer;
    delete circleArray;
    delete ui;
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

void GameWidget::paintEvent(QPaintEvent *)
{
    static int animationState = 0;
    static const QPoint minuteHand[3] =
    {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };
    static const QPoint secondHand[3] =
    {
        QPoint(3, 8),
        QPoint(-3, 8),
        QPoint(0, -70)
    };
    QPainter painter(this);
    painter.setOpacity(paused ? .5 : 1.0);
    if (startState)
    {
        for(int i = 0; i < numCircles; ++i)
            blueList << i;
    }
    int radius = side / 16;
    QColor circleColorBlue(Qt::GlobalColor::blue);
    QColor circleColorRed(Qt::GlobalColor::red);
    QColor circleColorGreen(Qt::GlobalColor::green);
    QColor textColor(Qt::GlobalColor::black);
    QList<QPoint> circleGraph;
    for (int i = 0; i < numCircles; ++i)
    {
        double degree = (2 * PI / numCircles) * i;
        QPoint circle = QPoint(cos(degree) * side / 4, sin(degree) * side / 4);
        circleGraph.append(circle);
    }
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    if (lose || (timeRemaining <= 0 && gameTimer->isActive()))
    {
        painter.setPen(textColor);
        painter.setFont(QFont("Times New Roman", 60));
        painter.drawText(-width() / 4, 0, "YOU LOSE!");
        gameOver = true;
        return;
    }
    if (win)
    {
        painter.setPen(textColor);
        painter.setFont(QFont("Times New Roman", 60));
        painter.drawText(-width() / 4, 0, "YOU WIN!");
        gameOver = true;
        return;
    }
    if(!animationActive || (animationState != 2 && animationState != 3))
    {
        int circleNum = 0;
        for (QPoint circle : circleGraph)
        {
            if(redList.contains(circleNum)) painter.setBrush(circleColorRed);
            else painter.setBrush(circleColorBlue);
            if (animationActive == 1 && circleNum == last) painter.setBrush(circleColorGreen);
            painter.drawEllipse(circle, radius, radius);
            circleNum++;
        }

        painter.setPen(textColor);
        painter.setFont(QFont("Times New Roman", 30));
        for (int i = 0 ; i < numCircles ; i++)
        {
            painter.drawText(circleGraph.at(i), QString::number(circleArray[i]));
        }
    }
    if(animationActive)
    {
        if (animationState == 0) //move hands to last
        {
            QColor ColorAnimate(Qt::GlobalColor::green);
            painter.scale(side / 200.0, side / 200.0);

            painter.setPen(Qt::NoPen);
            painter.setBrush(ColorAnimate);

            painter.save();
            painter.rotate((360.0/numCircles) * (lastBlue[0] + (last - lastBlue[0])*animationFrame/ANIMATIONENDFRAME) + 90);
            painter.drawConvexPolygon(minuteHand, 3);
            painter.restore();
            if(blueList.size() > 1)
            {
                painter.rotate((360.0/numCircles) * (lastBlue[1] + (last - lastBlue[1])*animationFrame/ANIMATIONENDFRAME) + 90);
                painter.drawConvexPolygon(minuteHand, 3);
            }
            else
            {
                painter.rotate((360.0/numCircles) * (lastBlue[1] + (last - lastBlue[1])*animationFrame/ANIMATIONENDFRAME) + 90);
                painter.drawConvexPolygon(minuteHand, 3);
            }
            if(animationFrame == ANIMATIONENDFRAME)
            {
                animationFrame = 0;
                animationState = 1;
            }
        }
        else if (animationState == 1) //move hands to next blue
        {
            QColor ColorAnimate(Qt::GlobalColor::green);
            painter.scale(side / 200.0, side / 200.0);

            painter.setPen(Qt::NoPen);
            painter.setBrush(ColorAnimate);
            painter.save();
            painter.rotate((360.0/numCircles) * (last + (blueList.at(0) - last)*animationFrame/ANIMATIONENDFRAME) + 90);
            painter.drawConvexPolygon(minuteHand, 3);
            painter.restore();
            painter.rotate((360.0/numCircles) * (last + (blueList.at(1) - last)*animationFrame/ANIMATIONENDFRAME) + 90);
            painter.drawConvexPolygon(minuteHand, 3);

            if(animationFrame == ANIMATIONENDFRAME)
            {
                animationFrame = 0;
                animationState = 2;
            }
        }
        else if (animationState == 2) //move old last out
        {
            int circleNum = 0;
            for (QPoint circle : circleGraph)
            {
                if(redList.contains(circleNum)) painter.setBrush(circleColorRed);
                else painter.setBrush(circleColorBlue);
                if (circleNum == last)
                {
                    painter.save();
                    painter.setBrush(circleColorGreen);
                    painter.setOpacity((ANIMATIONENDFRAME-animationFrame)/ANIMATIONENDFRAME);
                    painter.drawEllipse(circle, radius, radius);
                    circleNum++;
                    painter.restore();
                    continue;
                }
                painter.drawEllipse(circle, radius, radius);
                circleNum++;
            }

            painter.setPen(textColor);
            painter.setFont(QFont("Times New Roman", 30));
            for (int i = 0 ; i < numCircles ; i++)
            {
                if(i == last)
                {
                    painter.save();
                    painter.setOpacity((ANIMATIONENDFRAME-animationFrame)/ANIMATIONENDFRAME);
                    painter.drawText(circleGraph.at(i), QString::number(circleArray[i]));
                    painter.restore();
                    continue;
                }
                painter.drawText(circleGraph.at(i), QString::number(circleArray[i]));
            }
            QColor ColorAnimate(Qt::GlobalColor::green);
            painter.scale(side / 200.0, side / 200.0);

            painter.setPen(Qt::NoPen);
            painter.setBrush(ColorAnimate);

            painter.save();
            painter.rotate((360.0/numCircles) * (blueList.at(0)) + 90);
            painter.drawConvexPolygon(minuteHand, 3);
            painter.restore();
            painter.rotate((360.0/numCircles) * (blueList.at(1)) + 90);
            painter.drawConvexPolygon(minuteHand, 3);
            if(animationFrame == ANIMATIONENDFRAME)
            {
                animationFrame = 0;
                animationState = 3;
            }
        }
        else if (animationState == 3) //move circles to new position
        {
            int circleNum = 0;
            for (QPoint circle : circleGraph)
            {
                if(redList.contains(circleNum)) painter.setBrush(circleColorRed);
                else painter.setBrush(circleColorBlue);
                double degree = (circleNum > last) ? (2 * PI / (numCircles - 1)) * (circleNum - 1) : (2 * PI / (numCircles - 1)) * (circleNum);
                QPoint pos(circle.x() + ((cos(degree) * side / 4) - circle.x())*animationFrame/ANIMATIONENDFRAME,
                           circle.y() + ((sin(degree) * side / 4) - circle.y())*animationFrame/ANIMATIONENDFRAME);
                if (circleNum == last)
                {
                    circleNum++;
                    continue;
                }
                painter.drawEllipse(pos, radius, radius);
                circleNum++;
            }

            painter.setPen(textColor);
            painter.setFont(QFont("Times New Roman", 30));
            for (int i = 0 ; i < numCircles ; i++)
            {
                double degree = (i > last) ? (2 * PI / (numCircles - 1)) * (i - 1) : (2 * PI / (numCircles - 1)) * (i);
                QPoint pos(circleGraph.at(i).x() + ((cos(degree) * side / 4) - circleGraph.at(i).x())*animationFrame/ANIMATIONENDFRAME,
                           circleGraph.at(i).y() + ((sin(degree) * side / 4) - circleGraph.at(i).y())*animationFrame/ANIMATIONENDFRAME);
                if(i == last)
                    continue;
                painter.drawText(pos, QString::number(circleArray[i]));
            }
            QColor ColorAnimate(Qt::GlobalColor::green);
            painter.scale(side / 200.0, side / 200.0);

            painter.setPen(Qt::NoPen);
            painter.setBrush(ColorAnimate);

            painter.save();
            painter.rotate((360.0/numCircles) * (blueList.at(0)) + ((360.0/(numCircles - 1))*(blueList.at(0) + ((blueList.at(0) > last) ? -1 : 0))
                                                                    - (360.0/numCircles) * (blueList.at(0)))*animationFrame/ANIMATIONENDFRAME + 90);
            painter.drawConvexPolygon(minuteHand, 3);
            painter.restore();
            painter.rotate((360.0/numCircles) * (blueList.at(1)) + ((360.0/(numCircles - 1))*(blueList.at(1) + ((blueList.at(1) > last) ? -1 : 0))
                                                                    - (360.0/numCircles) * (blueList.at(1)))*animationFrame/ANIMATIONENDFRAME + 90);
            painter.drawConvexPolygon(minuteHand, 3);

            if(animationFrame == ANIMATIONENDFRAME)
            {
                animationState = 4;
            }
        }
        //exit
        if(animationFrame == ANIMATIONENDFRAME && animationState == 4)
        {
            animationState = 0;
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
            animationActive = false;
            animationTimer->stop();
            timeRemaining = TIMERESET;
            gameTimer->start(1000);
            gameStarted = true;
        }
    }
    else
    {
        // Clock
        if (!startState)
        {
            QColor secondColor(127, 0, 127);
            QColor minuteColor(0, 127, 127, 191);

            painter.scale(side / 200.0, side / 200.0);

            painter.setPen(Qt::NoPen);
            painter.setBrush(secondColor);

            painter.save();
            painter.rotate((360.0/TIMERESET) * (TIMERESET - timeRemaining));
            painter.drawConvexPolygon(secondHand, 3);
            painter.restore();

            painter.setPen(secondColor);

            for (int i = 0; i < 12; ++i)
            {
                painter.drawLine(88, 0, 96, 0);
                painter.rotate(30.0);
            }

            painter.setPen(Qt::NoPen);
            painter.setBrush(minuteColor);

            painter.save();
            painter.rotate((360.0/numCircles) * blueList.at(0) + 90);
            painter.drawConvexPolygon(minuteHand, 3);
            painter.restore();
            painter.rotate((360.0/numCircles) * blueList.at(1) + 90);
            painter.drawConvexPolygon(minuteHand, 3);
        }
    }
}

void GameWidget::mousePressEvent(QMouseEvent *event)
{
    if(animationActive || paused) return;
    QPoint cursorPos(event->x() - width() / 2, event->y() - height() / 2);
    QList<QPoint> circleGraph;
    for (int i = 0; i < numCircles; ++i)
    {
        double degree = (2 * PI / numCircles) * i;
        QPoint circle = QPoint(cos(degree) * side / 4, sin(degree) * side / 4);
        circleGraph.append(circle);
    }
    for (int i = 0; i < numCircles; ++i)
    {
        if (inCircle(circleGraph.at(i), side/16, cursorPos))
        {
            if (blueList.contains(i))
            {
                if(numCircles == 1)
                {
                    win = true;
                    repaint();
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
                if ((last + lastValue) % (numCircles) == last)
                {
                    lose = true;
                    qDebug() << "Lose Condition";
                    repaint();
                    return;
                }
                blueList.clear();
                redList.clear();
                blueList << ((last + lastValue) % numCircles);
                blueList << ((last - lastValue + (numCircles)*MAXCIRCLES) % numCircles);
                for(int i = 0; i < numCircles; ++i)
                {
                    if(!blueList.contains(i)) redList << i;
                }
                if(gameTimer->isActive()) gameTimer->stop();
                animationActive = true;
                animationFrame = 0;
                animationTimer->start(ANIMATIONDELAY);
                repaint();
                break;
            }
            else qDebug() << "Invalid";
        }
    }
}

bool GameWidget::inCircle(QPoint center, int radius, QPoint cursorPos)
{
    // simplification using square bounding box
    return cursorPos.x() < (center.x() + radius) && cursorPos.x() > (center.x() - radius) && cursorPos.y() < (center.y() + radius) && cursorPos.y() > (center.y() - radius);
}
