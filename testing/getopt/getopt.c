#include <stdio.h>
#include <unistd.h>

int main(int argc,char* argv[]) {

	int c;
	
	while ( (c = getopt(argc,argv,"f:s:")) != -1)
	{
	switch(c) {
		case 's':
			printf("s: ");
			printf("%s\n",optarg);
			break;
		case 'f':
			printf("f: ");
			printf("%s\n",optarg);
			break;
	}
	}
	return 0;
}
