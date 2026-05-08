#include "config.h"
#include "../tomlc99/toml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

config_t load_config(const char* path) {
    config_t cfg;

    // defaults
    cfg.tab_size = 4;
    cfg.line_numbers = 1;
    cfg.status_bar = 1;
    strcpy(cfg.theme, "dark");

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

        toml_datum_t theme = toml_string_in(ui, "theme");
        if (theme.ok) {
            strncpy(cfg.theme, theme.u.s, sizeof(cfg.theme) - 1);
            cfg.theme[sizeof(cfg.theme) - 1] = '\0';
            free(theme.u.s);
        }
    }

    toml_free(conf);
    return cfg;
}