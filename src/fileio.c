#include <stdio.h>
#include <string.h>
#include "editor.h"
#include "../include/ui.h"

void save_file(input_text* text) {

    char* filename;

    if (text->is_file_loaded)
        filename = text->filename;
    else
        return ; // or ask user later

    FILE* f = fopen(filename, "w");
    if (!f) return ;

    for (int i = 0; i < text->line_count; i++) {
        fprintf(f, "%s\n", text->content[i]);
    }

    fclose(f);
    return ;
}




//hanles saving and loading files based on user input
void load_file(input_text* text, const char* filename) {

    FILE* f = fopen(filename, "r");
    if (!f) return;

    memset(text->content, 0, sizeof(text->content));

    strncpy(text->filename, filename, 200);
    text->filename[199] = '\0';

    text->is_file_loaded = 1;

    char line[200];
    int i = 0;

    while (fgets(line, sizeof(line), f) && i < 100) {

        line[strcspn(line, "\n")] = 0;

        // Replace tabs with spaces
        for (char* p = line; *p; p++) {
            if (*p == '\t') *p = ' ';
        }

        strcpy(text->content[i], line);
        i++;
    }

    text->line_count = i;
    text->cursor_x = 0;
    text->cursor_y = 0;

    fclose(f);
}

