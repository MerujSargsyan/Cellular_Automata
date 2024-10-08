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

bool live = false;
bool paused = false;
bool brushing = false;

const int h_button = 50;
const int w_button = 200;

const int h_brush = 50;
const int w_brush = 50;

int cells[sq_count];

struct button {
    char* text;
    Rectangle rect;
    Color col;
};

struct button play_pause;
struct button start_stop;
struct button brush;

bool is_board_empty() {
    for(int i = 0; i < sq_count; i++) {
        if(cells[i]) {
            return false;
        }
    }
    return true;
}

void make_grid() {
    for(int i = 0; i < sq_count; i++) {
        cells[i] = 0;
    }
}

void start_life() {
    srand(time(0));
    for(int i = 0; i < start_count; i++) {
        int idx = rand() % sq_count;
        cells[idx] = 1;
    }
}

void init_buttons() {
    play_pause.text = malloc(MAX_LEN);
    strncpy(play_pause.text, "Pause", strnlen("Pause", MAX_LEN));
    play_pause.rect = (Rectangle){(w_width/4) - (w_button/2), w_height, w_button, h_button};
    play_pause.col = BLUE;
    
    start_stop.text = malloc(MAX_LEN);
    strncpy(start_stop.text, "Start", strnlen("Start", MAX_LEN));
    start_stop.rect = (Rectangle){(w_width - w_width/4) - (w_button/2), w_height, w_button, h_button};
    start_stop.col = BLUE;

    brush.text = malloc(MAX_LEN); 
    strncpy(brush.text, "B", strnlen("B", MAX_LEN));
    brush.rect = (Rectangle){(w_width - w_brush)/2, w_height, w_brush, h_brush};
    brush.col = BLUE;
}

void update_button_text() {
    free(play_pause.text);
    play_pause.text = malloc(MAX_LEN);
    if(paused) {
        strncpy(play_pause.text, "Play", strnlen("Play", MAX_LEN));
        play_pause.col = RED;
    } else {
        strncpy(play_pause.text, "Pause", strnlen("Pause", MAX_LEN));
        play_pause.col = BLUE;
    }

    free(start_stop.text);
    start_stop.text = malloc(MAX_LEN);
    if(live) {
        strncpy(start_stop.text, "Stop", strnlen("Stop", MAX_LEN));
    } else {
        strncpy(start_stop.text, "Start", strnlen("Start", MAX_LEN));
    }

    if(brushing) {
        brush.col = RED;
    } else {
        brush.col = BLUE;
    }
}

void update_buttons(Vector2 point) {
    if(CheckCollisionPointRec(point, play_pause.rect) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        paused = !paused;
    }

    if(CheckCollisionPointRec(point, start_stop.rect) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if(is_board_empty()) make_grid();
        if(!live) {
            if (!brushing) start_life();
            brushing = false;
        } else {
            make_grid();
        }
        live = !live;
    }

    if(CheckCollisionPointRec(point, brush.rect) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if(!live) {
            brushing = !brushing;
        }
    }

    update_button_text();

    DrawRectangleRec(play_pause.rect, play_pause.col);
    DrawText(play_pause.text, play_pause.rect.x + w_button/2 - MeasureText(play_pause.text, 12), play_pause.rect.y + h_button/2, 12, WHITE);

    DrawRectangleRec(start_stop.rect, start_stop.col);
    DrawText(start_stop.text, start_stop.rect.x + w_button/2 - MeasureText(start_stop.text, 12), start_stop.rect.y + h_button/2, 12, WHITE);

    DrawRectangleRec(brush.rect, brush.col);
    DrawText(brush.text, brush.rect.x + w_brush/2 - MeasureText(brush.text, 12), brush.rect.y + h_brush/2, 12, WHITE);
}

void free_buttons() {
    free(play_pause.text);
    free(start_stop.text);
    free(brush.text);
}

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
void draw_cells(Vector2 m_pos) {
    int x = m_pos.x / sq_width;
    int y = m_pos.y / sq_height;
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(cells[r * cols + c]) {
                DrawRectangle(c * sq_width, r * sq_height, sq_width, sq_height, WHITE);
            } else if(y == r && x == c) {
                DrawRectangle(c * sq_width, r * sq_height, sq_width, sq_height, GRAY);
            }
            else {
                DrawRectangle(c * sq_width, r * sq_height, sq_width, sq_height, BLACK);
            }
        }
    }
    draw_rows();
    draw_cols();
}

int neighbor_count(int r, int c) {
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

void apply_rules() {
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            int n = neighbor_count(r, c);
            int idx = r*rows+c;
            if(n > 3 || n < 2) {
                cells[idx] = 0;
            } else if(n == 3) {
                cells[idx] = 1;
            }
        }
    }
}

void use_brush(Vector2 m_pos) {
    if(!brushing) return;
    int x = m_pos.x / sq_width;
    int y = m_pos.y / sq_height;
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        cells[y * rows + x] = 1;
    }
}

void update() {
    Vector2 m_pos = GetMousePosition();
    update_buttons(m_pos);
    if(live) {
        if(!paused) {
            apply_rules();
        }
    } else {
        use_brush(m_pos);
    }
    draw_cells(m_pos);
}

int main(void) {
    SetTargetFPS(5);

    make_grid();
    InitWindow(w_width, w_height + h_button, "Cellular Automata");
    init_buttons();

    while(!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();
        update();
        EndDrawing();
    }
    CloseWindow();
    free_buttons();
    return 0;
}
