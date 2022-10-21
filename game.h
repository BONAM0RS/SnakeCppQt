
#ifndef GAME_H
#define GAME_H


#include <QWidget>


class Game : public QWidget
{

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

protected:
    void timerEvent(QTimerEvent*);
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent*);

private:
    void initGame();
    void spawnSnake();
    void spawnApple();
    void moveSnake();
    void checkApple();
    void checkCollision();
    void doDrawing();
    void gameOver(QPainter &);

private:
    static const int WINDOW_WIDTH = 400;
    static const int WINDOW_HEIGHT = 300;
    static const int DOT_SIZE = 10;
    static const int ALL_DOTS = (WINDOW_WIDTH * WINDOW_HEIGHT) / (DOT_SIZE * DOT_SIZE);
    static const int RANGE_DOTS_X = (WINDOW_WIDTH - DOT_SIZE) / DOT_SIZE;
    static const int RANGE_DOTS_Y = (WINDOW_HEIGHT - DOT_SIZE) / DOT_SIZE;
    static const int TICK_DELAY = 200;

    int timerId;
    int timerSec;

    int apple_x;
    int apple_y;

    int snake_dots;
    int snake_x[ALL_DOTS];
    int snake_y[ALL_DOTS];

    enum Directions
    {
        left,
        right,
        upper,
        down
    };

    Directions snake_dir;

    bool inGame;
    bool inReplay;
};


#endif // GAME_H

