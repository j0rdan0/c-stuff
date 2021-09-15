#include <stdio.h>
#include <MegaMimes.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	MegaFileInfo * fileinfo = getMegaFileInformation(argv[1]) ;
	if (!fileinfo) {
		fprintf(stderr, "failed to get file info\n");
		abort();
	}
	if(fileinfo->mTextFile){
		printf("Text file, encoding: %s\n",fileinfo->mTextEncoding);
	}
	printf("%s %s\n",fileinfo->mMimeType,fileinfo->mMimeName);

	freeMegaFileInfo(fileinfo);
	
	return 0;
}
