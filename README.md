# NewOS: Tugas Besar IF2230 - Sistem Operasi 

> - Milestone 1 : 1 Maret 2022 - Pembuatan Sistem Operasi Sederhana: Booting, Kernel, dan System Call
> - Milestone 2 : 5 April 2022 - Pembuatan Filesystem dan Shell Sederhana
> - Milestone 3 : 30 April 2022 - Pembuatan dan Eksekusi Program Menggunakan System Call

## Deskripsi

Pada _milestone 1_, implementasi yang dilakukan adalah implementasi fungsi `printString, readString dan clearScreen` sebagai berikut:

1. `printString`
   - Menerima argumen berupa _character buffer_, kemudian memanggil fungsi _interrupt_ dengan _function code_ `0xE * 256 + buffer[i]` untuk menampilkan karakter pada buffer
   - Karakter akan terus ditampilkan hingga mencapai _null-terminating string_ (`\0`)
   - Diimplementasikan pada file `screen.c`
2. `readString`
   - Menerima argumen berupa _character buffer_, lalu memanggil fungsi _interrupt_ dengan _interrupt call_ 0x16 untuk membaca input _keyboard_
     - Apabila input sama dengan _enter code_ (0xD), maka pembacaan akan dihentikan
     - Apabila input sama dengan _backspace code_ (0x8), maka isi buffer - 1 akan dihapus
     - Apabila input di luar _enter / backspace code_, maka buffer akan diisikan dengan karakter
   - Diimplementasikan pada file `keyboard.c`
3. `clearScreen`
   - Membersihkan layar dengan memanggil _interrupt_ 0x10, _function code_ `0x06`
   - Relokasi kursor ke posisi kiri atas
   - Reset warna _buffer_ ke warna putih
   - [**IMPORTANT**] Fungsi ini dipanggil dengan meng-input karakter `cls` ke dalam `readString`
   - Diimplementasikan pada file `screen.c`

Pada _milestone 2_, implementasi yang dilakukan adalah implementasi fungsi `readSector`, `writeSector`, `read`, `write`, dan `shell` sebagai berikut:

1. `readSector` dan `writeSector`
   - Menerima argumen berupa _buffer_ dan _sector number_, selanjutnya memanggil fungsi _interrupt_ 0x13 (read/write services) dengan AX bernilai 0x0201 untuk read dan 0x0301 untuk write, BX bernilai _buffer_, CX bernilai (cylinder << 8) + sector, DX bernilai (head << 8) + drive
   - `readSector` melakukan pembacaan _sector_ sesuai dengan _sector number_ ke _buffer_, sebaliknya `writeSector` melakukan penulisan _sector_ sesuai dengan _sector number_ ke _buffer_
   - Diimplementasikan pada file `filesystem.c`
2. `read` dan `write`
   - Menerima argumen berupa _file metadata_ dan _return code_, kemudian melakukan pemrosesan dimulai dengan validasi terhadap masukan _file metadata_, pemrosesan read/write, dan diakhiri dengan pass-by-reference pada _return code_
   - `read` melakukan pembacaan _file_ sesuai dengan _file metadata_ ke _buffer_, sebaliknya `write` melakukan penulisan _file_ sesuai dengan _file metadata_ ke _buffer_. Kedua jenis pemrosesan file memanfaatkan fungsi `readSector` dan `writeSector` yang telah didefinisikan sebelumnya
   - Diimplementasikan pada file `filesystem.c`
3. `shell`
   - Menerima argumen berupa _string_, kemudian dilakukan parsing terhadap _string_ yang diterima. Posisi argumen _string_ pertama merupakan _utility_ dan posisi argumen _string_ berikutnya akan menjadi masukan pada pemanggilan _utility_. _utility_ yang diimplementasikan terdiri atas: `cd`, `ls`, `mv`, `mkdir`, `cat`, `cp`
   - _Utility_ `cd` merupakan alat dasar untuk melakukan navigasi pada filesystem. Diimplementasikan pada file `cd.c`
   - _Utility_ `ls` memperlihatkan konten yang ada pada current working directory. Diimplementasikan pada file `ls.c`
   - _Utility_ `mv` digunakan untuk melakukan operasi rename atau memindahkan file maupun folder. Diimplementasikan pada file `mv.c`
   - _Utility_ `mkdir` digunakan untuk membuat folder baru. Diimplementasikan pada file `mkdir.c`
   - _Utility_ `cat` digunakan untuk menampilkan isi dari suatu file sebagai text file. Diimplementasikan pada file `cat.c`
   - _Utility_ `cp` digunakan untuk menyalin file. Diimplementasikan pada file `cp.c`

Selain implementasi yang diwajibkan pada spesifikasi _milestone_, dilakukan pula implementasi tambahan sebagai tujuan kreativitas sebagai berikut:

1. Implementasi cancel input shortcut dan pengaksesan history menggunakan arrow key. Tujuan pengimplementasian ini untuk memudahkan dalam menghapus input atau melakukan inputan yang sudah pernah dilakukan sebelumnya
   - Fungsi ini dapat diimplementasikan pada file `keyboard.c`.
2. Implementasi color printing. Tujuan pengimplementasian ini untuk memudahkan dalam menampilkan karakter dengan warna tertentu. Salah satu penggunaannya adalah pada _utility_ `ls`. Warna dapat membedakan dan mengindikasikan suatu direktori merupakan sebuah folder atau sebuah file.
   - Fungsi ini dapat diimplementasikan pada file `screen.c`.

Pada _milestone 3_, ada beberapa implementasi tambahan:
1. Pembuatan _library_ dan _syscall_ `executeProgram`
   - Fungsionalitas-fungsionalitas yang ada sebelumnya dipindahkan ke beberapa library (dalam folder `src/library` seperti berikut:
      -  `args.c`: fungsionalitas untuk melakukan _parsing_ argumen dan valdiasi direktori
      -  `fileio.c`: fungsionalitas untuk melakukan penulisan dan pembacaan file (implementasi lebih lanjut dari _read/write_)
      -  `program.c`: inisialisasi program
      -  `shell_common.c`: fungsionalitas untuk mengatur _shell_
      -  `std_lib.c`: seperti milestone sebelumnya, std_lib mengandung fungsi-fungsi dasar seperti mod, div, memcpy, strcpy, dan lain-lain
      -  `string.c`: fungsi-fungsi untuk melakukan operasi pada string (strcpy, strcat, strlen)
      -  `syscall.c`: fungsionalitas layar yang diterjemahkan dalam bentuk _system call_
      -  `textio.c`: fungsi-fungsi untuk meng-_handle_ input/output program (gets/puts)
   -  Pemanggilan fungsi-fungsi pada _library_ dilakukan via `handleInterrupt21`, yang mengatur fungsi-fungsi yang harus dijalankan melalui _interrupt_ tertentu. Implementasi `executeProgram` juga telah dilakukan di _kernel_.
2. Pembuatan aplikasi _shell_
   -  Shell yang sebelumnya terpasang di _kernel_ via model _shared memory_ diubah menjadi model _message passing_. _Shell_ dan fungsionalitas dalam _shell_ diletakkan dalam folder `apps`
3. _Multiple program_
   - Pengguna dapat melakukan eksekusi _utility_ secara berantai. Eksekusi _utility_ memanfaatkan `exit` per eksekusinya.

Selain implementasi yang diwajibkan pada spesifikasi _milestone_, dilakukan pula implementasi tambahan sebagai tujuan kreativitas sebagai berikut:

1. Pembuatan permainan Snake pada `apps`
2. Pembuatan _text editor_ Nano pada `apps`
   - Layaknya utilitas sebelumnya, eksekusi kedua ini memanfaatkan `exit`.

### Requirements

- [**RECOMMENDED**] **Ubuntu 20.04** (pengembangan dilakukan pada WSL2)
- <a href = "https://sourceforge.net/projects/vcxsrv/">**VcXsrv Windows X Server**</a>
- **Windows 10** sebagai OS basis eksekusi

### Cara Menjalankan OS

1. Lakukan instalasi VcXsrv Windows X Server pada OS Windows, jalankan `XLaunch` lalu pada menu pilihan gunakan pilihan berikut:
   ```
   Select display settings -> dibebaskan
   Select how to start clients -> 'Start no client'
   Extra settings -> tandai semuanya
   Klik Finish
   ```
2. Buka WSL2, lalu navigasi ke _folder_ `root` dan jalankan perintah berikut untuk melakukan instalasi _library_:

   ```
   sudo apt update
   sudo apt install nasm bcc bin86 bochs bochs-x make
   ```

   Selanjutnya, jalankan kompilasi makefile menggunakan perintah berikut:

   ```
   make build-run
   ```

   Kompilasi Makefile akan berjalan, dan apabila muncul _prompt_ dari `bochs`, tekan 'c' untuk melanjutkan.
   
   Untuk melakukan kompilasi menggunakan _testcase_ dari _folder_ `tc_gen`, lakukan urutan langkah berikut ini:
   1. Pada `makefile -> tc:`, ubah nilai karakter pada `./tc_gen/tc_gen <KARAKTER>`. Misalkan ingin menjalankan _testcase_ A, ubah menjadi `./tc_gen/tc_gen A`.
   2. Jalankan perintah `make tc-run`, tekan `c` pada `bochs` lalu matikan OS.
   3. Kompilasi akan berlanjut dengan memasukkan _testcase_ pada `system.img`, dan setelah itu, OS akan terbuka lagi.

3. OS akan tampil di layar emulator Bochs. Pengguna dapat menggunakan utilitas-utilitas yang disediakan!

### Identitas

- <a href = "https://github.com/marfgold1">13520103 - Amar Fadil</a>
- <a href = "https://github.com/clumsyyyy">13520124 - Owen Christian Wijaya</a>
- <a href = "https://github.com/dennisheraldi">13520139 - Fachry Dennis Heraldi</a>
