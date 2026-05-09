#include "../include/input.h"

#include <ncurses.h>
#include <string.h>



static void insert_tab_spaces(input_text* text, int tab_size) {
    for (int i = 0; i < tab_size; i++) {
        if (text->cursor_x < MAX_LINE_LENGTH - 1)
            add_char(text, ' ');
    }
}

static void delete_at_cursor(input_text* text) {
    int y = text->cursor_y;
    int x = text->cursor_x;
    int len = line_length(text, y);

    if (x < len) {
        for (int i = x; text->content[y][i] != '\0'; i++) {
            text->content[y][i] = text->content[y][i + 1];
        }
    } else if (y < text->line_count - 1) {
        int next_len = strlen(text->content[y + 1]);
        if (len + next_len < MAX_LINE_LENGTH) {
            strcat(text->content[y], text->content[y + 1]);
            for (int i = y + 1; i < text->line_count - 1; i++) {
                strcpy(text->content[i], text->content[i + 1]);
            }
            text->line_count--;
        }
    }
}

void handle_input(input_text* text, int ch, const config_t* cfg) {

    switch (ch) {

        case 10: // ENTER
            add_newline(text);
            break;

        case 9: // TAB
            insert_tab_spaces(text, cfg->tab_size);
            break;

        case KEY_DC:
            delete_at_cursor(text);
            break;

        case KEY_BACKSPACE:
        case 127:
        case 8:

            // ----------------------------
            // DELETE INSIDE LINE
            // ----------------------------
            if (text->cursor_x > 0) {

                int y = text->cursor_y;
                int x = text->cursor_x;

                for (int i = x; text->content[y][i] != '\0'; i++) {
                    text->content[y][i - 1] = text->content[y][i];
                }

                text->cursor_x--;
            }

            // ----------------------------
            // MERGE LINES
            // ----------------------------
            else if (text->cursor_y > 0) {

                int prev = text->cursor_y - 1;
                int prev_len = strlen(text->content[prev]);
                int curr_len = strlen(text->content[text->cursor_y]);

                if (prev_len + curr_len < MAX_LINE_LENGTH) {
                    // append current line to previous
                    strcat(text->content[prev], text->content[text->cursor_y]);

                    // shift lines up
                    for (int i = text->cursor_y; i < text->line_count - 1; i++) {
                        strcpy(text->content[i], text->content[i + 1]);
                    }

                    text->line_count--;

                    text->cursor_y--;
                    text->cursor_x = prev_len;
                }
            }

            break;

        case KEY_LEFT:
            if (text->cursor_x > 0)
                text->cursor_x--;
            break;

        case KEY_RIGHT:
            if (text->cursor_x < line_length(text, text->cursor_y))
                text->cursor_x++;
            break;

        case KEY_HOME:
            text->cursor_x = 0;
            break;

        case KEY_END:
            text->cursor_x = line_length(text, text->cursor_y);
            break;

        case KEY_UP:
            if (text->cursor_y > 0) {

                text->cursor_y--;

                if (text->cursor_x >
                    line_length(text, text->cursor_y)) {

                    text->cursor_x =
                        line_length(text, text->cursor_y);
                }
            }
            break;

        case KEY_DOWN:
            if (text->cursor_y < text->line_count - 1) {

                text->cursor_y++;

                if (text->cursor_x >
                    line_length(text, text->cursor_y)) {

                    text->cursor_x =
                        line_length(text, text->cursor_y);
                }
            }
            break;

        default:
            if (ch >= 32 && ch <= 126) {
                add_char(text, (char)ch);
            }
            break;
    }
}