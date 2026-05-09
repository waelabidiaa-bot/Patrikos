#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "include/editor.h"
#include "include/ui.h"
#include "include/input.h"
#include "include/config.h"

static int get_line_number_width(const input_text* text) {
    int max_line = text->line_count > 1 ? text->line_count : 1;
    int digits = 0;
    while (max_line > 0) {
        digits++;
        max_line /= 10;
    }
    return digits;
}

int main() {
    config_t cfg = load_config("config.toml");

    initscr();
    raw();
    noecho();
    curs_set(1);
    keypad(stdscr, TRUE);

    int use_color = 0;
    if (has_colors()) {
        use_color = 1;
        start_color();
        init_pair(1, cfg.colors.title_fg, cfg.colors.title_bg);
        init_pair(2, COLOR_BLACK, COLOR_CYAN); // fallback
        init_pair(3, cfg.colors.status_fg, cfg.colors.status_bg);
        init_pair(4, cfg.colors.text_fg, cfg.colors.text_bg);
    }

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
            handle_input(text, ch, &cfg);
        }

        int screen_h, screen_w;
        getmaxyx(stdscr, screen_h, screen_w);
        (void)screen_w;

        int top_bar_rows = cfg.title_bar ? 1 : 0;
        int status_bar_rows = cfg.status_bar ? 1 : 0;
        int display_rows = screen_h - top_bar_rows - status_bar_rows;
        if (display_rows < 1)
            display_rows = 1;

        int line_no_width = cfg.line_numbers ? get_line_number_width(text) : 0;
        int text_offset = cfg.line_numbers ? line_no_width + 3 : 0;
        int visible_text_width = screen_w - text_offset;

        // Horizontal scroll
        if (text->cursor_x >= text->scroll_x + visible_text_width)
            text->scroll_x = text->cursor_x - visible_text_width + 1;
        if (text->cursor_x < text->scroll_x)
            text->scroll_x = text->cursor_x;
        if (text->scroll_x < 0)
            text->scroll_x = 0;

        if (text->cursor_y >= text->scroll_y + display_rows)
            text->scroll_y = text->cursor_y - display_rows + 1;

        if (text->cursor_y < text->scroll_y)
            text->scroll_y = text->cursor_y;

        if (text->scroll_y < 0)
            text->scroll_y = 0;

        clear();

        int title_attr = use_color ? COLOR_PAIR(1) | A_BOLD : A_REVERSE | A_BOLD;
        if (cfg.title_bar) {
            attron(title_attr);
            mvprintw(0, 0, " patrikos - %s", text->filename[0] ? text->filename : "[new file]");
            clrtoeol();
            attroff(title_attr);
        }

        for (int i = 0; i < display_rows; i++) {
            int line = i + text->scroll_y;
            if (line >= text->line_count)
                break;

            if (cfg.line_numbers) {
                char truncated[201];
                int start = text->scroll_x;
                int len = strlen(text->content[line]) - start;
                if (len < 0) len = 0;
                if (len > visible_text_width) len = visible_text_width;
                strncpy(truncated, text->content[line] + start, len);
                truncated[len] = '\0';
                mvprintw(top_bar_rows + i, 0, "%*d | %s",
                         line_no_width,
                         line + 1,
                         truncated);
            } else {
                char truncated[201];
                int start = text->scroll_x;
                int len = strlen(text->content[line]) - start;
                if (len < 0) len = 0;
                if (len > visible_text_width) len = visible_text_width;
                strncpy(truncated, text->content[line] + start, len);
                truncated[len] = '\0';
                mvprintw(top_bar_rows + i, 0, "%s", truncated);
            }
        }

        if (cfg.status_bar) {
            int status_attr = use_color ? COLOR_PAIR(3) : A_REVERSE;
            attron(status_attr);
            mvprintw(screen_h - 1, 0,
                     " Ctrl+S Save | Ctrl+O Open | Ctrl+Q Quit | Ln %d Col %d ",
                     text->cursor_y + 1,
                     text->cursor_x + 1);
            clrtoeol();
            attroff(status_attr);
        }

        int cursor_screen_x = text->cursor_x - text->scroll_x + text_offset;
        if (cursor_screen_x < text_offset) cursor_screen_x = text_offset;
        if (cursor_screen_x >= screen_w) cursor_screen_x = screen_w - 1;
        move(text->cursor_y - text->scroll_y + top_bar_rows, cursor_screen_x);
        refresh();
    }

    free_input_text(text);
    endwin();

    return 0;
}