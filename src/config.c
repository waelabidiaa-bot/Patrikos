#include "config.h"
#include "../tomlc99/toml.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

config_t load_config(const char* path) {
    config_t cfg;

    // defaults
    cfg.tab_size = 4;
    cfg.line_numbers = 1;
    cfg.status_bar = 1;
    cfg.title_bar = 1;
    strcpy(cfg.theme, "dark");
    cfg.colors.title_fg = COLOR_WHITE;
    cfg.colors.title_bg = COLOR_BLUE;
    cfg.colors.status_fg = COLOR_WHITE;
    cfg.colors.status_bg = COLOR_BLACK;
    cfg.colors.text_fg = COLOR_WHITE;
    cfg.colors.text_bg = COLOR_BLACK;

    FILE* fp = fopen(path, "r");
    if (!fp) {
        return cfg; // return defaults if file not found
    }

    char errbuf[200];
    toml_table_t* conf = toml_parse_file(fp, errbuf, sizeof(errbuf));
    fclose(fp);

    if (!conf) {
        return cfg; // return defaults on parse error
    }

    // Load editor section
    toml_table_t* editor = toml_table_in(conf, "editor");
    if (editor) {
        toml_datum_t tab_size = toml_int_in(editor, "tab_size");
        if (tab_size.ok) cfg.tab_size = (int)tab_size.u.i;

        toml_datum_t line_numbers = toml_bool_in(editor, "line_numbers");
        if (line_numbers.ok) cfg.line_numbers = line_numbers.u.b;
    }

    // Load ui section
    toml_table_t* ui = toml_table_in(conf, "ui");
    if (ui) {
        toml_datum_t status_bar = toml_bool_in(ui, "status_bar");
        if (status_bar.ok) cfg.status_bar = status_bar.u.b;

        toml_datum_t title_bar = toml_bool_in(ui, "title_bar");
        if (title_bar.ok) cfg.title_bar = title_bar.u.b;

        toml_datum_t theme = toml_string_in(ui, "theme");
        if (theme.ok) {
            strncpy(cfg.theme, theme.u.s, sizeof(cfg.theme) - 1);
            cfg.theme[sizeof(cfg.theme) - 1] = '\0';
            free(theme.u.s);
        }
    }

    // Load colors section
    toml_table_t* colors = toml_table_in(conf, "colors");
    if (colors) {
        toml_datum_t title_fg = toml_int_in(colors, "title_fg");
        if (title_fg.ok) cfg.colors.title_fg = (int)title_fg.u.i;

        toml_datum_t title_bg = toml_int_in(colors, "title_bg");
        if (title_bg.ok) cfg.colors.title_bg = (int)title_bg.u.i;

        toml_datum_t status_fg = toml_int_in(colors, "status_fg");
        if (status_fg.ok) cfg.colors.status_fg = (int)status_fg.u.i;

        toml_datum_t status_bg = toml_int_in(colors, "status_bg");
        if (status_bg.ok) cfg.colors.status_bg = (int)status_bg.u.i;

        toml_datum_t text_fg = toml_int_in(colors, "text_fg");
        if (text_fg.ok) cfg.colors.text_fg = (int)text_fg.u.i;

        toml_datum_t text_bg = toml_int_in(colors, "text_bg");
        if (text_bg.ok) cfg.colors.text_bg = (int)text_bg.u.i;
    }

    toml_free(conf);
    return cfg;
}