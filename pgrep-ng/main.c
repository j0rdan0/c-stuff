#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#define SIZE 256

void getCmdline(char* pid) {
        char* path = malloc(256);
        sprintf(path,"/proc/%s/cmdline",pid);

        FILE* f = fopen(path,"r");
        if (!f) {
                if( errno == 2) {// file not found
                        printf("Invalid PID\n");  // most likely process PID incorrect or process no longer runs        
                }  
                free(path);     
                return;
        }
        free(path);

        char* comm = malloc(SIZE);
        fread(comm,SIZE,1,f);

        if ( strlen(comm) == 0) {
                char *statPath = malloc(256);
                sprintf(statPath,"/proc/%s/stat",pid); // use /proc/$PID/stat for kernel threads to fetch comm

                FILE* f = fopen(statPath,"r");
                free(statPath);
                char* comm = malloc(SIZE);
                fread(comm,SIZE,1,f);
                int start; // need to get start of cmdline
                for(int i = 0; i < strlen(comm);i++) {
                        if(comm[i] == '(') {
                                start = i;
                        }
                }
                for(int i=start+1; comm[i] != ')';i++) {
                        printf("%c",comm[i]);
                }
                printf("\n");
                free(comm);
                fclose(f);
                
        }
        printf("%s\n",comm);
        free(comm); 
        fclose(f);
}

int main(int argc, char* argv[]) {

        if(argc < 2) {
                printf("Usage: %s <PID>\n",argv[0]);
        return -1;
        }

getCmdline(argv[1]);
return 0;
}

