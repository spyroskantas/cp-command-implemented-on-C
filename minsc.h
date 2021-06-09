#pragma once
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

#define BUFFSIZE 4096

typedef struct{
	int bytes;
	int entities;
	int copied;
}copyInfo;

char* toPath(char*, char*);
int fileOfInterest(char*, struct dirent*, struct stat*);
