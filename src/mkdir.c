#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void makeDir(char *flag, char *dirName){
	if (strstr(flag, "-v") != NULL){
		if (mkdir(dirName, 0777) != 0){
			printf("Unable to create directory '%s'\n", dirName);
		} else {
			printf("mkdir: created directory '%s'\n", dirName);
		}
	} else {
		printf("%s\n", "Invalid Format");
	}
}

void makeDirWithPermission(char *permission, char *dirName){
	umask(0);
	// mode_t process_mask = umask(0);
	// int result_code = mkdir(argv[3], 0777);
	// umask(process_mask);
	// int filePermission = atoi(argv[2]);
	// if (chmod(argv[3], filePermission) != 0)
	// 	printf("%s\n", "FAILED");
	char * ptr;
	long perm = strtoul(permission, &ptr, 8);
	// int mode = atoi(argv[2]);
	// printf("%d\n", mode);
	if (mkdir(dirName, perm) != 0){
		printf("Unable to create directory '%s'\n", dirName);				
	}
	umask(0002);
}

void makePathDir(char *path) {
	int i = 0;
	for (i = 0; i < strlen(path); ++i){
		if (*(path+i) == '/'){
			*(path+i) = '\0';
			if (mkdir(path, S_IRWXU) != 0){
				printf("%s\n", "Unable to create directory");
			}
			*(path+i) = '/';
		}
	}
	if (mkdir(path, S_IRWXU) != 0){
		printf("%s\n", "Unable to create directory");
	}
	
}

int main(int argc, char * argv[]){
	int k = 0;
	if (argc > 2){
		if (strstr(argv[1], "-v") != NULL){
			for (k = 2; k < argc; ++k){
				makeDir(argv[1], argv[k]);
			}
		} else if (strcmp(argv[1], "-p") == 0){
			// mode_t filePermission = getFilePermission(argv[2]);
			// getFilePermission(argv[2]);
			// mode_t perm = atoi(argv[2]);
			for (k = 2; k < argc; ++k){
				makePathDir(argv[k]);
			}
		} else {
			for (k = 1; k < argc; ++k){
				if (mkdir(argv[k], 0777) != 0){
					printf("Unable to create directory '%s'\n", argv[k]);
				}
			}
		}
	} else if (argc == 2) {
		if (*argv[1] != '-'){
			if (mkdir(argv[1], 0777) != 0){
				printf("Unable to create directory '%s'\n", argv[1]);
			}
		} else {
			printf("mkdir: invalid option -- '%s'\n", argv[1]+1);
		}
	} else {
		printf("%s\n", "Invalid format cannot parse.");
	}
	return 0;
}