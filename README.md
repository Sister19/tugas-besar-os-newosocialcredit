# Milestone 1 Tugas Besar IF2230
> Implementasi fungsi-fungsi dasar dan pembuatan _Makefile_
> sebagai _Milestone 1_ Tugas Besar IF2230 Sistem Operasi 2021/2022

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

## Cara Penggunaan
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

3. OS akan tampil di layar emulator Bochs. Alur kerja OS adalah sebagai berikut:
    - OS menerima tiga buah _command_:
        - `rw` (read/write), menjalankan protokol pembacaan _string_, kemudian menampilkan kembali _string_ tersebut ke layar 
        - `cls` (clear screen), membersihkan layar dan mengubah warna _buffer_ menjadi putih
        - `help`, menampilkan menu _help_
    - _Command_ `rw` adalah implementasi dari fungsi `readString/printString`, dan _command_ `cls` adalah implementasi dari fungsi `clearScreen`
### Identitas
- <a href = "https://github.com/marfgold1">13520103 - Amar Fadil</a>
- <a href = "https://github.com/clumsyyyy">13520124 - Owen Christian Wijaya</a>
- <a href = "https://github.com/dennisheraldi">13520139 - Fachry Dennis Heraldi</a>
