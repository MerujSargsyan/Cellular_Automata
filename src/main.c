#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int sq_width = 10;
const int sq_height = 10;
const int w_width = 800;
const int w_height = 800;
const int sq_count = (w_width / sq_width) * (w_height / sq_height);
const int rows = sq_count / (w_width / sq_width);
const int cols = sq_count / (w_height/ sq_height);

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
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(cells[r * cols + c]) {
                DrawRectangle(c * sq_width, r * sq_height, sq_width, sq_height, BLUE);
            }
        }
    }
}

int* InitGrid(int start_count) {
    int* cells = malloc(sizeof(int) * sq_count);
    for(int i = 0; i < sq_count; i++) {
        cells[i] = 0;
    }
    srand(time(0));
    for(int i = 0; i < start_count; i++) {
        int idx = rand() % sq_count;
        cells[idx] = 1;
    }
    return cells;
}

int main(void) {
    SetTargetFPS(30);

    int* cells = InitGrid(500);

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
    free(cells);
    return 0;
}
