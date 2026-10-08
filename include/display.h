#ifndef DISPLAY_H
#define DISPLAY_H

#include "file_entry.h"
#include "options.h"
//- entries: Mảng các con trỏ file_entry_t đã được lọc và sắp xếp
// - count: Số lượng phần tử trong mảng
// - opts: Cấu hình các cờ lệnh
// - is_dir_contents: Bằng 1 nếu đây là nội dung bên trong một thư mục (cần in 'total' nếu có -l/-s)
void display_entries(file_entry_t **entries, int count, const ls_options_t *opts, int is_dir_contents);
#endif