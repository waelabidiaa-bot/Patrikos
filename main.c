#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "include/editor.h"
#include "include/ui.h"
#include "include/input.h"
#include "include/config.h"

int main() {

    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    input_text* text = create_input_text();
    




    int ch;

    while (1) {

        ch = getch();

        // EXIT
        if (ch == 17) { // Ctrl + Q
            break;
        }

        // SAVE
        if (ch == 19) { // Ctrl + S

            if (!text->is_file_loaded) {

                char filename[200];
                get_filename(filename, 200);

                strcpy(text->filename, filename);
                text->is_file_loaded = 1;
            }

            save_file(text);
        }

        // LOAD
        else if (ch == 15) { // Ctrl + O

            char filename[200];
            get_filename(filename, 200);

            strcpy(text->filename, filename);

            load_file(text, text->filename);
            text->is_file_loaded = 1;
        }

        else {
            handle_input(text, ch);
        }

        // SCREEN SIZE
        int screen_h, screen_w;
        getmaxyx(stdscr, screen_h, screen_w);

        // SCROLL DOWN
        if (text->cursor_y >= text->scroll_y + screen_h - 1)
            text->scroll_y++;

        // SCROLL UP
        if (text->cursor_y < text->scroll_y)
            text->scroll_y--;

        if (text->scroll_y < 0)
            text->scroll_y = 0;

        clear();

        // RENDER WITH SCROLL
        for (int i = 0; i < screen_h - 1; i++) {

            int line = i + text->scroll_y;

            if (line >= text->line_count)
                break;

            mvprintw(i, 0, "%2d | %s",
                     line + 1,
                     text->content[line]);
        }

        move(text->cursor_y - text->scroll_y,
             text->cursor_x + 5);

        refresh();
    }

    free_input_text(text);
    endwin();

    return 0;
}