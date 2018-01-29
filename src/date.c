#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void handleFlag(char * flag){
	// printf("HERE\n");
	if (strstr(flag, "-u") != NULL){
		char outputString[200];
		time_t currentTime;
		struct tm *timeTemp;
		char * timeFormat = "%a %d %b %Y %T %z";
		currentTime = time(NULL);
		timeTemp = gmtime(&currentTime);
		// timeString = ctime(&currentTime);
		strftime(outputString, sizeof(outputString), timeFormat, timeTemp);
		printf("%s\n", outputString);
	} else if (strstr(flag, "-R") != NULL){
		char outputString[200];
		char * timeString;
		time_t currentTime;
		struct tm *timeTemp;
		time(&currentTime);
		timeTemp = localtime(&currentTime);

		// time_t currentTime;
		// struct tm *timeTemp;
		char * timeFormat = "%a, %d %b %Y %T %z";
		// currentTime = time(NULL);
		// timeTemp = gmtime(&currentTime);
		// timeString = ctime(&currentTime);
		strftime(outputString, sizeof(outputString), timeFormat, timeTemp);
		printf("%s\n", outputString);
	} else {
		printf("date: invalid option -- '%s'\n", flag);
	}
}

int main(int argc, char * argv[]){
	// handleFlag("-R");
	// printf("\n");
	if (argc == 2){
		handleFlag(argv[1]);
	} else if (argc == 1){
		char outputString[200];
		char * timeString;
		time_t currentTime;
		struct tm *timeTemp;

		time(&currentTime);
		timeTemp = localtime(&currentTime);
		printf("%s", asctime(timeTemp));
	} else {
		printf("%s\n", "Invalid Format");
	}
	return 0;
}