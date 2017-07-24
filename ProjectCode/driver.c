/* Group - 20
* Yash Sinha 2012C6PS365P
* Krishna Garg 2012A7PS033P
*/
#include <stdio.h>
#include <stdlib.h>
#ifndef READBUFFER
#define READBUFFER 80
#endif

int main(int argc, char *argv[]) {
	printf("BATCH: 20 (Krishna Garg 2012A7033P, Yash Sinha 2012C6PS365P)\n");
	printf("============================================================\n");
	FILE* src = NULL;
	// if(argc < 2) {
	// 	printf("Please enter filename: \n");
	// 	char srcName[READBUFFER];
	// 	scanf("%s", srcName);
	// 	src = fopen(srcName, "r");
	// 	if (src == NULL) {
	// 		printf("Error: No file with filename %s exists\n", srcName);
	// 		exit(1);
	// 	}
	// 	printf("Compiling file: %s\n", srcName);
	// }
	// else {
		src = fopen(argv[1], "r");
		// src = fopen("test-scope.c", "r");

		if (src == NULL) {
			printf("Error: No file with filename %s exists\n", argv[1]);
			exit(1);
		}
		// printf("Compiling file: %s\n\n", argv[1]);
	// }
	lexAnalysis(src);
	makeParseTree(src);
	generateCode(src);
	return 0;
}
