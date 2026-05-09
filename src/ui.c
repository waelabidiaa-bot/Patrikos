#include "../include/ui.h"
#include <string.h>
#include <stdlib.h>



title_lbl create_title_lbl(const char *name, int version) {
    title_lbl t;
    strncpy(t.ide_name, name, 24);
    t.ide_name[24] = '\0';
    t.ide_version = version;
    return t;
}
#include <ncurses.h>
#include <string.h>

void get_filename(char* buffer, int max_len) {

    echo();          // show typed characters
    curs_set(1);     // show cursor

    mvprintw(0, 0, "File name: ");
    clrtoeol();
    refresh();

    wgetnstr(stdscr, buffer, max_len - 1);

    noecho();        // restore editor mode
    curs_set(0);
}