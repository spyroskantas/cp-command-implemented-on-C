#include "removeFunctions.h"

int removeDir(int flag, char *folder){
	DIR *dir;
	char* file;
	struct stat finfo;
	struct dirent *dirp;
	
	if(!(dir = opendir(folder))) return -1;
	
	while((dirp = readdir(dir))){
		file = toPath(folder, dirp->d_name);
		if(!fileOfInterest(file, dirp, &finfo)) continue;
		
		if((finfo.st_mode & S_IFMT) == S_IFREG){
			if(flag) printf("%s deleted\n", file);
			remove(file);
		}
		if((finfo.st_mode & S_IFMT) == S_IFDIR)
			removeDir(flag, file);
		free(file);
	}
	if(flag) printf("%s deleted\n", folder);
	closedir(dir);
	rmdir(folder);
	return 0;
}

int removeDel(int flag, char* src, char* des){
	int exists;
	char* newDes;
	DIR *desDir, *srcDir;
	struct stat desInfo;
	struct dirent *desDirentp, *srcDirentp;
	
	if(!(desDir = opendir(des)) || !(srcDir = opendir(src))) return -1;
	
	//search if destination folder file exists in source folder and act appropriately
	while((desDirentp = readdir(desDir))){
		newDes = toPath(des, desDirentp->d_name);
		if(!fileOfInterest(newDes, desDirentp, &desInfo)) continue;
		//read all files from source folder to check if destination folder file is there or delete it
		rewinddir(srcDir);
		exists=0;
		while((srcDirentp = readdir(srcDir)))
			if(srcDirentp && strcmp(desDirentp->d_name, srcDirentp->d_name) == 0){
				exists=1; break;
			}
			
		if(!exists)
			if((desInfo.st_mode & S_IFMT) == S_IFREG){
				//if file doesn't exist on source folder delete it
				if(flag) printf("%s deleted\n",newDes);
				remove(newDes);
			}
			else if((desInfo.st_mode & S_IFMT) == S_IFDIR){
				removeDir(flag, newDes);
			}
		free(newDes);
	}
	closedir(desDir); closedir(srcDir);
}
