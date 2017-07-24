/* Group - 20
* Yash Sinha 2012C6PS365P
* Krishna Garg 2012A7PS033P
*/
#include <stdio.h>
#include <string.h>
#define MAX_SIZE 15
#define TYPECHECKDEBUG 1

struct SymbolTable {
    int lineNum, key;
    char name[100];
    char datatype[20];
    int marker;
    int scope;
    int block;
    void *pointer;
};

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
extern struct ASTNode* procArray[10];
extern int recordArrayCount;
extern int executeArrayCount;
extern int procArrayCount;
extern struct SymbolTable *hashBucket;
extern int tableSz;
extern char *idArray[];


extern char* varUsage[100][100];
extern int varUsageCount[100];

extern char* opUsage[100][100];
extern int opUsageCount[100];

extern char* opIDUsage[100][100];
extern int opIDUsageCount[100];



struct node *findNode(struct node *n, char *str) {
    int i;
    if(strcmp(n->name,str) == 0){
        // printf("%s\n", n->name);
        return n;
    }
    for (i = 0; i < n->childCount; ++i)
    {
        struct node *node = findNode(n->children[i],str);
        if(node != NULL)
            return node;
    }
    return NULL; 
}

void check(char type, char b) {
    if(TYPECHECKDEBUG) printf("\nIN CHECK %c %c\n", type, b);
    if(type == b) {
        if(TYPECHECKDEBUG) printf("OK ");
    }
    else {
        if(TYPECHECKDEBUG) printf("..........ERROR ");// else printf("TYPECHECK ERROR \n");
        exit(1);
    }
}

void checkTypeofLHS_RHS(struct ASTNode *assign_stmt) {
    // printf("%s\n", assign_stmt->children[0]->name);
    struct ASTNode* LHS = assign_stmt->children[0]->stmtArray[1]->stmtArray[0];
    if(TYPECHECKDEBUG) printf("%s %d\n", LHS->name, assign_stmt->lineNum);
    int i, hashInd;
    char type = NULL;
    for (i = 0; i < opIDUsageCount[assign_stmt->lineNum]; ++i) {
        // if(TYPECHECKDEBUG) printf("%s \n", opIDUsage[assign_stmt->lineNum][i]);
        if((hashInd = getHashIndex(opIDUsage[assign_stmt->lineNum][i])) != -1) {
            if(TYPECHECKDEBUG) printf("%s ", hashBucket[hashInd].datatype);     
            if(type == NULL)
                type = hashBucket[hashInd].datatype[0];
            else check(type, hashBucket[hashInd].datatype[0]);
        }
        else {
            if(type == NULL)
                type = opIDUsage[assign_stmt->lineNum][i][0];
            else check(type, opIDUsage[assign_stmt->lineNum][i][0]);
        }
        // if(TYPECHECKDEBUG) printf("Type: %c ", type);
    }
    if(TYPECHECKDEBUG) printf("\n\n\n");
    // char* RHS = assign_stmt->children[0]->stmtArray[3]->pointerToParseTree->name;
    // printf("%s\n", RHS);

}

struct node *typeCheckExecute(struct ASTNode *executeArray[], int *executeArrayCount) {
	// if(n == NULL)
	// 	return NULL;
	// struct node* stmts = findNTNode(n,"stmts");
	// char* a = stmts->children[0]->children[0]->name;

	// checkTypeofLHS_RHS(stmts);
	// // printf("\n");
	// // printf("%s %d\n", executeArray[executeArrayCount]->name,executeArray[executeArrayCount]->childCount);
	// executeArrayCount++;

	// while(stmts->children[1]->childCount == 1) {
	// 	stmts = findNTNode(stmts->children[1],"stmts");
	// 	char* a = stmts->children[0]->children[0]->name;
	// 	checkTypeofLHS_RHS(stmts);
	// 	// printf("\n");
	// 	// printf("%s %d\n", executeArray[executeArrayCount]->name,executeArray[executeArrayCount]->childCount);
	// 	executeArrayCount++;
	// }
	// return NULL;
    int i;
    for (i = 0; i < *executeArrayCount; i++){
        // printf("%s\n", executeArray[i]->type);
        if(strcmp(executeArray[i]->type,"assign_stmt")==0){
           checkTypeofLHS_RHS(executeArray[i]);
        }
        else if(strcmp(executeArray[i]->type,"cond_stmt")==0){
            int j;
            for(j = 0; j < executeArray[i]->stmtArray[0]->stmtArrayCount; j++){
                if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->type,"cond_stmt") == 0){
                    int count = executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArrayCount;
                    if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray[0]->type,"decl_stmt") == 0){
                        typeCheckExecute(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray,&count);
                    }
                }
                else if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->type,"else_if") == 0){
                    int count = executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArrayCount;
                    if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray[0]->type,"decl_stmt") == 0){
                        typeCheckExecute(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray,&count);
                    }
                } 
                else if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->type,"_cond_stmt") == 0){
                    int count = executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArrayCount;
                    if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray[0]->type,"decl_stmt") == 0){
                        typeCheckExecute(executeArray[i]->stmtArray[0]->stmtArray[j]->stmtArray[0]->stmtArray,&count);
                    }
                }
            }  
        }
        else if(strcmp(executeArray[i]->type,"loop_stmt")==0){
            if(strcmp(executeArray[i]->stmtArray[0]->stmtArray[0]->type,"decl_stmt") == 0){
                int count = executeArray[i]->stmtArray[0]->stmtArrayCount;
                typeCheckExecute(executeArray[i]->stmtArray[0]->stmtArray,&count);
            }
        }
    }
}

void typeCheck(struct node *recordsNode, struct node *executeNode, struct node *procsNode) {
     printf("_____________Type_Checker__________________________________\n");
    // int i;
    // for(i = 0;i < procArrayCount; i++){
    //     typeCheckExecute(procArray[i]->stmtArray, &(procArray[i]->stmtArrayCount));
    // }
    typeCheckExecute(executeArray, &executeArrayCount);
    // typeCheckExecute(procArray, &procArrayCount);
}