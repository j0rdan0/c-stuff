#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

if(argc < 2) {
	printf("Usage: %s <PID>\n",argv[0]);
	return -1;
}

char* path = malloc(256);

sprintf(path,"/proc/%s/cmdline\n",argv[1]);

FILE* f = fopen(path,"r");
if (!f) {
	if( errno == 2) // file not found
		printf("Invalid PID\n"); // most likely process PID incorrect or process no longer runs
	return -1;
}

free(path);

struct stat st;

stat(path,&st);

char* comm = malloc(256);
fread(comm,st.st_size,1,f);
printf("%s\n",comm);
free(comm); 

return 0;
}
