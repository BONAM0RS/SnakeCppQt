
#include <QDebug>
#include <QTime>
#include <QRandomGenerator>
#include <QPainter>
#include <QKeyEvent>

#include "game.h"


Game::Game(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color:black;");
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    initGame();
    qDebug() << "Game created";
}


Game::~Game()
{
    qDebug() << "Game deleted";
}


void Game::initGame()
{
    inGame = true;
    inReplay = false;
    snake_dir = right;
    snake_dots = 3;

    spawnSnake();
    spawnApple();

    timerSec = 4;
    timerId = startTimer(TICK_DELAY);
}


void Game::spawnSnake()
{
    for (int i = 0; i < snake_dots; ++i) {
        snake_x[i] = 0;
        snake_y[i] = 0;
    }

    qDebug() << "Snake spawned";
}


void Game::spawnApple()
{
    QTime time = QTime::currentTime();
    QRandomGenerator ((uint) time.msec());

    apple_x = QRandomGenerator::global()->bounded(RANGE_DOTS_X) * DOT_SIZE;
    apple_y = QRandomGenerator::global()->bounded(RANGE_DOTS_Y) * DOT_SIZE;

    qDebug() << "Apple spawned at pos (x, y) =" << apple_x << "," << apple_y;
}


void Game::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);

    if (inGame)
    {
        moveSnake();
        checkApple();
        checkCollision();
    }
    else
    {
        timerSec--;
        qDebug() << "The game will restart in ..." << timerSec;
    }

    repaint();
}


void Game::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    doDrawing();
}


void Game::doDrawing()
{
    QPainter qp(this);

    if (inGame)
    {
        qp.setBrush(Qt::red);
        qp.drawEllipse(apple_x, apple_y, DOT_SIZE, DOT_SIZE);

        for (int i = 0; i < snake_dots; ++i)
        {
            if (i == 0)
            {
                qp.setBrush(Qt::white);
                qp.drawEllipse(snake_x[i], snake_y[i], DOT_SIZE, DOT_SIZE);
            }
            else
            {
                qp.setBrush(Qt::green);
                qp.drawEllipse(snake_x[i], snake_y[i], DOT_SIZE, DOT_SIZE);
            }
        }
    }
    else
    {
        gameOver(qp);
    }
}


void Game::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();

    if (key == Qt::Key_Left  && snake_dir != right) { snake_dir = left;  }
    if (key == Qt::Key_Right && snake_dir != left ) { snake_dir = right; }
    if (key == Qt::Key_Up    && snake_dir != down ) { snake_dir = upper; }
    if (key == Qt::Key_Down  && snake_dir != upper) { snake_dir = down;  }

    QWidget::keyPressEvent(e);
}


void Game::moveSnake()
{
    for (int i = snake_dots; i > 0; --i)
    {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    switch (snake_dir)
    {
        case left:  { snake_x[0] -= DOT_SIZE; break; }
        case right: { snake_x[0] += DOT_SIZE; break; }
        case upper: { snake_y[0] -= DOT_SIZE; break; }
        case down:  { snake_y[0] += DOT_SIZE; break; }
    }

    // qDebug() << "Snake head pos (x, y) =" << snake_x[0] << "," << snake_y[0];
}


void Game::checkApple()
{
    if (snake_x[0] == apple_x && snake_y[0] == apple_y)
    {
        snake_dots++;
        spawnApple();
    }

    if (snake_dots > ALL_DOTS / 2)
    {
        inGame = false;
    }
}


void Game::checkCollision()
{
    if (snake_dots > 4)
    {
        for (int i = 1; i < snake_dots; ++i)
        {
            if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
            {
                inGame = false;
            }
        }
    }

    if (snake_x[0] >= WINDOW_WIDTH ) { inGame = false; }
    if (snake_y[0] >= WINDOW_HEIGHT) { inGame = false; }

    if (snake_x[0] < 0) { inGame = false; }
    if (snake_y[0] < 0) { inGame = false; }

    if (!inGame && !inReplay)
    {
        killTimer(timerId);
    }
}


void Game::gameOver(QPainter &qp)
{
    QString message;

    if (snake_dots < ALL_DOTS / 2)
    {
        message = "You lose";
    }
    else
    {
        message = "You win";
    }

    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.QFontMetrics::horizontalAdvance(message);

    qp.setPen(QColor(Qt::white));
    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, message);

    if (!inReplay)
    {
        timerId = startTimer(1000);
        inReplay = true;
    }

    if (timerSec <= 1)
    {
        killTimer(timerId);
        initGame();
    }
}

