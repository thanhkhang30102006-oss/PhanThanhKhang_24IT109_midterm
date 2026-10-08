# BÁO CÁO LẬP TRÌNH HỆ THỐNG GIỮA KỲ
## Cài đặt và thực thi lệnh `ls(1)` đơn giản hóa

**Họ và tên:** Phan Thanh Khang  
**MSSV:** 24IT109  
**Môn học:** Lập trình hệ thống (Midterm Project)  
**Repository:** [GitHub Link](https://github.com/thanhkhang30102006oss/PhanThanhKhang_24IT109_midterm.git)

---

## 1. Yêu cầu, Cấu trúc chương trình & Cách thực thi

### 1.1 Yêu cầu hệ thống
* **Hệ điều hành:** NetBSD, Linux (Ubuntu/WSL)
* **Trình biên dịch C:** `gcc`
* **Công cụ build:** `make` (GNU Make)
* **Quản lý mã nguồn:** `git`

---

### 1.2 Cấu trúc chương trình

```text
Midterm/
├── include/
│   ├── display.h
│   ├── file_entry.h
│   ├── options.h
│   ├── sort.h
│   ├── traverse.h
│   └── utils.h
├── src/
│   ├── display.c
│   ├── file_entry.c
│   ├── main.c
│   ├── options.c
│   ├── sort.c
│   ├── traverse.c
│   └── utils.c
├── .gitignore
├── LICENSE
├── Makefile
└── README.md
```

---

### 1.3 Cách thực thi chương trình

**Bước 1: Clone repository**
```bash
git clone https://github.com/thanhkhang30102006oss/PhanThanhKhang_24IT109_midterm.git
cd Midterm
```

**Bước 2: Biên dịch chương trình**
```bash
make          # Biên dịch và tạo file thực thi ./ls
make clean    # Dọn dẹp các file .o và file thực thi
```

**Bước 3: Cú pháp chạy lệnh**
```bash
./ls [flags] [file/directory]
```

* **Chú thích:**
  * `./ls`: File thực thi sinh ra sau lệnh `make` (bắt buộc).
  * `[flags]`: Các cờ lệnh tùy chọn (ví dụ: `-a`, `-l`, `-R`,...).
  * `[file/directory]`: Đường dẫn file hoặc thư mục cần xem (nếu bỏ trống, mặc định sẽ liệt kê thư mục hiện tại).

* **Ví dụ mẫu:**
  * `./ls`: In danh sách file/thư mục hiện tại.
  * `./ls -a`: Hiển thị tất cả file/thư mục bao gồm cả file ẩn (`.` và `..`).
  * `./ls -la ./include`: Hiển thị thông tin chi tiết tất cả file trong thư mục `./include`.

---

### 1.4 Chức năng các cờ lệnh

| Cờ lệnh | Chức năng |
| :---: | :--- |
| `-A` | Liệt kê tất cả các thư mục và file ngoại trừ `.` và `..` |
| `-a` | In tất cả các thư mục và file bao gồm cả `.` và `..` |
| `-c` | Sử dụng thời điểm đổi trạng thái file (ctime) khi sắp xếp hoặc hiển thị (khi dùng với `-t` hoặc `-l`) |
| `-d` | Xem thư mục như một file thông thường (không liệt kê nội dung bên trong) |
| `-F` | Hiển thị ký tự đánh dấu loại file (`/` thư mục, `*` file thực thi, `@` symlink, `=` socket, `\|` FIFO, `%` whiteout) |
| `-f` | Không sắp xếp, in theo thứ tự đọc trong thư mục (bật ẩn danh sách giống `-a`) |
| `-h` | In kích thước file theo dạng human-readable (B, K, M, G) |
| `-i` | In số inode (serial number) của mỗi file/thư mục |
| `-k` | In kích thước file/thư mục theo đơn vị Kilobytes (KB) |
| `-l` | Hiển thị danh sách dạng chi tiết (long format: quyền, user, group, size, time, name) |
| `-n` | Tương tự `-l` nhưng hiển thị UID và GID thay vì User/Group name |
| `-q` | Bắt buộc chuyển các ký tự không in được (non-printable) thành dấu `?` |
| `-R` | Liệt kê đệ quy tất cả các thư mục con |
| `-r` | Đảo ngược thứ tự sắp xếp |
| `-S` | Sắp xếp danh sách theo kích thước file giảm dần |
| `-s` | Hiển thị số block hệ thống tập tin thực tế của mỗi file |
| `-t` | Sắp xếp theo thời gian sửa đổi (mtime) mới nhất |
| `-u` | Sử dụng thời gian truy cập gần nhất (atime) thay vì mtime |
| `-w` | Bắt buộc in nguyên bản các ký tự không in được |

> **Lưu ý về các cặp cờ đè lệnh nhau:**  
> Các cặp cờ đối lập bao gồm: `-c`/`-u`, `-R`/`-d`, `-k`/`-h`, `-l`/`-n`, `-w`/`-q`. Cờ nằm ở vị trí bên phải nhất trong câu lệnh sẽ đè tác động của cờ trước đó.  
> *Ví dụ:* `./ls -ln` sẽ hiển thị ID số (`UID`/`GID`) thay vì tên User/Group.

---

## 2. Thực hiện Demo chạy chương trình

### 2.1 Biên dịch chương trình
```bash
$ make clean && make
rm -f src/*.o ls
gcc -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -c src/display.c -o src/display.o
gcc -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -c src/file_entry.c -o src/file_entry.o
gcc -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -c src/main.c -o src/main.o
gcc -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -c src/options.c -o src/options.o
gcc -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -c src/sort.c -o src/sort.o
gcc -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -c src/traverse.c -o src/traverse.o
gcc -Wall -Wextra -Iinclude -std=c99 -D_DEFAULT_SOURCE -c src/utils.c -o src/utils.o
gcc src/display.o src/file_entry.o src/main.o src/options.o src/sort.o src/traverse.o src/utils.o -o ls
```

### 2.2 Demo các lệnh cơ bản

* **Chạy lệnh mặc định `./ls` và định dạng dài `./ls -l`:**
```bash
$ ./ls
LICENSE
Makefile
README.md
include
ls
src

$ ./ls -l
total 96
-rw-r--r-- 1 thanhkhang thanhkhang  1079 Oct  8 15:45 LICENSE
-rw-r--r-- 1 thanhkhang thanhkhang   279 Oct  8 18:57 Makefile
-rw-r--r-- 1 thanhkhang thanhkhang    76 Oct  8 15:45 README.md
drwxr-xr-x 2 thanhkhang thanhkhang  4096 Oct  8 18:39 include
-rwxr-xr-x 1 thanhkhang thanhkhang 26736 Oct  8 21:28 ls
drwxr-xr-x 2 thanhkhang thanhkhang  4096 Oct  8 21:28 src
```

* **Chạy lệnh xem chi tiết trên thư mục chỉ định `./ls -l ./include`:**
```bash
$ ./ls -l ./include
total 40
-rw-r--r-- 1 thanhkhang thanhkhang 488 Oct  8 18:13 display.h
-rw-r--r-- 1 thanhkhang thanhkhang 598 Oct  8 17:28 file_entry.h
-rw-r--r-- 1 thanhkhang thanhkhang 989 Oct  8 18:48 options.h
-rw-r--r-- 1 thanhkhang thanhkhang 178 Oct  8 17:48 sort.h
-rw-r--r-- 1 thanhkhang thanhkhang 150 Oct  8 18:40 traverse.h
-rw-r--r-- 1 thanhkhang thanhkhang   0 Oct  8 15:54 utils.h
```

### 2.3 Demo các cờ hiển thị & sắp xếp (`-a`, `-A`, `-t`, `-S`, `-r`, `-h`, `-i`, `-R`)

* **Hiển thị file ẩn với `-a` và `-A`:**
```bash
$ ./ls -a
.
..
.git
.gitignore
LICENSE
Makefile
README.md
include
ls
src
```

* **Hiển thị định dạng kích thước dễ đọc (`-h`) và số Inode (`-i`):**
```bash
$ ./ls -lh
total 96
-rw-r--r-- 1 thanhkhang thanhkhang 1.1K Oct  8 15:45 LICENSE
-rw-r--r-- 1 thanhkhang thanhkhang 279B Oct  8 18:57 Makefile
-rw-r--r-- 1 thanhkhang thanhkhang  76B Oct  8 15:45 README.md
drwxr-xr-x 2 thanhkhang thanhkhang 4.0K Oct  8 18:39 include
-rwxr-xr-x 1 thanhkhang thanhkhang  26K Oct  8 21:28 ls
drwxr-xr-x 2 thanhkhang thanhkhang 4.0K Oct  8 21:28 src

$ ./ls -li
total 96
55918 -rw-r--r-- 1 thanhkhang thanhkhang  1079 Oct  8 15:45 LICENSE
57399 -rw-r--r-- 1 thanhkhang thanhkhang   279 Oct  8 18:57 Makefile
55919 -rw-r--r-- 1 thanhkhang thanhkhang    76 Oct  8 15:45 README.md
 2327 drwxr-xr-x 2 thanhkhang thanhkhang  4096 Oct  8 18:39 include
57477 -rwxr-xr-x 1 thanhkhang thanhkhang 26736 Oct  8 21:28 ls
13682 drwxr-xr-x 2 thanhkhang thanhkhang  4096 Oct  8 21:28 src
```

* **Liệt kê đệ quy với `-R`:**
```bash
$ ./ls -R
LICENSE
Makefile
README.md
include
ls
src

./include:
display.h
file_entry.h
options.h
sort.h
traverse.h
utils.h

./src:
display.c
display.o
file_entry.c
file_entry.o
main.c
main.o
options.c
options.o
sort.c
sort.o
traverse.c
traverse.o
utils.c
utils.o
```
