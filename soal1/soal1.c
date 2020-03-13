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

//Fungsi utama
int main(int argc, char **argv){
    if(argc != 5){
        //Mengecek jumlah argumen
        printf("Argumen anda salah\n");
        exit(EXIT_FAILURE);
    } else{
        int jadwal[4];

        //Mengecek apakah argumen benar dengan memanggil fungsi cekAsterisk() dan cekAngka()
        for(int i=1; i<4; i++){
        	char temp[] = argv[i];
        	
        	//Mengecek apakah argumen adalah asterisk
            if((strlen(temp) == 1) && (temp[0] == '*'))){
                jadwal[i] = -5;
            } else{
            	
            	//Mengecek apakah argumen bukan sebuah nomor
            	for(int i=0; i<strlen(temp); i++){
            		if((temp[i]) < '0') || (temp[i] > '9')){
            			printf("Argumen anda salah\n");
            			exit(EXIT_FAILURE);
					}
				}
            }
        }

        if((jadwal[1] != -5) || (jadwal[2] != -5) || (jadwal[3] != -5)){
            if((jadwal[1] >= 60) || (jadwal[2] >= 60) || (jadwal[3]) >= 24){
                printf("Argumen anda salah\n");
                exit(EXIT_FAILURE);
            }
        }

        int jam, menit, detik;
        jam = jadwal[3];
        menit = jadwal[2];
        detik = jadwal[1];

        //Memulai fork
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

        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        //Looping utama fungsi daemon
        while(1){
            time_t t = time(NULL);
            sturct tm tm = *localtime(&t);

			if(fork() == 0){
	            if(((tm.tm_hour == jam) || (jam == -5)) && ((tm.tm_min == menit) || (menit == -5)) && ((tm.tm_sec == detik) || (detik == -5))){
	                char *argvv = {"bash", argv[4], NULL};
	                execv("/bin/bash", argvv);
	            }
			}

            sleep(1);
        }
    }
}
