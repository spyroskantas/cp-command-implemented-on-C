#include "copyFunctions.h"

int main(int argc, char** argv){
	int flag[]={0,0,0};
	double time;
	char *src, *des;
	copyInfo *cpInfo; 
	clock_t begin, end;
	
	if(argc < 3){
		puts("usage: source-dir dest-dir");
		exit(-1);
	}
	src = malloc(sizeof(argv[argc-2]));
	des = malloc(sizeof(argv[argc-1]));
	strcpy(src, argv[argc-2]);
	strcpy(des, argv[argc-1]);
	
	while(--argc){
		if(strcmp(argv[argc], "-v") == 0) flag[0]=1;
		if(strcmp(argv[argc], "-d")== 0) flag[1]=1;
		if(strcmp(argv[argc], "-l") == 0) flag[2]=1;
	}
	
	begin = clock();
	if(!(cpInfo = copyDir(flag, src, des))){
		fprintf(stderr, "Error copying directory\n");
		exit(-2);
	}
	end = clock();
	time = (double)(end-begin)/CLOCKS_PER_SEC;
	printf("there are %d files/directories in the hierarchy\n", cpInfo->entities);
	printf("number of entities copied is %d\n", cpInfo->copied);
	printf("copied %d bytes in %0.3f time at %0.2f bytes/sec\n", cpInfo->bytes, time, (double)cpInfo->bytes/time);
	free(src); free(des); free(cpInfo);
	return 0;
}
