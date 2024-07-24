#include <raylib.h>
#include <stdio.h>

const int sq_width = 50;
const int sq_height = 50;
const int w_width = 800;
const int w_height = 800;

void DrawRows() {
    int curr = 0;
    for(int i = 0; i < w_height; i += sq_height) {
        DrawLine(0, i, w_width, i, WHITE);
    }
}

void DrawCols() {
    int curr = 0;
    for(int i = 0; i < w_height; i += sq_width) {
        DrawLine(i, 0, i, w_height, WHITE);
    }
}

int main(void) {
    SetTargetFPS(30);

    InitWindow(w_width, w_height, "Cellular Automata");
    while(!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
         {
             DrawRows();
             DrawCols();
         }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
