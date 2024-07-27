#include <raylib.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_LEN 11

const int sq_width = 10;
const int sq_height = 10;

const int w_width = 800;
const int w_height = 800;

const int start_count = 500;
const int sq_count = (w_width / sq_width) * (w_height / sq_height);

const int rows = sq_count / (w_width / sq_width);
const int cols = sq_count / (w_height/ sq_height);

int live = 0;
int paused = 0;

const int button_height = 50;
const int button_width = 200;

struct button {
    char* text;
    Rectangle rect;
};

struct button play_pause;
struct button start_stop;

void init_buttons() {
    play_pause.text = malloc(MAX_LEN);
    strncpy(play_pause.text, "Pause", strnlen("Pause", MAX_LEN));
    play_pause.rect = (Rectangle){(w_width/4) - (button_width/2), w_height, button_width, button_height};
    
    start_stop.text = malloc(MAX_LEN);
    strncpy(start_stop.text, "Start", strnlen("Start", MAX_LEN));
    start_stop.rect = (Rectangle){(w_width - w_width/4) - (button_width/2), w_height, button_width, button_height};
}

void update_button_text() {
    if(paused) {
        strncpy(play_pause.text, "Play", strnlen("Play", MAX_LEN));
    } else {
        strncpy(play_pause.text, "Pause", strnlen("Pause", MAX_LEN));
    }

    if(live) {
        strncpy(start_stop.text, "Stop", strnlen("Stop", MAX_LEN));
    } else {
        strncpy(start_stop.text, "Start", strnlen("Start", MAX_LEN));
    }
}

void update_buttons(Vector2 point) {
    if(CheckCollisionPointRec(point, play_pause.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        paused = !paused;
    }

    if(CheckCollisionPointRec(point, start_stop.rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        live = !live;
    }

    update_button_text();
    DrawRectangleRec(play_pause.rect, BLUE);
    DrawText(play_pause.text, play_pause.rect.x + button_width/2 - MeasureText(play_pause.text, 12), play_pause.rect.y + button_height/2, 12, WHITE);

    DrawRectangleRec(start_stop.rect, BLUE);
    DrawText(start_stop.text, start_stop.rect.x + button_width/2 - MeasureText(start_stop.text, 12), start_stop.rect.y + button_height/2, 12, WHITE);
}

void draw_rows() {
    int curr = 0;
    for(int i = 0; i <= w_height; i += sq_height) {
        DrawLine(0, i, w_width, i, WHITE);
    }
}

void draw_cols() {
    int curr = 0;
    for(int i = 0; i <= w_height; i += sq_width) {
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

void start_life(int* cells) {
    srand(time(0));
    for(int i = 0; i < start_count; i++) {
        int idx = rand() % sq_count;
        cells[idx] = 1;
    }
}

int* init_grid(int start_count) {
    int* cells = malloc(sizeof(int) * sq_count);
    for(int i = 0; i < sq_count; i++) {
        cells[i] = 0;
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

void update(int* cells) {
    update_buttons(GetMousePosition());
    if(live) {
        if(!paused) {
            apply_rules(cells);
        }
        draw_rows();
        draw_cols();
        draw_cells(cells);
    }
}

int main(void) {
    SetTargetFPS(5);

    int* cells = init_grid(start_count);
    InitWindow(w_width, w_height + button_height, "Cellular Automata");
    init_buttons();
    start_life(cells);
    while(!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
         {
            printf("paused: %d, live: %d\n", paused, live);
            update(cells);
         }
        EndDrawing();
    }
    CloseWindow();
    free(cells);
    return 0;
}
