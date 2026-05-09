#include <stdio.h>
#include <string.h>
#include "editor.h"

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

    while (fgets(line, sizeof(line), f) && i < MAX_LINES) {

        line[strcspn(line, "\n")] = 0;

        // Strip UTF-8 BOM from the first line if present
        if (i == 0 && (unsigned char)line[0] == 0xEF &&
            (unsigned char)line[1] == 0xBB &&
            (unsigned char)line[2] == 0xBF) {
            memmove(line, line + 3, strlen(line + 3) + 1);
        }

        // Replace tabs with spaces and remove other control chars
        for (char* p = line; *p; p++) {
            if (*p == '\t')
                *p = ' ';
            else if ((unsigned char)*p < 32)
                *p = ' ';
        }

        strncpy(text->content[i], line, MAX_LINE_LENGTH - 1);
        text->content[i][MAX_LINE_LENGTH - 1] = '\0';
        i++;
    }

    text->line_count = i > 0 ? i : 1;
    text->cursor_x = 0;
    text->cursor_y = 0;
    text->scroll_x = 0;
    if (text->line_count == 1 && text->content[0][0] == '\0')
        text->content[0][0] = '\0';

    fclose(f);
}

