#ifndef EDITOR_H
#define EDITOR_H

#define MAX_LINES 100
#define MAX_LINE_LENGTH 200

typedef struct text {
    char content[MAX_LINES][MAX_LINE_LENGTH];
    int line_count;
    int cursor_x;
    int cursor_y;
    int scroll_y;
    char filename[200];
    int is_file_loaded;
} input_text;

input_text* create_input_text();
void add_char(input_text* text, char c);
void add_newline(input_text* text);
void free_input_text(input_text* text);
int line_number(input_text* text);
int line_length(input_text* text, int line);
void save_file(input_text* text);
void load_file(input_text* text, const char* filename);

#endif