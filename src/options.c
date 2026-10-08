#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void init_options(ls_options_t *opts){

    *opts = (ls_options_t){0};
    //root
    if (geteuid() == 0){
        opts->showNearlyAll = 1; 
    }
    // hiển thị ở terminal
    if (isatty(STDOUT_FILENO)){
        opts->convert_strange_character =1;
    }
    else {
        opts->force_raw_character =1;
    }
}

int parse_options(int argc, char *argv[], ls_options_t *opts){
    init_options(opts);
    int opt;

    const char *optstring = "AacdFfhiklnqRrSstuw";

    while ((opt = getopt(argc, argv, optstring)) !=-1){
        switch (opt){
            // Hien thi ngoai tru file an
            case 'A': opts->showNearlyAll =1;
            break;
            //Hien thi het tat ca file
            case 'a': opts->showAllFile=1;
            break;
            //Thoi gian trang thai lan cuoi thay doi
            case 'c': opts->timeLastChange=1;
                      opts->time_access=0;
            break;
            // Coi dir nhu file
            case 'd': opts->dir_as_file=1;
                      opts->recursive=0;
            break;
            // Phan loai file, thu muc
            case 'F': opts->classify=1;
            break;
            //hien thi khong sap xep
            case 'f': opts->unsorted=1;
                      opts->showAllFile=1;
            break;
            //Hien thi kich thuoc duoi dang doc duoc
            case 'h': opts->knownSize=1;
                      opts->kbSize=0;
            break;
            //Hien thi serial number cua file
            case 'i': opts->inode=1;
            break;
            // Hien thi kich thuoc KB
            case 'k': opts->kbSize=1;
                      opts->knownSize=0;
            break;
            // Hien thi long format
            case 'l': opts->long_format=1;
                      opts->numeric_id=0;
            break;
            // Hien thi giong -l nhung duoi dang id
            case 'n': opts->numeric_id=1;
                      opts->long_format=1;
            break;
            // Hien thi ky tu khong in duoc thanh ?
            case 'q': opts->convert_strange_character=1;
                      opts->force_raw_character=0;
            break;
            // De quy thu muc
            case 'R': opts->recursive=1;
                      opts->dir_as_file=0;
            break;
            //Dao nguoc sap xep
            case 'r': opts->reverse=1;
            break;
            // Sort theo size
            case 'S': opts->sort_by_size=1;
            break;
            // Show block
            case 's': opts->show_blocks=1;
            break;
            // Sort theo time
            case 't': opts->sort_by_time=1;
            break;
            // Co doi time sang accesstime
            case 'u': opts->time_access=1;
                      opts->timeLastChange=0;
            break;
            // Hien thi ky tu khong in dc
            case 'w': opts->force_raw_character=1;
                      opts->convert_strange_character=0;
            break;
            
            default:
                fprintf(stderr, "Usage: %s [-AacdFfhiklnqRrSstuw] [file ...]\n", argv[0]);
                exit(1);
        }
    }
    return optind;
}