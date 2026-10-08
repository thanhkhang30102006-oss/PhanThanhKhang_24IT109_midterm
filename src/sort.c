#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const ls_options_t *g_opts = NULL;

static int compare_entries(const void *a, const void *b){
    const file_entry_t *entry_a = *(const file_entry_t **)a;
    const file_entry_t *entry_b = *(const file_entry_t **)b;

    int result = 0;

    if (g_opts->sort_by_size){
        if(entry_a->st.st_size < entry_b->st.st_size){
            result = 1;
        }
        else if (entry_a->st.st_size > entry_b->st.st_size){
            result =-1;
        }
    }

    else if(g_opts->sort_by_time){
        time_t time_a, time_b;
        if(g_opts->timeLastChange){
            time_a = entry_a->st.st_ctime;
            time_b = entry_b->st.st_ctime;
        }
        else if(g_opts->time_access){
            time_a = entry_a->st.st_atime;
            time_b = entry_b->st.st_atime;
        }
        else {
            time_a = entry_a->st.st_mtime;
            time_b = entry_b->st.st_mtime;
        }

        if (time_a < time_b) {
            result = 1;
        } else if (time_a > time_b) {
            result = -1;
        }
    }
    // Xep theo bang chu cai
    if (result == 0) {
        result = strcoll(entry_a->name, entry_b->name);
        if (result == 0) {
            result = strcmp(entry_a->name, entry_b->name);
        }
    }

    if (g_opts->reverse) {
        result = -result;
    }
    return result;
}

void sort_file_entries(file_entry_t **entries, int count, const ls_options_t *opts){
    if (!entries || count <= 1 || !opts) {
        return;
    }

    if (opts->unsorted) {
        return;
    }

    g_opts = opts;
    qsort(entries, count, sizeof(file_entry_t *), compare_entries);
    g_opts = NULL;
}