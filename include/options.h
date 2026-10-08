#ifndef OPTIONS_H
#define OPTIONS_H

#include <unistd.h>
typedef  struct 
{
    int showAllFile; //ls -a ..
    int showNearlyAll; // ls - A ..
    // - c và -u đè nhau
    int timeLastChange; //ls -c
    int time_access; // ls -u
    //
    // -R và -d đè nhau
    int dir_as_file; //ls -d Directory
    int recursive; //ls -R
    //
    int classify;   // ls -F
    int unsorted;  //ls -f
    // -k và -h đè nhau     ls -kh
    int knownSize; // ls -h
    int kbSize; // ls -k 
    //

    int inode; //ls -i
    // -l và -n đè nhau ls -ln
    int long_format; //ls -l
    int numeric_id; //ls -n
    //
    // - w và -q đè nhau
    int convert_strange_character; // ls -q
    int force_raw_character; //ls -w
    int reverse; //ls -r

    //sort
    int sort_by_size; // ls -S
    int sort_by_time; // ls -t
    //
    int show_blocks; //ls -s

} ls_options_t;

void init_options(ls_options_t *opts);

int parse_options(int argc, char *argv[], ls_options_t *opts);
#endif