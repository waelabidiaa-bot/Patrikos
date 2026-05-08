#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    int tab_size;
    int line_numbers;
    int status_bar;

    char theme[20];
} config_t;

config_t load_config(const char* path);

#endif