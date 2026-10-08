#include "traverse.h"
#include "file_entry.h"
#include "sort.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void process_directory_recursive(const char *dir_path, const ls_options_t *opts, int print_header, int *exit_code) {
    if (print_header) {
        printf("%s:\n", dir_path);
    }

    int count = 0;
    file_entry_t **entries = read_directory(dir_path, opts, &count);

    if (!entries && count == 0) {
        *exit_code = 1;
        return;
    }
    // sort file
    sort_file_entries(entries, count, opts);
    // hien thi file
    display_entries(entries, count, opts, 1);
    // neu bat co -R
    if (opts->recursive) {
        for (int i = 0; i < count; i++) {
            file_entry_t *entry = entries[i];

            // Chỉ duyệt nếu là thư mục va không phải '.' hoặc '..'
            if (S_ISDIR(entry->st.st_mode)) {
                if (strcmp(entry->name, ".") != 0 && strcmp(entry->name, "..") != 0) {
                    putchar('\n');
                    // Gọi đệ quy với đường dẫn đầy đủ của thư mục con
                    process_directory_recursive(entry->path, opts, 1, exit_code);
                }
            }
        }
    }
    free_file_entries(entries, count);
}

int process_operands(int argc, char *argv[], int optind, const ls_options_t *opts) {
    int exit_code = 0;
    int num_operands = argc - optind;
    // mac dinh liet ke thu muc hien tai
    if (num_operands == 0) {
        process_directory_recursive(".", opts, 0, &exit_code);
        return exit_code;
    }

    file_entry_t **files = malloc(num_operands * sizeof(file_entry_t *));
    file_entry_t **dirs = malloc(num_operands * sizeof(file_entry_t *));
    int file_count = 0;
    int dir_count = 0;

    for (int i = optind; i < argc; i++) {
        file_entry_t *entry = create_file_entry("", argv[i]);
        if (!entry) {
            *(&exit_code) = 1;
            continue;
        }

        // Nếu bật cờ -d
        if (opts->dir_as_file) {
            files[file_count++] = entry;
        } 
        // Nếu là thư mục
        else if (S_ISDIR(entry->st.st_mode)) {
            dirs[dir_count++] = entry;
        } 
        // Nếu là file thường / symlink / socket
        else {
            files[file_count++] = entry;
        }
    }
    // Sort file 
    sort_file_entries(files, file_count, opts);
    sort_file_entries(dirs, dir_count, opts);

    if (file_count > 0) {
        display_entries(files, file_count, opts, 0);
    }

    int show_header = (num_operands > 1) || opts->recursive;
    for (int i = 0; i < dir_count; i++) {
        if (file_count > 0 || i > 0) {
            putchar('\n');
        }

        process_directory_recursive(dirs[i]->path, opts, show_header, &exit_code);
    }

    free_file_entries(files, file_count);
    free_file_entries(dirs, dir_count);

    return exit_code;
}