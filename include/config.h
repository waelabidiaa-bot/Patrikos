#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int tab_size;
    int line_numbers;
    int status_bar;
    int title_bar;
    char theme[20];
    struct {
        int title_fg;
        int title_bg;
        int status_fg;
        int status_bg;
        int text_fg;
        int text_bg;
    } colors;
} config_t;

config_t load_config(const char* path);

#endif