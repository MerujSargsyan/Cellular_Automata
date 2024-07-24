#include <raylib.h>
#include <stdio.h>

const int sq_width = 50;
const int sq_height = 50;
const int w_width = 800;
const int w_height = 800;
const int sq_count = (w_width / sq_width) * (w_height / sq_height);

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

/*
 * 2D:
 * 1 0 0 0
 * 0 1 0 0
 * 0 0 1 0
 * 0 0 0 1
 *
 * 1D:
 * 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1
*/
void DrawCells(int* cells) {
    int rows = sq_count / (w_width / sq_width);
    int cols = sq_count / (w_height/ sq_height);
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(cells[r * cols + c]) {
                DrawRectangle(c * sq_width, r * sq_height, sq_width, sq_height, BLUE);
            }
        }
    }
}

int main(void) {
    SetTargetFPS(30);

    int cells[][16] = {
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0,},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,},
    };
    

    InitWindow(w_width, w_height, "Cellular Automata");
    while(!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
         {
             DrawRows();
             DrawCols();
             DrawCells((int*)cells);
         }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
