#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define CRED     "\x1b[31m"
#define CGREEN   "\x1b[32m"
#define CYELLOW  "\x1b[33m"
#define CBLUE    "\x1b[34m"
#define RESET 	 "\x1b[0m"


void intHandler(int dummy) {
	// printf("%s\n", "OPA GANGNAM");
}

void printFormattedStringWithoutNewline(char * s, int typeOfQuote){
	int i = 0;
	for (i = 0; s[i] != '\0'; ++i){
		if (typeOfQuote == 1){
			if (s[i] != '"'){
				printf("%c", s[i]);
			}
		} else if (typeOfQuote == 2){
			if (s[i] != '\''){
				printf("%c", s[i]);
			}
		}
	}
}

void handleEcho(int numOfEle, char * tokens[]){
	int beginQuote = -1;
	int endQuote = -1;
	int typeOfQuote = -1;
	// printf("%s\n", tokens[1]);
	if (tokens[1] != NULL) {
		int j = 0;
		for (j = 1; j < numOfEle; ++j) {
			if (tokens[j][0] == '"' && typeOfQuote == -1){
				beginQuote = j;
				typeOfQuote = 1;
			} else if (tokens[j][0] == '\'' && typeOfQuote == -1) {
				beginQuote = j;
				typeOfQuote = 2;
			}
			if (tokens[j][strlen(tokens[j]) - 1] == '"' || tokens[j][strlen(tokens[j]) - 1] == '\''){
				endQuote = j;
				break;
			}
		}
		if (beginQuote == -1 || endQuote == -1){
			printf("%s\n", "Quote Error.");
			return;
		}
	}
	if (tokens[1] != NULL){
		if (*tokens[1] == '-'){
			if (tokens[2] != NULL){
				int k = 0;
				int tempFlag = 0;
				for (k = beginQuote; k <= endQuote; ++k){
					if (strcmp(tokens[1], "-n") == 0){
						// printf("%s", tokens[2]+1);
						printFormattedStringWithoutNewline(tokens[k], typeOfQuote);
						printf(" ");					
					} else if (strcmp(tokens[1], "-E") == 0){
						printFormattedStringWithoutNewline(tokens[k], typeOfQuote);
						tempFlag = 1;
						printf(" ");
					} else if ((strcmp(tokens[1], "-nE") == 0) || (strcmp(tokens[1], "-En") == 0)){
						printFormattedStringWithoutNewline(tokens[k], typeOfQuote);
						printf(" ");
					} else {
						printf("%s\n", "Invalid Argument");
					}
				}
				if (tempFlag == 1){
					printf("\n");
				}
			} else {
				printf("%s\n", "Invalid Argument/Syntax.");
			}
		} else {
			int iter = 0;
			int k = 0;
			for (k = beginQuote; k <= endQuote; ++k){
				for (iter = 0; *(tokens[k]+iter) != '\0'; ++iter){
					if (typeOfQuote == 1){
						if (*(tokens[k]+iter) != '"'){
							printf("%c", *(tokens[k]+iter));
						}
					} else if (typeOfQuote == 2) {
						if (*(tokens[k]+iter) != '\''){
							printf("%c", *(tokens[k]+iter));
						}
					}
				}
				printf(" ");
			}
			printf("\n");
		}
	} else {
		printf("\n");
	}
}

void readHistory(char * commandPath){
	// printf("%s\n", commandPath);	
	char historyPath[1024];
	strcpy(historyPath, commandPath);
	strcat(historyPath, "/history.txt");
	// printf("%s\n", historyPath);
	FILE * fptr = fopen(historyPath, "r");
	if (!fptr){
		printf("Error can't open history file\n");
	} else {
		char ch;
		while ((ch = getc(fptr)) != EOF){
			printf("%c", ch);
		}
	}
	fclose(fptr);
}

void readLastNLines(char *commandPath, int n){
	char historyPath[1024];
	int lines = 0;
	strcpy(historyPath, commandPath);
	strcat(historyPath, "/history.txt");
	// Get total number of lines into Lines variable
	FILE * fptr = fopen(historyPath, "r");
	if (!fptr){
		printf("Error can't open history file\n");
	} else {
		char ch;
		while ((ch = getc(fptr)) != EOF){
			if (ch == '\n')
				++lines;
		}
	}
	fclose(fptr);

	// printf("%s\n", historyPath);
	// Print only last n lines.
	int counter = 0;
	fptr = fopen(historyPath, "r");
	if (!fptr){
		printf("Error can't open history file\n");
	} else {
		char ch;
		while ((ch = getc(fptr)) != EOF){
			if (ch == '\n')
				++counter;
			if (counter > lines - n)
				printf("%c", ch);
			else if (counter == lines - n){
				if (ch != '\n')
					printf("%c", ch);
			}
		}
	}
	fclose(fptr);
}

void clearHistory(char * commandPath){
	char historyPath[1024];
	strcpy(historyPath, commandPath);
	strcat(historyPath, "/history.txt");
	FILE * fptr = fopen(historyPath, "w");
	fclose(fptr);
}

void redirectionError(int fd, int redirectFlag) {
	if (fd == -1){
		printf("Redirect flag value = %d\n", redirectFlag);
		printf("Error creating pipe. Command not executed\n");
		exit(1);
	}
}

int main(int argc, char *argv[]){
	char commandPath[1024];
	getcwd(commandPath, sizeof(commandPath));
	// printf("%s %s\n", "Path is ", commandPath);
	// strcat(commandPath, "./command/ls");
	// printf("%s\n", commandPath);
	char historyPath[1024];
	strcpy(historyPath, commandPath);
	strcat(historyPath, "/history.txt");
	// sighandler_t oldHandler;
	while (1) {
		char currentCommandPath[1024];
		strcpy(currentCommandPath, commandPath);
		char currentDir[1024];
		getcwd(currentDir, sizeof(currentDir));
		int pipeFlag = 0;
		int pipeCount = 0;
		printf("%s%s:%s%s%s", CYELLOW, "ush", currentDir, "$ ", RESET);
		char user_inp[101], *tokens[100];
		fgets(user_inp, sizeof(user_inp), stdin);

		if (user_inp[0] == '\n')
			continue;

		// Strcspn returns the offset to the string to reject from the beginning of the string
		user_inp[strcspn(user_inp, "\r\n") ] = 0;
		FILE *fHist = fopen(historyPath, "a");
		fprintf(fHist, "%s\n", user_inp);
		fclose(fHist);
		tokens[0] = strtok(user_inp, " ");
		int i = 0;
		while (tokens[i] != NULL){
			if (strcmp(tokens[i], "|") == 0){
				pipeFlag = 1;
				pipeCount = i;
			}
			++i;
			tokens[i] = strtok(NULL, " ");
		}
		tokens[i] = NULL;

		int redirectFlag = 0;
		int redirectIndex = 0;
		int j = 0;
		for (j = 0; j < i; ++j){
			// printf("%s\n", tokens[j]);
			if (strcmp(tokens[j], ">>") == 0){
				// printf("%d is the value of j\n", j);
				redirectFlag = 3;
				redirectIndex = j;
				break;
			}
			else if (*tokens[j] == '>'){
				redirectFlag = 1;
				redirectIndex = j;
				break;
			} else if (*tokens[j] == '<'){
				redirectFlag = 2;
				redirectIndex = j;
				break;
			}
		}
		// printf("%d redirectIndex of asd\n", redirectIndex);


		// for (int j = 0; j < i; ++j)	{
		// 	printf("%s\n", tokens[j]);
		// }
		int pid;
		// printf("%s is %d", "pipeFlag", pipeFlag);
		// printf("%s is %d", "pipeCount", pipeCount);
		if (pipeFlag == 1){
			goto pipeing;
		} else if (redirectFlag != 0){
			goto redirection;
		}
		if (strcmp(tokens[0], "exit") == 0){
			printf("%sEXITING%s\n", CRED, RESET);
			// fclose(fHist);
			exit(0);
		}
		else if (strcmp(tokens[0], "cd") == 0){
			if (tokens[1] != NULL){
				if (strcmp(tokens[1], "~") == 0){
					char * homeDir = getenv("HOME");
					chdir(homeDir);
					setenv("OLDPWD", currentDir, 1);
				} else if (strcmp(tokens[1], "-") == 0){
					char * oldPWD = getenv("OLDPWD");
					chdir(oldPWD);
					setenv("OLDPWD", currentDir, 1);
				} else {
					if (chdir(tokens[1]) != 0){
						printf("cd: no such file or directory: %s\n", user_inp+3);	
					}
				}
			} else {
				if (chdir(getenv("HOME")) != 0){
					printf("Cannot process\n");	
				} else 
					setenv("OLDPWD", currentDir, 1);
			}
		} else if (strcmp(tokens[0], "history") == 0){
			// printf("HEREoutside\n");
			// printf("%c\n", *tokens[1]);
			if (tokens[1] != NULL){
				int cnt = 0;
				int numFlag = 0;
				// printf("%s %d\n", tokens[1], strlen(tokens[1]));
				for (cnt = 0; cnt < strlen(tokens[1]); ++cnt){
					// printf("%c\n", *(tokens[1]+cnt));
					if (isdigit(*(tokens[1]+cnt)) == 0){
						numFlag = 0;
						break;
					} else{
						numFlag = 1;
					}
				}
				if (strcmp(tokens[1], "-c") == 0){
					clearHistory(commandPath);
				} else if (numFlag == 1){
					readLastNLines(commandPath, atoi(tokens[1]));
					// printf("%s\n", "HEREBOI");
				} else {
					printf("%s\n", "Invalid Argument.");
				}
			} else {
				// printf("%s\n", "OPA");
				readHistory(commandPath);
			}
		} else if (strcmp(tokens[0], "echo") == 0){
			handleEcho(i, tokens);
		} else if (strcmp(tokens[0], "pwd") == 0){
			if (tokens[1] != NULL) {
				if (*tokens[1] == '-') {
					printf("pwd: invalid option -- '%s'\n", tokens[1]);
					printf("%s\n", "Try 'pwd --help' for more information.");
				} else {
					printf("%s\n", "pwd: ignoring non-option arguments");
					printf("%s\n", currentDir);
				}
			} else {
				printf("%s\n", currentDir);
			}

		} else if (strcmp(tokens[0], "cat") == 0){
			strcat(currentCommandPath, "/cat");
			tokens[0] = currentCommandPath;
			// handleCat(currentDir, i, tokens);
			if ((pid = fork()) == 0){
				execvp(tokens[0], tokens);
			} else {
				signal(SIGINT, intHandler);
				wait(NULL);
			}
			// signal(SIGINT, oldHandler);
		} else if (strcmp(tokens[0], "date") == 0){
			strcat(currentCommandPath, "/date");
			// printf("%s\n", currentCommandPath);
			tokens[0] = currentCommandPath;
			// tokens[0] = "./date";
			if ((pid = fork()) == 0){
				execvp(tokens[0], tokens);
			} else {
				wait(NULL);
			}
		} else if (strcmp(tokens[0], "rm") == 0){
			strcat(currentCommandPath, "/rm");
			// printf("%s\n", currentCommandPath);
			tokens[0] = currentCommandPath;
			// tokens[0] = "./rm";
			if ((pid = fork()) == 0){
				execvp(tokens[0], tokens);
			} else {
				wait(NULL);
			}
		} else if (strcmp(tokens[0], "mkdir") == 0){
			strcat(currentCommandPath, "/mkdir");
			// printf("%s\n", currentCommandPath);
			tokens[0] = currentCommandPath;
			// tokens[0] = "./mkdir";
			if ((pid = fork()) == 0){
				execvp(tokens[0], tokens);
			} else {
				wait(NULL);
			}
		} else if (strcmp(tokens[0], "ls") == 0){
			strcat(currentCommandPath, "/ls");
			// printf("%s\n", currentCommandPath);
			tokens[0] = currentCommandPath;
			// tokens[0] = "./ls";
			if ((pid = fork()) == 0){
				execvp(tokens[0], tokens);
			} else {
				wait(NULL);
			}
		}
		// } else if (strcmp(tokens[0], "mkdir") == 0){
		// 	tokens[0] = "./mkdir";
		// 	printf("%s\n", tokens[1]);
		// 	if ((pid == fork()) == 0){
		// 		execvp(tokens[0], tokens);
		// 	} else {
		// 		wait(NULL);
		// 	}
		// }
		else {
			pipeing:
			// Piping is working for a single level of pipeing
			if (pipeFlag) {
				int descriptors[2];
				if (pipe(descriptors) == -1){
					printf("%s\n", "Error occurred. Exiting");
					exit(1);
				}

				// Open one fork to execute the first half of the pipe.
				// Duplicate the descriptor to act as standard output.
				if((pid = fork()) == 0){
					// close(STDOUT_FILENO);
					dup2(descriptors[1], 1);
					close(descriptors[0]);
					// close(descriptors[1]);	

					char* prog1[i+1];
					int j = 0;
					for (j = 0; j < pipeCount; ++j){
						prog1[j] = tokens[j];
					}
					prog1[j] = NULL;
					execvp(prog1[0], prog1);
					exit(1);
				}

				// Execute the second half of the pipe.
				// Duplicate file descriptor[0] to act as standard input.
				if((pid = fork()) == 0){
					dup2(descriptors[0], 0);
					close(descriptors[1]);
					// close(descriptors[0]);

					char* prog2[i - pipeCount + 1];
					int j = 0;
					int k = 0;
					for(j = pipeCount + 1; j < i; ++j){
						prog2[k] = tokens[j];
						++k;
					}
					prog2[k] = NULL;
					execvp(prog2[0], prog2);
					exit(1);
				}

				close(descriptors[0]);
				close(descriptors[1]);
				wait(0);
				wait(0);
			} else {
				redirection:
				if ((pid = fork()) == 0){
					if (redirectFlag != 0) {
						char * filePath[2];
						filePath[0] = tokens[redirectIndex+1];
						// printf("%s\n", filePath[0]);
						char * command[redirectIndex+1];
						int k = 0;
						for (j = 0; j < redirectIndex; ++j){
							command[j] = tokens[k];
							++k;
						}
						command[j] = NULL;
						int fd = -1;
						if (redirectFlag == 1){
							// printf("%s\n", "HERE IN ONE");
							fd = open(filePath[0], O_RDWR | O_CREAT, S_IRWXU);
							if (fd == -1){
								redirectionError(fd, redirectFlag);
							}
							dup2(fd, 1);
							// printf("%s %d\n", "fd value is", fd);
							close(fd);
						} else if (redirectFlag == 2) {
							// printf("%s\n", "HERE IN TWO");
							fd = open(filePath[0], O_RDWR);
							if (fd == -1){
								redirectionError(fd, redirectFlag);
							}
							dup2(fd, 0);
							// printf("%s %d\n", "fd value is", fd);

							close(fd);
						} else if (redirectFlag == 3) {
							// printf("%s\n", "HERE IN THREE");
							fd = open(filePath[0], O_RDWR | O_APPEND);
							if (fd == -1){
								redirectionError(fd, redirectFlag);
							}
							dup2(fd, 1);
							// printf("%s %d\n", "fd value is", fd);
							
							close(fd);
						}	
						execvp(command[0], command);
						close(fd);
						exit(1);
					}
					// dup2(STDERR_FILENO, STDOUT_FILENO);
					if (execvp(tokens[0], tokens) == -1){
						printf("%s%s%s\n", CGREEN, "Error executing command.",RESET);
					}
					exit(1);
				} else {
					wait(NULL);
				}
			}
		}	
	}
	return 0;
}
