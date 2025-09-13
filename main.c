#include "raylib.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define BG_WIDTH 50
#define MOVE_INTERVAL 0.1f

#define MY_BLACK CLITERAL(Color){24, 24, 24, 255}

typedef struct {
    int pos_x;
    int pos_y;
} Snake;

Snake snake = {
    .pos_x = 100,
    .pos_y = 250,
};
float move_timer = 0;
int direction = KEY_RIGHT;

void UpdateSnake() {
    move_timer += GetFrameTime();

    if (IsKeyDown(KEY_UP)) direction = KEY_UP;
    if (IsKeyDown(KEY_DOWN)) direction = KEY_DOWN;
    if (IsKeyDown(KEY_LEFT)) direction = KEY_LEFT;
    if (IsKeyDown(KEY_RIGHT)) direction = KEY_RIGHT;

    if (move_timer >= MOVE_INTERVAL) {
        move_timer = 0;

        switch (direction) {
            case KEY_UP:
                snake.pos_y -= BG_WIDTH;
                break;
            case KEY_DOWN:
                snake.pos_y += BG_WIDTH;
                break;
            case KEY_RIGHT:
                snake.pos_x += BG_WIDTH;
                break;
            case KEY_LEFT:
                snake.pos_x -= BG_WIDTH;
                break;
            default:
                snake.pos_x += BG_WIDTH;
        }

        if (snake.pos_x >= WIN_WIDTH) snake.pos_x = 0;
        if (snake.pos_x < 0) snake.pos_x = WIN_WIDTH - BG_WIDTH;
        if (snake.pos_y >= WIN_HEIGHT) snake.pos_y = 0;
        if (snake.pos_y < 0) snake.pos_y = WIN_HEIGHT - BG_WIDTH;
    }
}

void DrawBackground() {
    ClearBackground(MY_BLACK);
    for (int x = 0; x < WIN_WIDTH; x += (BG_WIDTH * 2))
        for (int y = 0; y < WIN_HEIGHT; y += (BG_WIDTH * 2)) {
            DrawRectangle(x + BG_WIDTH, y + BG_WIDTH, BG_WIDTH, BG_WIDTH, DARKGREEN);
            DrawRectangle(x, y, BG_WIDTH, BG_WIDTH, DARKGREEN);
        }
}

void DrawSnake() {
    DrawRectangle(snake.pos_x, snake.pos_y, BG_WIDTH, BG_WIDTH, YELLOW);
}

int main(void) {
    InitWindow(WIN_WIDTH, WIN_HEIGHT, "cnake");
    SetTargetFPS(60);

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
