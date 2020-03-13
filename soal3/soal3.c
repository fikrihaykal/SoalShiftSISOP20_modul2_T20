#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <dirent.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include <sys/wait.h>

int main(void){
  pid_t cid1, cid2, cid3, cid4, cid5, cid6;
  int status;

  cid1 = fork();
  if(cid1 < 0){
    exit(EXIT_FAILURE);
  }

  if(cid1 == 0){
    char *argv[] = {"mkdir", "-p", "indomie", NULL};
    execv("/bin/mkdir", argv);
  } else{
    while((wait(&status)) > 0);
    cid2 = fork();

    if(cid2 < 0){
      exit(EXIT_FAILURE);
    }

    if(cid2 == 0){
      sleep(5);
      char *argv[] = {"mkdir", "-p", "sedaap", NULL};
      execv("/bin/mkdir", argv);
    } else{
      while((wait(&status)) > 0);
      cid3 = fork();

      if(cid3 < 0){
        exit(EXIT_FAILURE);
      }

      if(cid3 == 0){
        char *argv[] = {"unzip", "-q", "jpg.zip", NULL};
        execv("/usr/bin/unzip", argv);
      } else{
        while((wait(&status)) > 0);
        cid4 = fork();

        if(cid4 < 0){
          exit(EXIT_FAILURE);
        }

        if(cid4 == 0){
          execl("/usr/bin/find", "find", "/home/fikri/Modul2/Soal3/jpg", "-type", "f", "-name", "*", "-exec", "mv", "-t", "/home/fikri/Modul2/Soal3/sedaap", "{}", "+", (char *) NULL);
        } else{
          while((wait(&status)) > 0);
          cid5 = fork();

          if(cid5 < 0){
            exit(EXIT_FAILURE);
          }

          if(cid5 == 0){
            execl("/usr/bin/find", "find", "/home/fikri/Modul2/Soal3/jpg/", "-mindepth", "1", "-type", "d", "-name", "*", "-exec", "mv", "-t", "/home/fikri/Modul2/Soal3/indomie/", "{}", "+", (char *) NULL);
          } else{
            while(wait((&status)) > 0);
            cid6 = fork();

            if(cid6 < 0){
              exit(EXIT_FAILURE);
            }

            if(cid6 == 0){
              execl("/usr/bin/find", "find", "/home/fikri/Modul2/Soal3/indomie", "-mindepth", "1", "-type", "d", "-name", "*", "-exec", "sh", "-c", "for d; do touch $d/coba1.txt;done", "{}", "+", (char *) NULL);
            } else{
              while(wait(&status) > 0);
              sleep(3);
              execl("/usr/bin/find", "find", "/home/fikri/Modul2/Soal3/indomie", "-mindepth", "1", "-type", "d", "-name", "*", "-exec", "sh", "-c", "for d; do touch $d/coba2.txt;done", "{}", "+", (char *) NULL);
            }
          }
        }
      }
    }
  }
}
