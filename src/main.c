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

void draw_rows() {
    int curr = 0;
    for(int i = 0; i <=w_height; i += sq_height) {
        DrawLine(0, i, w_width, i, WHITE);
    }
}

void draw_cols() {
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
void draw_cells(int* cells) {
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(cells[r * rows + c]) {
                DrawRectangle(c * sq_width, r * sq_height, sq_width, sq_height, WHITE);
            } else {
                DrawRectangle(c * sq_width, r * sq_height, sq_width, sq_height, BLACK);
            }
        }
    }
}

int* init_grid(int start_count) {
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

int neighbor_count(int* cells, int r, int c) {
    int idx = r * rows + c;
    int count = 0;
    if(c > 0 && cells[idx-1]) count++;
    if(c < cols-1 && cells[idx+1]) count++;

    // check top row
    if(r > 0 && cells[idx-cols]) count++;
    if(r > 0 && c > 0 && cells[idx-cols-1]) count++;
    if(r > 0 && c < cols-1 && cells[idx-cols+1]) count++;

    // check bottom row
    if(r < rows-1 && cells[idx+cols]) count++;
    if(r < rows-1 && c > 0 && cells[idx+cols-1]) count++;
    if(r < rows-1 && c < cols-1 && cells[idx+cols+1]) count++;

    return count;
}

void apply_rules(int* cells) {
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            int n = neighbor_count(cells, r, c);
            int idx = r*rows+c;
            if(n > 3 || n < 2) {
                cells[idx] = 0;
            } else if(n == 3) {
                cells[idx] = 1;
            }
        }
    }
}

int main(void) {
    SetTargetFPS(10);

    int* cells = init_grid(500);
    InitWindow(w_width, w_height, "Cellular Automata");
    while(!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
         {
             draw_cells((int*)cells);
             draw_rows();
             draw_cols();
             apply_rules(cells);
         }
        EndDrawing();
    }
    CloseWindow();
    free(cells);
    return 0;
}
