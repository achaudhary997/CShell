#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

int checkEmpty(char * path){
	DIR *dp;
	struct dirent *directoryPtr;
	dp = opendir(path);
	int counter = 0;
	if (dp == NULL){
		printf("ls: cannot access '%s': No such file or directory\n", path);
		return 0;
	} else {
		// while (directoryPtr = readdir(dp)){
		// 	printf("%s\t", directoryPtr->d_name);
		// }
		// directoryPtr = readdir(dp);
		for (directoryPtr = readdir(dp); directoryPtr != NULL ; directoryPtr = readdir(dp)){
			// printf("%s ", directoryPtr->d_name);
			++counter;
			if (counter > 2)
				break;
		}
		// printf("\n");
		if (counter <= 2) //Directory Empty
			return 1;
		else
			return 0;
		closedir(dp);
	}
}

int is_regular_file(const char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

void handleFlag(char * flag, char * fileName){
	if (strstr(flag, "-d") != NULL){
		if (opendir(fileName) != NULL){
			if (checkEmpty(fileName)){
				if (remove(fileName) != 0){
					printf("rm: cannot remove '%s': No such file or directory\n", fileName);
				}
			} else {
				printf("%s\n", "Not an empty directory.");
			}
		} else {
			printf("%s\n", "Not a directory/Cannot Delete");
		}
	} else if (strstr(flag, "-v") != NULL){
		if (is_regular_file(fileName)){
			if (remove(fileName) != 0){
				printf("rm: cannot remove '%s': No such file or directory\n", fileName);			
			} else {
				printf("removed '%s'\n", fileName);
			}
		} else {
			printf("%s\n", "Not a file");
		}
	}
}

int main(int argc, char * argv[]){
	// char * temp = "./Test.txt";
	int k = 0;
	if (argc >= 3){
		for (k = 2; k < argc; ++k){
			handleFlag(argv[1], argv[k]);
	// } else if (*argv[1] == '-') && argc[])
		}
	} else if (argc == 2){
		if (is_regular_file(argv[1])){
			if (remove(argv[1]) != 0){
				printf("rm: cannot remove '%s': No such file or directory\n", argv[1]);
			}
		} else {
			printf("%s\n", "Not a directory/file.");
		}
		// remove(temp);
	} else {
		printf("%s\n", "rm: missing operand/invalid format");
	}
	return 0;
}