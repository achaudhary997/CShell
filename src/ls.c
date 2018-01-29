#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

void printListing(int flag){
	DIR *dp;
	struct dirent *directoryPtr;
	dp = opendir("./");

	if (dp == NULL){
		printf("Error opening the directory");
	} else {
		// while (directoryPtr = readdir(dp)){
		// 	printf("%s\t", directoryPtr->d_name);
		// }
		// directoryPtr = readdir(dp);
		for (directoryPtr = readdir(dp); directoryPtr != NULL ; directoryPtr = readdir(dp)){
			if (flag == 1)
				printf("%s ", directoryPtr->d_name);
			else if (flag == 0) {
				if (strcmp(directoryPtr->d_name, ".") != 0 && strcmp(directoryPtr->d_name, "..") != 0 && *(directoryPtr->d_name) != '.' ){
					printf("%s ", directoryPtr->d_name);
				}
			} else if (flag == 2){
				if (strcmp(directoryPtr->d_name, ".") != 0 && strcmp(directoryPtr->d_name, "..") != 0 && *(directoryPtr->d_name) != '.' ){
					printf("%s, ", directoryPtr->d_name);
				}
			}
		}
		if (flag == 2)
			printf("\b\b ");
		printf("\n");
		closedir(dp);
	}
}

void printDirListing(char * path){
	DIR *dp;
	struct dirent *directoryPtr;
	dp = opendir(path);

	if (dp == NULL){
		printf("ls: cannot access '%s': No such file or directory\n", path);
	} else {
		// while (directoryPtr = readdir(dp)){
		// 	printf("%s\t", directoryPtr->d_name);
		// }
		// directoryPtr = readdir(dp);
		for (directoryPtr = readdir(dp); directoryPtr != NULL ; directoryPtr = readdir(dp)){
			printf("%s ", directoryPtr->d_name);
		}
		printf("\n");
		closedir(dp);
	}
}

int main(int argc, char * argv[]){
	// printf("HERE\n");
	if (argc == 1){
		printListing(0);
	} else if (argc == 2){
		if (strstr(argv[1], "-a") != NULL){
			printListing(1);
		} else if (strstr(argv[1], "-m") != NULL){
			printListing(2);
		} else {
			printDirListing(argv[1]);
		}
	} else {
		printf("%s\n", "Syntax Error Cannot Parse");
	}
	return 0;
}