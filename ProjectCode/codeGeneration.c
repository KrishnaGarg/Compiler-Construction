/* Group - 20
* Yash Sinha 2012C6PS365P
* Krishna Garg 2012A7PS033P
*/
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 15


struct node {
	char *name;
	char *lexeme;
	int lineNum;
	int childCount;
	int ruleNum;
	struct node* children[MAX_SIZE]; // 15 children pointers
};


struct ASTNode {
	char *type;
	char *name;
	int childCount;
	int lineNum;
	struct node* pointerToParseTree;
	struct ASTNode* stmtArray[10];
	int stmtArrayCount;
	struct ASTNode* children[MAX_SIZE]; // 15 children pointers
};


extern struct ASTNode* recordArray[10];
extern struct ASTNode* executeArray[1024];
extern int recordArrayCount;
extern int executeArrayCount;
int g_recordCount;
int g_executeCount;
char *idArray[30];
int idCount = 0;

extern int tableSz;
extern char *idArray[];

extern struct SymbolTable *hashBucket;
extern char* varUsage[100][100];
extern int varUsageCount[100];

extern char* opUsage[100][100];
extern int opUsageCount[100];

extern char* opIDUsage[100][100];
extern int opIDUsageCount[100];

extern char* values[100][100];
extern int valuesCount[100];

void generateDeclStmt(){
	
    int i;
    for (i = 0; i < executeArrayCount; i++){
        if(strcmp(executeArray[i]->type,"decl_stmt")==0){
            // printf("%s %d %d\n", executeArray[i]->name,executeArray[i]->childCount,executeArray[i]->stmtArrayCount);
            int j;
            for (j = 0; j < executeArray[i]->stmtArrayCount; j++)
            {
                // printf("type = %s, name = %s, children = %d\n", executeArray[i]->stmtArray[j]->type,executeArray[i]->stmtArray[j]->name,executeArray[i]->stmtArray[j]->childCount);
                if(strcmp(executeArray[i]->stmtArray[j]->type,"basic_type"))
                {
                    idArray[idCount++] = executeArray[i]->stmtArray[j]->children[0]->name;
                }
            }
        }
    }
}
FILE *generateIDlist(FILE *fp){
	int i;
	for (i = 0; i < idCount; ++i)
	{
		fprintf(fp, "%s %s\n", idArray[i], "	dd	?");
	}
}
FILE *generateAssignStmt(FILE *fp){
	int i,j;
	for (i = 0; i < executeArrayCount; i++){	
		if(strcmp(executeArray[i]->type,"assign_stmt")==0){
			struct node *temp = executeArray[i]->pointerToParseTree->children[0]->children[1];
			if(temp->children[1]->childCount == 0){

			int idName = temp->children[0]->lexeme;
			// printf("%s\n", idName);

			for (j = 0; j < valuesCount[executeArray[i]->lineNum]; ++j) {
		    }
		        // printf("%s %d\n", values[executeArray[i]->lineNum][j-1],j-1);

			fprintf(fp, "%s%d%s%s%s\n", "MOV\tEAX,",atoi(values[executeArray[i]->lineNum][j-1]),"\nPUSH\tEAX\nPOP\tEAX\nMOV\t",idName,",EAX\n");
			}
		}
		else if(strcmp(executeArray[i]->type,"str_assign_stmt")==0){
			char *LHS = executeArray[i]->stmtArray[0]->children[1]->name;
			for (j = 0; j < valuesCount[executeArray[i]->lineNum]; ++j) {
		    }
			char *RHS = values[executeArray[i]->lineNum][j-1];
			// printf("%s %s\n",  LHS, RHS);
			fprintf(fp, "%s\n", "LEA	ESI,	tempString1");
			int i;
			int count = 0;
			for(i = 1; RHS[i]!='\0';i++){
				count++;
			}
			for(i = 1; i < count;i++){
				fprintf(fp, "%s %d %s %c %s\n", "MOV	[ESI + ",i-1,"],	'",RHS[i],"'");
			}	
			fprintf(fp, "%s %d %s\n", "MOV	[ESI + ",count-1,"],	'$'");

			// printf("%d\n", count);
			fprintf(fp, "%s\n", "EDI,	str");
			fprintf(fp, "%s\n", "destinationStringAddress,	EDI");
			fprintf(fp, "%s\n", "ESI,	tempString1");
			fprintf(fp, "%s\n", "MOV	sourceStringAddress,	ESI");
			fprintf(fp, "%s\n", "CALL	CopyString");
	// LEA	EDI,	str
	// MOV	destinationStringAddress,	EDI
	// LEA	ESI,	tempString1
	// MOV	sourceStringAddress,	ESI
	// CALL	CopyString
			
		}
	}
}
void generateCode(FILE *f){
	g_recordCount = recordArrayCount;
	g_executeCount = executeArrayCount;
	printf("________________________GENERATING_CODE_________________________________\n");
	// printexecuteArray(executeArray);
	FILE *fp = fopen("file.asm", "w");
	fprintf(fp, "%s\n", ".model tiny\n.stack\t10000\n.data");
	generateDeclStmt();
	generateIDlist(fp);
	fprintf(fp, "%s\n", "tempString1	db	100 dup($)");
	fprintf(fp, "%s\n", "tempString2	db	100 dup($)");
	fprintf(fp, "%s\n", "tempString3	db	100 dup($)");
	fprintf(fp, "%s\n", "tempString4	db	100 dup($)");
	fprintf(fp, "%s\n", "sourceStringAddress	dw	?");
	fprintf(fp, "%s\n", "destinationStringAddress	dw	?");
	
	//start code segment
	fprintf(fp, "%s\n", "\n.code\n.startup\n");
	generateAssignStmt(fp);
	// end program
	fprintf(fp, "%s\n", "\n.exit");
	fprintf(fp, "%s\n\n", "CopyString	PROC	NEAR");
	fprintf(fp, "%s\n", "LEA	ESI,	sourceStringAddress");
	fprintf(fp, "%s\n", "LEA	EDI,	destinationStringAddress");
	fprintf(fp, "%s\n", "CopyStringLoop_1:");
	fprintf(fp, "%s\n", "CMP	BYTE PTR[ESI],	'$'");
	fprintf(fp, "%s\n", "JE	CopyStringEnd_1");
	fprintf(fp, "%s\n", "MOV	EAX,	BYTE PTR[ESI]");
	fprintf(fp, "%s\n", "MOV	BYTE PTR[EDI],	EAX");
	fprintf(fp, "%s\n", "INC	ESI");
	fprintf(fp, "%s\n", "INC	EDI");
	fprintf(fp, "%s\n", "JMP	CopyStringLoop_1");
	fprintf(fp, "%s\n", "CopyStringEnd_1:");
	fprintf(fp, "%s\n", "MOV	BYTE PTR[EDI],	'$'");
	fprintf(fp, "%s\n", "RET");
	fprintf(fp, "%s\n", "CopyString	ENDP");
	fprintf(fp, "%s\n", "\nend");
}