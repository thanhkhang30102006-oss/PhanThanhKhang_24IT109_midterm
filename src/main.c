#include "options.h"
#include "traverse.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    ls_options_t opts;

    // Đọc cờ lệnh
    int optind_out = parse_options(argc, argv, &opts);

    // 2. Thực thi đọc, sắp xếp, đệ quy và hiển thị
    int exit_status = process_operands(argc, argv, optind_out, &opts);

    // 3. Trả về 0 nếu thành công toàn bộ
    return exit_status;
}