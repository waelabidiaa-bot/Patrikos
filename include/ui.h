#ifndef UI_H
#define UI_H    




typedef struct title_label {
    char ide_name[25];
    int ide_version;
} title_lbl;

title_lbl create_title_lbl(const char *name, int version);
void get_filename(char* buffer, int max_len);
#endif