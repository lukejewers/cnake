#include "raylib.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define CELL_SIZE 50
#define MAX_SNAKE_LENGTH 100
#define MOVE_INTERVAL 0.1f

#define BG_EARTHY CLITERAL(Color){ 45, 35, 25, 255 }
#define GRID_EARTHY_LIGHT CLITERAL(Color){ 60, 45, 35, 255 }

typedef struct {
    int x;
    int y;
} Segment;

typedef struct {
    Segment segments[MAX_SNAKE_LENGTH];
    int length;
} Snake;

Snake snake = {0};
float move_timer = 0;
int direction = KEY_RIGHT;
int next_direction = KEY_RIGHT;

void InitSnake() {
    snake.length = 2;
    // initialise the head
    snake.segments[0].x = 0;
    snake.segments[0].y = 250;
    // initialise the tail
    for (int i = 1; i < snake.length; ++i) {
        snake.segments[i].x = snake.segments[0].x - (i * CELL_SIZE);
        snake.segments[i].y = snake.segments[0].y;
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
        Segment old_head = snake.segments[0];
        direction = next_direction;

        switch (direction) {
            case KEY_UP:    snake.segments[0].y -= CELL_SIZE; break;
            case KEY_DOWN:  snake.segments[0].y += CELL_SIZE; break;
            case KEY_RIGHT: snake.segments[0].x += CELL_SIZE; break;
            case KEY_LEFT:  snake.segments[0].x -= CELL_SIZE; break;
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

int main(void) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "cnake");
    SetTargetFPS(60);

    InitSnake();

    while (!WindowShouldClose()) {
        // UPDATE
        UpdateSnake();

        // DRAW
        BeginDrawing();
        DrawBackground();
        DrawSnake();
        EndDrawing();
    }
    return 0;
}
