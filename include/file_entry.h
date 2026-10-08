#ifndef FILE_ENTRY_H
#define FILE_ENTRY_H

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "options.h"

typedef struct {
    char *name;          
    char *path;          
    struct stat st;      
    char *link_target;
} file_entry_t;

file_entry_t *create_file_entry(const char *parent_dir, const char *name);
// doc toan bo file/thu muc con ben trong mot thu muc
file_entry_t **read_directory(const char *dir_path, const ls_options_t *opts, int *out_count);

void free_file_entry(file_entry_t *entry);
void free_file_entries(file_entry_t **entries, int count);
#endif