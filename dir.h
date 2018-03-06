#include<stdio.h>
#include<stdlib.h>

struct PATH{
    struct PATH* next;
    char* path;
    unsigned int size;
};

void search_path(char* current_path, struct PATH* head);