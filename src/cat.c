#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <signal.h>

static volatile int loopBreaker = 1;

// void intHandler(int dummy) {
// 	loopBreaker = 0;
// 	exit(0);
// }

void readFile(char * flag, char * fileName){
	FILE * fptr = fopen(fileName, "rb");
	if (!fptr){
		printf("Error can't open file\n");
	} else {
		char ch;
		if (strstr(flag, "-n") != NULL){
			int counter = 1;
			printf("1 ");
			while ((ch = getc(fptr)) != EOF){
				printf("%c", ch);
				if (ch == '\n'){
					++counter;
					printf("%d ", counter);
				}
			}
		} else if (strstr(flag, "-E") != NULL) {
			while ((ch = getc(fptr)) != EOF){
				if (ch == '\n')
					printf("$");
				printf("%c", ch);
			}	
		} else if (strstr(flag, "-nE") != NULL || strstr(flag, "-En") != 0) {
			int counter = 1;
			while ((ch = getc(fptr)) != EOF){
				printf("%c", ch);
				if (ch == '\n'){
					printf("$\t%d  ", counter);
					++counter;
				}
			}
		}
		else {
			// char s[1024];
			// while (fscanf(fptr, "%s", s) != EOF){
			// 	printf("%s\n", s);
			// }
			while ((ch = getc(fptr)) != EOF){
				printf("%c", ch);
			}
		}
		printf("\n");
		fclose(fptr);
	}
}

int main(int argc, char * argv[]){
	// printf("%c\n", *argv[1]);
	// for(int i = 0; i < argc; ++i){
	// 	printf("Here %s\n", argv[i]);
	// }
	// printf("%d\n", argc);
	int k = 0;
	if (argc >= 3){
		if (*argv[1] == '-'){
			for (k = 2; k < argc; ++k){
				readFile(argv[1], argv[k]);
			}
		} else {
			for (k = 1; k < argc; ++k){
				// printf("%s\n", argv[k]);
				readFile("noFlag", argv[k]);
			}
		}
	} else if (argc == 2) {
		readFile("", argv[1]);
	} else if (argc == 1){
		char inputStr[1024];
		// signal(SIGINT, intHandler);
		while (loopBreaker){
			fgets(inputStr, 1000, stdin);
			printf("%s", inputStr);
		}
		// printf("%s\n", "HERE BOI");
	} else {
		printf("%s\n", "Invalid Syntax.");
	}
	// readFile("", argv[1]);
	return 0;
}
