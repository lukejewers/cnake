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
    // initialise the head
    snake.segments[0].x = 300;
    snake.segments[0].y = 250;
    // initialise the tail
    for (int i = 1; i < snake.length; ++i) {
        snake.segments[i].x = snake.segments[0].x - (i * CELL_SIZE);
        snake.segments[i].y = snake.segments[0].y;
    }
}

void InitApple() {
    apple.eaten = 0;
    // initialise the position
    apple.position.x = 500;
    apple.position.y = 250;
}

void UpdateApple() {
    if (apple.position.x == snake.segments[0].x &&
        apple.position.y == snake.segments[0].y) {
        apple.position.x = ((int)rand() % WIN_WIDTH/CELL_SIZE) * CELL_SIZE;
        apple.position.y = ((int)rand() % WIN_HEIGHT/CELL_SIZE) * CELL_SIZE;
        if (snake.length < MAX_SNAKE_LENGTH) {
            snake.segments[snake.length] = snake.segments[snake.length - 1];
            snake.length++;
        }
        apple.eaten++;
    }
}

void UpdateSnake() {
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
        DrawRectangle(snake.segments[i].x, snake.segments[i].y, CELL_SIZE, CELL_SIZE, i == 0 ? DARKGREEN : GREEN);
    }
}

void DrawApple() {
    DrawRectangle(apple.position.x, apple.position.y, CELL_SIZE, CELL_SIZE, RED);
}

void DrawScore() {
    char score[100];
    sprintf(score, "SCORE: %d", apple.eaten);
    int textWidth = MeasureText(score, FONT_SIZE);
    DrawText(score, WIN_WIDTH - textWidth - CELL_SIZE, CELL_SIZE - FONT_SIZE, FONT_SIZE, WHITE);
}

void DrawState() {
    char *game_state_text;
    if (game_state == PAUSE) game_state_text = "PAUSE";
    else if (game_state == PAUSE) game_state_text = "GAME OVER";
    int textWidth = MeasureText(game_state_text, FONT_SIZE);
    DrawText(game_state_text, (WIN_WIDTH/2) - (textWidth/2), (WIN_HEIGHT/2) - (FONT_SIZE/2), FONT_SIZE, WHITE);
}


int main(void) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "cnake");
    SetTargetFPS(60);
    srand(time(NULL));

    InitApple();
    InitSnake();

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE) && game_state == PLAY) {
            game_state = PAUSE;
        } else if (game_state == PAUSE     &&
                  (IsKeyPressed(KEY_SPACE) ||
                   IsKeyPressed(KEY_UP)    ||
                   IsKeyPressed(KEY_DOWN)  ||
                   IsKeyPressed(KEY_LEFT)  ||
                   IsKeyPressed(KEY_RIGHT))) {
            game_state = PLAY;
        }

        // UPDATE
        if (game_state == PLAY) {
            UpdateApple();
            UpdateSnake();
        }

        // DRAW
        BeginDrawing();
        DrawBackground();
        DrawApple();
        DrawSnake();
        DrawScore();
        if (game_state == PAUSE || game_state == GAME_OVER) DrawState();
        EndDrawing();
    }
    return 0;
}
