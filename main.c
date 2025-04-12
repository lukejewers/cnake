#include <raylib.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define BG_WIDTH 50

#define MY_BLACK CLITERAL(Color){24, 24, 24, 255}

void DrawBackground() {
  ClearBackground(MY_BLACK);
  for (int x = 0; x < WIN_WIDTH; x += (BG_WIDTH * 2))
    for (int y = 0; y < WIN_HEIGHT; y += (BG_WIDTH * 2)) {
      DrawRectangle(x + BG_WIDTH, y + BG_WIDTH, BG_WIDTH, BG_WIDTH, DARKGREEN);
      DrawRectangle(x, y, BG_WIDTH, BG_WIDTH, DARKGREEN);
    }
}

int main(void) {
  InitWindow(WIN_WIDTH, WIN_HEIGHT, "cnake");
  while (!WindowShouldClose()) {
    BeginDrawing();
    DrawBackground();
    EndDrawing();
  }
  return 0;
}
