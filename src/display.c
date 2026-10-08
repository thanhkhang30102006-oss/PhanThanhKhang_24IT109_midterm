#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef __linux__
#include <sys/sysmacros.h>
#endif

static long get_block_size_unit(void);
static void format_human_size(off_t bytes, char *out_buf, size_t buf_len);
static void print_filename(const char *name, const ls_options_t *opts);
static void print_classified_suffix(mode_t mode);
static void get_mode_string(mode_t mode, char *str);
static void format_entry_time(const file_entry_t *entry, const ls_options_t *opts, char *out_buf, size_t buf_len);
static void print_long_entry(const file_entry_t *entry, const ls_options_t *opts);
static void print_short_entry(const file_entry_t *entry, const ls_options_t *opts);

static long get_block_size_unit(void) {
    char *env_blocksize = getenv("BLOCKSIZE");
    if (env_blocksize && strlen(env_blocksize) > 0) {
        long bs = atol(env_blocksize);
        if (bs > 0) return bs;
    }
    return 512;
}

static void format_human_size(off_t bytes, char *out_buf, size_t buf_len) {
    const char *units[] = {"B", "K", "M", "G", "T", "P"};
    double size = (double)bytes;
    int unit_idx = 0;

    while (size >= 1024.0 && unit_idx < 5) {
        size /= 1024.0;
        unit_idx++;
    }

    if (unit_idx == 0) {
        snprintf(out_buf, buf_len, "%4lldB", (long long)bytes);
    } else if (size < 10.0) {
        snprintf(out_buf, buf_len, "%3.1f%s", size, units[unit_idx]);
    } else {
        snprintf(out_buf, buf_len, "%4.0f%s", size, units[unit_idx]);
    }
}


static void print_filename(const char *name, const ls_options_t *opts) {
    for (size_t i = 0; i < strlen(name); i++) {
        unsigned char c = (unsigned char)name[i];
        if (opts->convert_strange_character && !isprint(c)) {
            putchar('?');
        } else {
            putchar(c);
        }
    }
}

static void print_classified_suffix(mode_t mode) {
    if (S_ISDIR(mode)) {
        putchar('/');
    } else if (S_ISLNK(mode)) {
        putchar('@');
    } else if (S_ISSOCK(mode)) {
        putchar('=');
    } else if (S_ISFIFO(mode)) {
        putchar('|');
    } else if ((mode & S_IXUSR) || (mode & S_IXGRP) || (mode & S_IXOTH)) {
        putchar('*');
    }
}

static void get_mode_string(mode_t mode, char *str) {
    if (S_ISDIR(mode))       str[0] = 'd';
    else if (S_ISLNK(mode))  str[0] = 'l';
    else if (S_ISBLK(mode))  str[0] = 'b';
    else if (S_ISCHR(mode))  str[0] = 'c';
    else if (S_ISSOCK(mode)) str[0] = 's';
    else if (S_ISFIFO(mode)) str[0] = 'p';
    else                     str[0] = '-';

    str[1] = (mode & S_IRUSR) ? 'r' : '-';
    str[2] = (mode & S_IWUSR) ? 'w' : '-';
    if (mode & S_ISUID) {
        str[3] = (mode & S_IXUSR) ? 's' : 'S';
    } else {
        str[3] = (mode & S_IXUSR) ? 'x' : '-';
    }

    str[4] = (mode & S_IRGRP) ? 'r' : '-';
    str[5] = (mode & S_IWGRP) ? 'w' : '-';
    if (mode & S_ISGID) {
        str[6] = (mode & S_IXGRP) ? 's' : 'S';
    } else {
        str[6] = (mode & S_IXGRP) ? 'x' : '-';
    }

    str[7] = (mode & S_IROTH) ? 'r' : '-';
    str[8] = (mode & S_IWOTH) ? 'w' : '-';
    if (mode & S_ISVTX) {
        str[9] = (mode & S_IXOTH) ? 't' : 'T';
    } else {
        str[9] = (mode & S_IXOTH) ? 'x' : '-';
    }

    str[10] = '\0';
}

static void format_entry_time(const file_entry_t *entry, const ls_options_t *opts, char *out_buf, size_t buf_len) {
    time_t target_time;

    if (opts->timeLastChange) {
        target_time = entry->st.st_ctime;
    } else if (opts->time_access) {
        target_time = entry->st.st_atime;
    } else {
        target_time = entry->st.st_mtime;
    }

    struct tm *tm_info = localtime(&target_time);
    strftime(out_buf, buf_len, "%b %e %H:%M", tm_info);
}

static void print_long_entry(const file_entry_t *entry, const ls_options_t *opts) {
    if (opts->inode) {
        printf("%llu ", (unsigned long long)entry->st.st_ino);
    }

    if (opts->show_blocks) {
        long block_unit = get_block_size_unit();
        long long num_blocks = (entry->st.st_blocks * 512 + block_unit - 1) / block_unit;
        printf("%lld ", num_blocks);
    }

    char mode_str[11];
    get_mode_string(entry->st.st_mode, mode_str);
    printf("%s ", mode_str);

    printf("%4lu ", (unsigned long)entry->st.st_nlink);

    if (opts->numeric_id) {
        printf("%-8u %-8u ", (unsigned int)entry->st.st_uid, (unsigned int)entry->st.st_gid);
    } else {
        struct passwd *pw = getpwuid(entry->st.st_uid);
        struct group  *gr = getgrgid(entry->st.st_gid);

        if (pw) printf("%-8s ", pw->pw_name);
        else    printf("%-8u ", (unsigned int)entry->st.st_uid);

        if (gr) printf("%-8s ", gr->gr_name);
        else    printf("%-8u ", (unsigned int)entry->st.st_gid);
    }

    if (S_ISCHR(entry->st.st_mode) || S_ISBLK(entry->st.st_mode)) {
        printf("%3u, %3u ", major(entry->st.st_rdev), minor(entry->st.st_rdev));
    } else if (opts->knownSize) {
        char hbuf[16];
        format_human_size(entry->st.st_size, hbuf, sizeof(hbuf));
        printf("%5s ", hbuf);
    } else if (opts->kbSize) {
        long long size_kb = (entry->st.st_size + 1023) / 1024;
        printf("%8lld ", size_kb);
    } else {
        printf("%8lld ", (long long)entry->st.st_size);
    }

    char time_str[32];
    format_entry_time(entry, opts, time_str, sizeof(time_str));
    printf("%s ", time_str);

    print_filename(entry->name, opts);
    if (opts->classify) {
        print_classified_suffix(entry->st.st_mode);
    }

    if (S_ISLNK(entry->st.st_mode) && entry->link_target) {
        printf(" -> %s", entry->link_target);
    }

    putchar('\n');
}

static void print_short_entry(const file_entry_t *entry, const ls_options_t *opts) {
    if (opts->inode) {
        printf("%llu ", (unsigned long long)entry->st.st_ino);
    }

    if (opts->show_blocks) {
        long block_unit = get_block_size_unit();
        long long num_blocks = (entry->st.st_blocks * 512 + block_unit - 1) / block_unit;
        printf("%lld ", num_blocks);
    }

    print_filename(entry->name, opts);

    if (opts->classify) {
        print_classified_suffix(entry->st.st_mode);
    }

    putchar('\n');
}

void display_entries(file_entry_t **entries, int count, const ls_options_t *opts, int is_dir_contents) {
    if (!entries || count <= 0) return;

    if (is_dir_contents && (opts->long_format || opts->numeric_id || opts->show_blocks)) {
        long long total_blocks = 0;
        long block_unit = get_block_size_unit();

        for (int i = 0; i < count; i++) {
            total_blocks += (entries[i]->st.st_blocks * 512 + block_unit - 1) / block_unit;
        }
        printf("total %lld\n", total_blocks);
    }

    for (int i = 0; i < count; i++) {
        if (opts->long_format || opts->numeric_id) {
            print_long_entry(entries[i], opts);
        } else {
            print_short_entry(entries[i], opts);
        }
    }
}