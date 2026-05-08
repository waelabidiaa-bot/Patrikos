#include "../include/editor.h"
#include <stdlib.h>
#include <string.h>



input_text* create_input_text() {

    input_text* text = malloc(sizeof(input_text));

    memset(text->content, 0, sizeof(text->content));

    text->line_count = 1;
    text->cursor_x = 0;
    text->cursor_y = 0;
    text->scroll_y = 0;
    text->filename[0] = '\0';
    text->is_file_loaded = 0;



    return text;
}

int line_length(input_text* text, int y) {
    return strlen(text->content[y]);
}

void add_char(input_text* text, char c) {

    int y = text->cursor_y;
    int x = text->cursor_x;

    if (line_length(text, y) >= MAX_LINE_LENGTH - 1) return; // prevent overflow

    // shift right to allow insert in middle
    for (int i = line_length(text, y); i >= x; i--) {
        text->content[y][i + 1] = text->content[y][i];
    }

    text->content[y][x] = c;
    text->cursor_x++;
}

void add_newline(input_text* text) {

    int y = text->cursor_y;
    int x = text->cursor_x;

    if (text->line_count >= MAX_LINES - 1) return; // prevent overflow

    // shift lines down
    for (int i = text->line_count; i > y + 1; i--) {
        strcpy(text->content[i], text->content[i - 1]);
    }

    // split current line
    strcpy(text->content[y + 1], &text->content[y][x]);

    // cut current line
    text->content[y][x] = '\0';

    text->cursor_y++;
    text->cursor_x = 0;
    text->line_count++;
}

void free_input_text(input_text* text) {
    free(text);
}
