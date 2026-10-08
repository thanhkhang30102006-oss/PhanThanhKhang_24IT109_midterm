#ifndef SORT_H
#define SORT_H

#include "file_entry.h"
#include "options.h"
// Sap xep
void sort_file_entries(file_entry_t **entries, int count, const ls_options_t *opts);
#endif