#include <raylib.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define MY_BLACK CLITERAL(Color){ 24, 24, 24, 255 }

int main(void) {

  InitWindow(WIN_WIDTH, WIN_HEIGHT, "cnake");
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(MY_BLACK);
    EndDrawing();
  }
  return 0;
}
