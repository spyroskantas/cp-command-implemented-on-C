#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "minsc.h"

#define BUFFSIZE 4096

char* toPath(char* src, char* file){
	int size = strlen(src) + strlen(file) + 2;
	char *path = malloc(size);
	snprintf(path, size, "%s/%s", src, file);
	return path;
}

int fileOfInterest(char *file, struct dirent *direntp, struct stat *finfo){
	if(direntp->d_ino == 0) return 0;
	if(stat(file, finfo) < 0) return 0;
	if(strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0) return 0;
	if((finfo->st_mode & S_IFMT) != S_IFREG && (finfo->st_mode & S_IFMT) != S_IFDIR) return 0;
	return 1;
}
