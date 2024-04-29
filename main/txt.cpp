#include <stdio.h>
#include <iostream>
#include "function.h"
void txt_ini() {
    FILE *file;
    char filename[] = "lcd.txt";
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("creat new file%s\n", filename);

        
        file = fopen(filename, "w");
        if (file == NULL) {
            fprintf(stderr, "creat file error %s\n", filename);
        }
        fclose(file);
    } else {
        printf("file : %s existx pass\n", filename);
        fclose(file);
    }
}
void write_txt(const char *text) {
    FILE *file;
    char filename[] = "lcd.txt";

    file = fopen(filename, "w");

    if (file != NULL) {
        fprintf(file, "%s", text);
        fclose(file);
    } else {
        printf("Unable to open file %s\n", filename);
    }
}
char* read_txt() {
    FILE *file;
    char filename[] = "lcd.txt";
    char line[100];
    char* result = NULL;

    file = fopen(filename, "r");

    if (file != NULL) {
        if (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;
            
            if (strstr(line, "state") != NULL) {
                fclose(file);

                file = fopen(filename, "w");
                fclose(file);

                result = strdup(line);
            } else {
                fclose(file);
                return NULL;
            }
        } else {
            fclose(file);
            return NULL;
        }
    } else {
        printf("Unable to open file %s\n", filename);
        return NULL;
    }

    return result;
}
