#include "file_entry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

static char *build_full_path(const char *parent_dir, const char *name){
    if (!parent_dir || strlen(parent_dir) == 0){
        return strdup(name);
    }

    size_t parentLength= strlen(parent_dir);
    size_t nameLength= strlen(name);

    int need_slash = (parent_dir[parentLength -1] != '/');

    char *full_path = malloc(parentLength + nameLength + (need_slash ? 2 : 1));
    if(!full_path){
        perror("ls: malloc failed");
        return NULL;
    }
    if(need_slash){
        sprintf(full_path, "%s/%s", parent_dir, name);
    }
    else {
        sprintf(full_path, "%s%s", parent_dir, name);
    }
    return full_path;
}

file_entry_t *create_file_entry(const char *parent_dir, const char *name){
    file_entry_t *entry = calloc(1, sizeof(file_entry_t));
    if(!entry){
        perror("ls: malloc failed");
        return NULL;
    }

    entry->name = strdup(name);
    entry->path = build_full_path(parent_dir, name);
    if (!entry->name || !entry->path) {
        free_file_entry(entry);
        return NULL;
    }

    if (lstat(entry->path, &entry->st) != 0) {
        fprintf(stderr, "ls: cannot access '%s': ", entry->path);
        perror("");
        free_file_entry(entry);
        return NULL;
    }

    if (S_ISLNK(entry->st.st_mode)) {
        char buf[1024];
        ssize_t len = readlink(entry->path, buf, sizeof(buf) - 1);
        if (len != -1) {
            buf[len] = '\0';
            entry->link_target = strdup(buf);
        } else {
            entry->link_target = NULL;
        }
    }

    return entry;
}

file_entry_t **read_directory(const char *dir_path, const ls_options_t *opts, int *out_count){
    *out_count=0;

    DIR *dir = opendir(dir_path);
    if (!dir) {
        fprintf(stderr, "ls: cannot open directory '%s': ", dir_path);
        perror("");
        return NULL;
    }

    int capacity = 16;
    file_entry_t **entries = malloc(capacity * sizeof(file_entry_t *));
    if (!entries) {
        closedir(dir);
        return NULL;
    }

    struct dirent *dp;
    while ((dp= readdir(dir)) != NULL){
        if (opts->showAllFile){

        }
        else if (opts->showNearlyAll) {
            if (strcmp(dp->d_name, ".") ==0 || strcmp(dp->d_name, "..") ==0){
                continue;
            }
        }

        else {
            if (dp->d_name[0] == '.'){
                continue;
            }
        }

        file_entry_t *entry= create_file_entry(dir_path, dp->d_name);
        if(!entry){
            continue;
        }

        if (*out_count >= capacity) {
            capacity *= 2;
            file_entry_t **new_entries = realloc(entries, capacity * sizeof(file_entry_t *));
            if (!new_entries) {
                perror("ls: realloc failed");
                free_file_entry(entry);
                break;
            }
            entries = new_entries;
        }
        entries[(*out_count)++] = entry;
    }
    closedir(dir);
    return entries;
}

void free_file_entry(file_entry_t *entry) {
    if (!entry) return;
    if (entry->name) free(entry->name);
    if (entry->path) free(entry->path);
    if (entry->link_target) free(entry->link_target);
    free(entry);
}

void free_file_entries(file_entry_t **entries, int count) {
    if (!entries) return;
    for (int i = 0; i < count; i++) {
        free_file_entry(entries[i]);
    }
    free(entries);
}