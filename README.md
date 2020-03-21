# SoalShiftSISOP20_modul2_T20
Praktikum Sistem Operasi 2020 - Modul 2 <br />
Departemen Teknologi Informasi

Kelompok T20

Anggota :
- Fikri Haykal
- Hana Ghaliyah Azhar

# SOAL
## SOAL 1
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan
ketentuan sebagai berikut: <br />
- Program menerima 4 argumen berupa: <br />
a. Detik: 0-59 atau * (any value) <br />
b. Menit: 0-59 atau * (any value) <br />
c. Jam: 0-23 atau * (any value) <br />
d. Path file .sh <br />
- Program akan mengeluarkan pesan error jika argumen yang diberikan tidak
sesuai <br />
- Program hanya menerima 1 config cron <br />
- Program berjalan di background (daemon) <br />
- Tidak boleh menggunakan fungsi system() <br />

Contoh: ./program \* 34 7 /home/somi/test.sh <br />
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap detik pada jam 07:34. <br />

### Pembahasan SOAL 1
- Masuk ke directory soal 1 <br />
Syntax : `cd Downloads/Modul2/soal1`
- Membuat file <b>soal1.c</b> yang berisi program C<br />
Syntax : `nano soal1.c` <br />
___source code : [soal1.c](https://github.com/fikrihaykal/SoalShiftSISOP20_modul2_T20/blob/master/soal1/soal1.c)___ <br />
- Compile program <br />
Perintah `gcc nama_program.c -o nama_file_outputd` digunakan untuk mengcompile program C. <br />
Syntax : `gcc soal1.c -o soal1` <br />
- Menjalankan program <br />
Perintah `./nama_file_outputd` digunakan untuk menjalankan program C. <br />
Syntax : `./namafile detik menit jam lokasi.sh` <br />


##### Penjelasan Script yang ada di dalam file _soal1.c_ <br />
Fungsi untuk mengecek apakah argumen merupakan angka. <br /> 
```
int cekAngka(char temp[]){
    for(int i=0; i<strlen(temp); i++){
        if((temp[i] < '0') || (temp[i] > '9')){
            return 0;
            break;
        }
    }
    return 1;
}
```
- Perintah `strlen()` digunakan untuk menghitung panjang string. <br />

Fungsi utama dari program <b>soal1.c</b>. 
```
int main(int argc, char **argv){
    if(argc != 5){
```
- Perintah `argc` sebagai parameter bertipe int dan berfungsi untuk menunjukkan banyaknya parameter yang digunakan. 
- Perintah `argv` bertipe char berfungsi untuk menyimpan parameter apa saja yang digunakan (dalam bentuk array). <br />

Mengecek jumlah argumen
```
printf("Argumen anda salah\n");
        exit(EXIT_FAILURE);
    } else{
        int jadwal[4];
```

Mengecek apakah argumen benar dengan memanggil fungsi cekAsterisk() dan cekAngka(). <br /> 
```
for(int i=1; i<4; i++){
```

Mengecek apakah argumen adalah asterisk. 
```
              if(strcmp(argv[i], "*")){
                jadwal[i] = -5;
            } else if(cekAngka(argv[i])){
                jadwal[i] = atoi(argv[i]);
            } else{
                printf("Argumen anda salah\n");
                exit(EXIT_FAILURE);
            }
```
- Perintah `strcmp()` digunakan untuk membandingkan string dengan string yang lainnya. <br />

Membuat <b>Daemon</b>, langkah pertama yaitu membuat sebuah parent process dan memunculkan child process dengan melakukan `fork()`
```
        pid_t pid, sid;
        pid = fork();

        if(pid < 0){
            exit(EXIT_FAILURE);
        }

        if(pid > 0){
            exit(EXIT_SUCCESS);
        }

        umask(0);
        sid = setsid();
        
        if(sid < 0){
            exit(EXIT_FAILURE);
        }
```
- Fungsi `pid_t pid` Variabel untuk menyimpan PID
- Fungsi `pid = fork()` berfungsi untuk menyimpan PID dari Child Process.
- `if(pid < 0){` mendeklarasikan bahwa nilai variabel pid < 0. `exit(EXIT_FAILURE);` berfungsi untuk keluar saat fork gagal.
- `exit(EXIT_SUCCESS)` berfungsi untuk keluar saat fork berhasil. `if(pid > 0)` mendeklarasikan bahwa nilai variabel pid adalah PID dari child process)
- `umask` berfungsi untuk mengatur permission dari suatu file pada saat file itu dibuat. Kita mengatur nilai `umask(0)` agar kita mendapatkan akses full terhadap file yang dibuat oleh daemon.
- Perintah `setsid()` berfungsi untuk mendapatkan SID. Perintah ini memiliki return type yang sama dengan perintah `fork()`. <br />

Proses Daemon adalah proses yang berjalan di background secara otomatis. Oleh sebab itu kita harus menutup file descriptor standar (file yang tidak diperlukan). 
```
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
```
- STDIN (standar input) berfungsi untuk menutup/ mencegah membaca data dari I/O. 
- STDOUT (standar output) berfungsi untuk menutup/mencegah menampilkan I/O ke layar. 
- STDERR (standar error) berfungsi untuk menutup/mecegah menampilkan pesan error ke layar.

Looping utama fungsi daemon
```
    while(1){
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

	        if(((tm.tm_hour == jam) || (jam == -5)) && ((tm.tm_min == menit) || (menit == -5)) && ((tm.tm_sec == detik) || (detik == -5))){
	            if(fork()==0){
		        execl("/bin/bash", "bash", argv[4], NULL);
		        sleep(1);
		    }

	    }
```
- Perintah `while(1)` berfungsi untuk looping, karena daemon akan bekerja dalam waktu tertentu.
- Perintah `time_t t` berfungsi untuk menyimpan waktu saat ini dalam variabel t.
- Perintah `struct tm tm` berisikan localtime yang akan mengonversi nilai dalam variabel t.
- Perintah `((tm.tm_hour == jam) || (jam == -5)) && ((tm.tm_min == menit) || (menit == -5)) && ((tm.tm_sec == detik) || (detik == -5))` berfungsi untuk menyamakan detik, menit, dan jam yang diinputkan oleh pengguna dengan jam, menit, dan detik pada saat program dijalankan. lalu Daemon akan berjalan sesuai dengan perintah yang telah diinputkan.
- Apabila kondisi perintah telah sesuai, maka program akan menjalankan bash script pada file yang telah dibuat dengan perintah `execl("/bin/bash", "bash", argv[4], NULL);`. 
- Perintah `sleep(1)` akan memberhentikan program selama 1 detik. Setelah itu program akan melakukan looping kembali.


##### Tampilan soal1 setelah dijalankan pada linux



## SOAL 2
Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat sebuah program. <br />
- Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp [YYYY-mm-dd_HH:ii:ss]. <br />
- Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari https://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss]. <br />
- Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip). <br />
- Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-generate sebuah program "killer" yang siap di run(executable) untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri. <br />
- Kiwa menambahkan bahwa program utama bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen -a. Untuk MODE_B, program harus dijalankan dengan argumen -b. Ketika dijalankan dalam MODE_A, program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).<br />

Kiwa lalu terbangun dan sedih karena menyadari programnya hanya sebuah mimpi. <br />
Buatlah program dalam mimpi Kiwa jadi nyata! <br />

Catatan: <br />
- Tidak boleh memakai system().
- Program utama harus ter-detach dari terminal <br />

Hint: <br />
- Gunakan fitur picsum.photos untuk mendapatkan gambar dengan ukuran
tertentu
- Epoch Unix bisa didapatkan dari time()

### Pembahasan SOAL 2
- Masuk ke directory soal 2 <br />
Syntax : `cd ../soal2`
- Membuat file <b>soal2.c</b> yang berisi program C<br />
Syntax : `nano soal2.c` <br />
___source code : [soal2.c](https://github.com/fikrihaykal/SoalShiftSISOP20_modul2_T20/blob/master/soal2/soal2.c)___ <br />
Penjelasan Script yang ada di dalam file <b>soal2.c</b> <br />

- Compile program <br />
Syntax : `gcc soal2.c -o soal2` <br />
- Menjalankan program <br />
Syntax : `./soal2`

##### Penjelasan Script yang ada di dalam file _soal2.c_ <br />
Library yang akan digunakan untuk algoritma di dalam fungsi main. <br /> 
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
```

Deklarasi variabel serta memulai proses `fork();` <br /> 
```
    FILE *fileTarget;
    fileTarget = fopen("killer.sh", "w");
    int status;
    pid_t pid, sid;
    pid = fork();
```
- Perintah `fileTarget = fopen("killer.sh", "w");` digunakan untuk membuat file `killer.sh`.<br />
- Perintah `fork()` digunakan untuk membuat _child process_ baru. <br />


Mengecek apakah perintah `fork();` berhasil membuat _child process_.<br /> 
```
    if(pid < 0){
        exit(EXIT_FAILURE);
    }

    if(pid > 0){
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if(sid < 0){
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
```
- Perintah `if(pid < 0)` digunakan jika _child process_ gagal dibuat.<br />
- Perintah `if(sid < 0)` digunakan jika _session ID_ gagal dibuat.<br />
- Perintah lainnya sesuai dengan template yang ada di modul materi.<br />

Membaca argumen sebagai perintah `bash` yang akan disimpan pada file `killer.sh`
```
    if(strcmp(argv[1], "-a") == 0){
        fprintf(fileTarget, "#!/bin/bash\nkill -9 -%d", getpid());
    } else if(strcmp(argv[1], "-b") == 0){
        fprintf(fileTarget, "#!/bin/bash\n/kill %d", getpid());
    }
```
- Fungsi tersebut akan menyimpan arguman `bash` ke dalam file `killer.sh`.<br />
- Jika argumennya adalah `-a` maka akan menyimpan `#!/bin/bash kill -9 -%d` dimana `%d` adalah _process ID_.<br />
- Jika argumennya adalah `-b` maka akan menyimpan `#!/bin/bash kill -%d` dimana `%d` adalah _process ID_.<br />

## SOAL 3
Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan (multiprocessing). <br />
- Program buatan jaya harus bisa membuat dua direktori di “/home/[USER]/modul2/”. Direktori yang pertama diberi nama “indomie”, lalu
lima detik kemudian membuat direktori yang kedua bernama “sedaap”.
- Kemudian program tersebut harus meng-ekstrak file jpg.zip di direktori “/home/[USER]/modul2/”. Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya.
- Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori “home/[USER]/modul2/jpg/”) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke “/home/[USER]/modul2/sedaap/” dan semua direktori harus dipindahkan ke “/home/[USER]/modul2/indomie/”.
- Untuk setiap direktori yang dipindahkan ke “/home/[USER]/modul2/indomie/” harus membuat dua file kosong. File yang pertama diberi nama “coba1.txt”, lalu 3 detik kemudian membuat file bernama “coba2.txt”. (contoh : “/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt”). <br />

Karena Jaya terlalu banyak tugas dia jadi stress, jadi bantulah Jaya agar bisa membuat
program tersebut. <br />

Catatan : <br />
- Tidak boleh memakai system().
- Tidak boleh memakai function C mkdir() ataupun rename().
- Gunakan exec dan fork
- Direktori “.” dan “..” tidak termasuk

### Pembahasan SOAL 
- Masuk ke directory soal 3 <br />
Syntax : `cd ../soal3`
- Membuat file <b>soal3.c</b> yang berisi program C<br />
Syntax : `nano soal2.c` <br />
___source code : [soal3.c](https://github.com/fikrihaykal/SoalShiftSISOP20_modul2_T20/blob/master/soal3/soal3.c)___ <br />
Penjelasan Script yang ada di dalam file <b>soal3.c</b> <br />

- Compile program <br />
Syntax : `gcc soal3.c -o soal3` <br />
- Menjalankan program <br />
Syntax : `./soal3`


## Kendala
- Bobot soal terlalu sulit
