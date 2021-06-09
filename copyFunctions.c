#include"copyFunctions.h"

int copyFile(char *src, char *des){
	int fdr, fdw, nread, bytes=0;
	char buf[BUFFSIZE];
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	
	//open files
	if((fdr = open(src, O_RDONLY)) == -1) return -1;
	if((fdw = open(des, O_WRONLY | O_CREAT | O_TRUNC , mode)) == -1) return -2;
	
	//copy files
	while((nread = read(fdr, buf, BUFFSIZE)) > 0){
		if(write(fdw, buf, nread) < nread){
			close(fdr); close(fdw); return -3;
		}
		bytes += nread;
	}
	close(fdr); close(fdw);
	return bytes;
}

copyInfo* copyDir(int flag[], char src[], char des[]){
	copyInfo *info, *cpInfo = malloc(sizeof(copyInfo));
	int bytes;
	char *newSrc, *newDes;
	DIR *srcDir;
	struct stat finfo, desInfo;
	struct dirent *srcDirentp;
	cpInfo->bytes=0; cpInfo->copied=0; cpInfo->entities=0;
	
	//create folder if it does not exist
	if(stat(des, &finfo) == -1){
		mkdir(des, 0700);
		cpInfo->copied++;
		if(flag[0])
			printf("%s\n", des);
	}
	//open source directory	
	if(!(srcDir = opendir(src))){
		fprintf(stderr, "cannot open folder: %s\n", src);
		return NULL;
	}
	if(flag[1])
		removeDel(flag[0], src, des);
		
	while((srcDirentp = readdir(srcDir))){
		//generate source-file path
		newSrc = toPath(src, srcDirentp->d_name);
		if(!fileOfInterest(newSrc, srcDirentp, &finfo)) continue;
		newDes = toPath(des, srcDirentp->d_name);
		
		cpInfo->entities++;
		
		if((finfo.st_mode & S_IFMT) == S_IFREG){
			//check for same file
			if(!stat(newDes, &desInfo)){
				//same name, modification date, size = same file
				if(finfo.st_ctime == desInfo.st_ctime || finfo.st_size == desInfo.st_size)
					continue;
			}
			//if file is changed update file
			if(flag[0])
				puts(newDes); //print file to copy
				
			if((bytes = copyFile(newSrc, newDes)) < 0){
				fprintf(stderr, "Error copying file: %s\n", newSrc);
				return NULL;
			}
			cpInfo->bytes += bytes;
			cpInfo->copied++;
		}
		if((finfo.st_mode & S_IFMT) == S_IFDIR){
		
			if(!(info = copyDir(flag, newSrc, newDes))){
				fprintf(stderr, "Error copying folder: %s\n", newSrc);
				return NULL;
			}
			cpInfo->bytes += info->bytes;
			cpInfo->copied += info->copied;
			cpInfo->entities += info->entities;
			free(info);
		}
		free(newSrc); free(newDes);
	}
	closedir(srcDir);
	return cpInfo;
}
