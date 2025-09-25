#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define CELL_SIZE 50
#define MAX_SNAKE_LENGTH 100
#define MOVE_INTERVAL 0.1f
#define FONT_SIZE 20
#define SNAKE_HEAD_COLOR DARKGREEN
#define SNAKE_BODY_COLOR GREEN
#define APPLE_COLOR RED
#define TEXT_COLOR WHITE
#define BG_EARTHY CLITERAL(Color){ 45, 35, 25, 255 }
#define GRID_EARTHY_LIGHT CLITERAL(Color){ 60, 45, 35, 255 }

typedef enum {
    PLAY,
    PAUSE,
    GAME_OVER
} GameState;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position segments[MAX_SNAKE_LENGTH];
    int length;
} Snake;

typedef struct {
    Position position;
    int eaten;
} Apple;

Snake snake = {0};
Apple apple = {0};
float move_timer = 0;
GameState game_state = PLAY;
KeyboardKey direction = KEY_RIGHT;
KeyboardKey next_direction = KEY_RIGHT;

void InitSnake() {
    snake.length = 2;
    snake.segments[0].x = 300;
    snake.segments[0].y = 250;
    for (int i = 1; i < snake.length; ++i) {
        snake.segments[i].x = snake.segments[0].x - (i * CELL_SIZE);
        snake.segments[i].y = snake.segments[0].y;
    }
}

void InitApple() {
    apple.eaten = 0;
    apple.position.x = 500;
    apple.position.y = 250;
}

void UpdateApple() {
    if (game_state != PLAY) return;
    if (apple.position.x == snake.segments[0].x &&
        apple.position.y == snake.segments[0].y) {

        Position new_pos;
        bool on_snake;
        do {
            on_snake = false;
            new_pos.x = (rand() % (WIN_WIDTH / CELL_SIZE)) * CELL_SIZE;
            new_pos.y = (rand() % (WIN_HEIGHT / CELL_SIZE)) * CELL_SIZE;

            for (int i = 0; i < snake.length; ++i) {
                if (new_pos.x == snake.segments[i].x && new_pos.y == snake.segments[i].y) {
                    on_snake = true;
                    break;
                }
            }
        } while (on_snake);

        apple.position = new_pos;

        if (snake.length < MAX_SNAKE_LENGTH) {
            snake.segments[snake.length] = snake.segments[snake.length - 1];
            snake.length++;
        }
        apple.eaten++;
    }
}

void UpdateSnake() {
    if (game_state != PLAY) return;
    move_timer += GetFrameTime();

    if (IsKeyPressed(KEY_UP) && direction != KEY_DOWN) next_direction = KEY_UP;
    if (IsKeyPressed(KEY_DOWN) && direction != KEY_UP) next_direction = KEY_DOWN;
    if (IsKeyPressed(KEY_LEFT) && direction != KEY_RIGHT) next_direction = KEY_LEFT;
    if (IsKeyPressed(KEY_RIGHT) && direction != KEY_LEFT) next_direction = KEY_RIGHT;

    if (move_timer >= MOVE_INTERVAL) {
        move_timer = 0;
        Position old_head = snake.segments[0];
        direction = next_direction;

        switch (direction) {
            case KEY_UP:    snake.segments[0].y -= CELL_SIZE; break;
            case KEY_DOWN:  snake.segments[0].y += CELL_SIZE; break;
            case KEY_RIGHT: snake.segments[0].x += CELL_SIZE; break;
            case KEY_LEFT:  snake.segments[0].x -= CELL_SIZE; break;
            default: break;
        }

        for (int i = 1; i < snake.length; ++i) {
            if (snake.segments[0].x == snake.segments[i].x &&
                snake.segments[0].y == snake.segments[i].y) {
                game_state = GAME_OVER;
                break;
            }
        }

        if (snake.segments[0].x >= WIN_WIDTH) snake.segments[0].x = 0;
        if (snake.segments[0].x < 0) snake.segments[0].x = WIN_WIDTH - CELL_SIZE;
        if (snake.segments[0].y >= WIN_HEIGHT) snake.segments[0].y = 0;
        if (snake.segments[0].y < 0) snake.segments[0].y = WIN_HEIGHT - CELL_SIZE;

        for (int i = snake.length - 1; i > 0; i--) {
            snake.segments[i] = snake.segments[i - 1];
        }
        if (snake.length > 1) snake.segments[1] = old_head;
    }
}

void DrawBackground() {
    ClearBackground(BG_EARTHY);

    for (int x = 0; x < WIN_WIDTH; x += CELL_SIZE) {
        DrawLine(x, 0, x, WIN_HEIGHT, GRID_EARTHY_LIGHT);
    }
    for (int y = 0; y < WIN_HEIGHT; y += CELL_SIZE) {
        DrawLine(0, y, WIN_WIDTH, y, GRID_EARTHY_LIGHT);
    }
}

void DrawSnake() {
    for (int i = 0; i < snake.length; ++i) {
        DrawRectangle(snake.segments[i].x, snake.segments[i].y, CELL_SIZE, CELL_SIZE, i == 0 ? SNAKE_HEAD_COLOR : SNAKE_BODY_COLOR);
    }
}

void DrawApple() {
    DrawRectangle(apple.position.x, apple.position.y, CELL_SIZE, CELL_SIZE, APPLE_COLOR);
}

void DrawScore() {
    char score[50];
    snprintf(score, sizeof(score), "SCORE: %d", apple.eaten);
    int textWidth = MeasureText(score, FONT_SIZE);
    DrawText(score, WIN_WIDTH - textWidth - CELL_SIZE, CELL_SIZE, FONT_SIZE, TEXT_COLOR);
}

void DrawState() {
    if (game_state == PLAY) return;
    const char *game_state_text = (game_state == PAUSE) ? "PAUSE" : "GAME OVER";
    int state_text_width = MeasureText(game_state_text, FONT_SIZE);
    DrawText(game_state_text, (WIN_WIDTH/2) - (state_text_width/2), (WIN_HEIGHT/2) - (FONT_SIZE/2), FONT_SIZE, TEXT_COLOR);

    const char *game_replay_text = (game_state == PAUSE) ? "Press SPACE or ARROWS to play" : "Press SPACE to replay";
    int replay_text_width = MeasureText(game_replay_text, FONT_SIZE);
    DrawText(game_replay_text, (WIN_WIDTH/2) - (replay_text_width/2), (WIN_HEIGHT/2) + (FONT_SIZE/2) * 2, FONT_SIZE, TEXT_COLOR);
}

void ResetGame() {
    game_state = PLAY;
    direction = KEY_RIGHT;
    next_direction = KEY_RIGHT;
    InitApple();
    InitSnake();
}

void UpdateGameState() {
    if (game_state == PLAY && IsKeyPressed(KEY_SPACE)) {
        game_state = PAUSE;
    } else if (game_state == PAUSE     &&
               (IsKeyPressed(KEY_SPACE) ||
                IsKeyPressed(KEY_UP)    ||
                IsKeyPressed(KEY_DOWN)  ||
                IsKeyPressed(KEY_LEFT)  ||
                IsKeyPressed(KEY_RIGHT))) {
        game_state = PLAY;
    } else if (game_state == GAME_OVER &&
               IsKeyPressed(KEY_SPACE)) {
        ResetGame();
    }
}

int main(void) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "cnake");
    SetTargetFPS(60);
    srand(time(NULL));

    InitApple();
    InitSnake();

    while (!WindowShouldClose()) {
        // UPDATE
        UpdateGameState();
        UpdateApple();
        UpdateSnake();
        // DRAW
        BeginDrawing();
        DrawBackground();
        DrawApple();
        DrawSnake();
        DrawScore();
        DrawState();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
