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

int main(void){
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

    while(1){
        pid_t cid;
        int status;
        
        time_t t = time(NULL);
        struct tm* tm = localtime(&t);

        char namaFolder[64];
        strftime(namaFolder, 64, "%Y-%m-%d_%H:%M:%S", tm);
        
        cid = fork();

        if(cid < 0){
            exit(EXIT_FAILURE);
        }

        if(cid == 0){
            if(fork()==0){
                char *argv[] = {"mkdir", "-p", namaFolder, NULL};
                execv("/bin/mkdir", argv);
            } else{
                while((wait(&status)) > 0);
                for(int i=0; i<20; i++){
                    if(fork()==0){
                        if(chdir(namaFolder) < 0){
                            exit(EXIT_FAILURE);
                        }
                        
                        int cnt;
                        cnt = (int) time(NULL);
                        cnt = (cnt % 1000) + 100;
                        time_t tcount = time(NULL);
                        struct tm* tmcount = localtime(&tcount);

                        char alamatDownload[48], namaFile[64];
                        sprintf(alamatDownload, "https://picsum.photos/%d", cnt);
                        strftime(namaFile, 64, "%Y-%m-%d+%H:%M:%S", tmcount);

                        char *argvv[] = {"wget", alamatDownload, "-q", namaFile, NULL};
                        execv("/usr/bin/wget", argvv);
                    }
                    sleep(5);
                }

                if(fork() == 0){
                    char namaFolderZIP[64];
                    sprintf(namaFolderZIP, "%s.zip", namaFolder);

                    char *argvzip[] = {"zip", "-q", namaFolderZIP, namaFolder, NULL};
                    execv("/usr/bin/zip", argvzip);
                }
            }
        } else{
            sleep(30);
        }
    }
}
